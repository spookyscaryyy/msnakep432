/**
 * Starter code for Project 2. Good luck!
 */

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* HAL and Application includes */
#include <Application.h>
#include <HAL/HAL.h>

// Non-blocking check. Whenever Launchpad S1 is pressed, LED1 turns on.
static void InitNonBlockingLED()
{
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
}

// Non-blocking check. Whenever Launchpad S1 is pressed, LED1 turns on.
static void PollNonBlockingLED()
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == 0) {
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
    }
}

int main()
{
    WDT_A_holdTimer();

    // Initialize the system clock and background hardware timer, used to enable
    // software timers to time their measurements properly.
    InitSystemTiming();

    // Initialize the main Application object and HAL object
    HAL hal = HAL_construct();
    Application app = App_Snake_construct();

    initRandomSeed(&hal);

    // Do not remove this line. This is your non-blocking check.
    InitNonBlockingLED();

    // Draw the title screen to begin with
    App_Snake_showMenuScreen( &hal);

    while (1)
    {
        // Do not remove this line. This is your non-blocking check.
        PollNonBlockingLED();
        HAL_refresh(&hal);
        App_Snake_loop(&app, &hal);
    }
}

/**
 * The main constructor for your application. This function should initialize
 * each of the FSMs which implement the application logic of your project.
 *
 * @return a completely initialized Application object
 */
Application App_Snake_construct()
{
    Application app;

    // Initialization of FSM variables
    app.state = MAIN_MENU;
    app.gameTimer = SWTimer_construct(MILLI_IN_SECOND);

    app.cursor = CURSOR_0;

    // Initialize starting high scores and ingredient counts
    uintArrayFill(app.highScores, 0, HIGH_SCORE_AMT);

    return app;
}

/**
 * The main super-loop function of the application. We place this inside of a
 * single infinite loop in main. In this way, we can model a polling system of
 * FSMs. Every cycle of this loop function, we poll each of the FSMs one time,
 * followed by refreshing all inputs to the system through a convenient
 * [HAL_refresh()] call.
 *
 * @param app_p:  A pointer to the main Application object.
 * @param hal_p:  A pointer to the main HAL object
 */
void App_Snake_loop(Application* app_p, HAL* hal_p) {
    switch (app_p->state)
    {

    case MAIN_MENU:
        App_Snake_handleMenuScreen(app_p, hal_p);
        break;

    case HIGH_SCORE_SCREEN:
        App_Snake_handleScoreScreen(app_p, hal_p);
        break;

    case GAME_SCREEN:
        App_Snake_handleGameScreen(app_p, hal_p);
        break;

    case RESULTS_SCREEN:
        App_Snake_handleResultsScreen(app_p, hal_p);
        break;

    default:
        break;
    }

}

/**
 * Handles logic of the menu screen, if the joystick is pressed, the option located at
 * current cursor pos its chosen and the state/screen is updated
 *
 * @param app_p:  A pointer to the main Application object.
 * @param hal_p:  A pointer to the main HAL object
 */
void App_Snake_handleMenuScreen(Application* app_p, HAL* hal_p) {
    static bool justSwitched = false;
    if (Button_isTapped(&hal_p->boosterpackJS)) {
        switch (app_p->cursor) {
        case CURSOR_0:
            app_p->state = GAME_SCREEN;
            App_Snake_showGameScreen(app_p, hal_p);
            SWTimer_start(&app_p->gameTimer);
            break;
        case CURSOR_1:
            app_p->state = HIGH_SCORE_SCREEN;
            App_Snake_showScoreScreen(app_p, hal_p);
            break;
        }
    }

    if (!justSwitched) {
        if (hal_p->joy.y < MENU_DOWN_THRESHOLD) {
            uint32_t newCursor = CircularIncrement((uint32_t) app_p->cursor, NUM_CURSOR_LOCATIONS);
            app_p->cursor = (Cursor) newCursor;
            App_Snake_drawCursor(app_p, hal_p);
            justSwitched = true;
        }

        if (hal_p->joy.y > MENU_UP_THRESHOLD) {
            uint32_t newCursor = CircularDecrement((uint32_t) app_p->cursor, NUM_CURSOR_LOCATIONS);
            app_p->cursor = (Cursor) newCursor;
            App_Snake_drawCursor(app_p, hal_p);
            justSwitched = true;
        }
    }

    if (hal_p->joy.y > MENU_DOWN_THRESHOLD  && hal_p->joy.y < MENU_UP_THRESHOLD){
        justSwitched = false;
    }
}

/**
 * Handles logic of the high score screen, switches to the main menu screen
 * if the joystick is tapped.
 *
 * @param app_p:  A pointer to the main Application object.
 * @param hal_p:  A pointer to the main HAL object
 */
void App_Snake_handleScoreScreen(Application* app_p, HAL* hal_p) {
    if (Button_isTapped(&hal_p->boosterpackJS)) {
        app_p->state = MAIN_MENU;
        App_Snake_showMenuScreen(app_p, hal_p);
    }
}

/**
 * Handles logic of the game  screen, submits the ingredients
 * if the joystick is tapped. Moving the joystick will select the different ingredients,
 * tapping BB1 will add the ingredient, tapping BB2 will remove the ingredient. There
 * is a timer counting down and will switch to the result screen upon reaching 0
 *
 * @param app_p:  A pointer to the main Application object.
 * @param hal_p:  A pointer to the main HAL object
 */
void App_Snake_handleGameScreen(Application* app_p, HAL* hal_p) {
    if (app_p->time == 0 || app_p->strikes == MAX_STRIKES) {
        app_p->state = RESULTS_SCREEN;
        App_Snake_showResults(app_p, hal_p);
    }
}

/**
 * Handles the results screen. Pressing the joystick will move back to the menu screen and resets
 * the game variables
 *
 * @param app_p:  A pointer to the main Application object
 * @param hal_p:  A pointer to the main HAL object
 */
void App_Snake_handleResultsScreen(Application* app_p, HAL* hal_p) {
    if (Button_isTapped(&hal_p->boosterpackJS)) {
        app_p->state = MAIN_MENU;
        App_Snake_showMenuScreen(app_p, hal_p);
        App_Snake_initGameData(app_p);
    }
}

/**
 * Shows the main menu screen containing options to see the high scores,
 * instructions on how to play, and begin the game. Uses
 * the GFX_print function, clears the screen first
 *
 * @param hal_p:  A pointer to the main HAL object
 */
void App_Snake_showMenuScreen(Application* app_p, HAL* hal_p) {
    GFX_clear(&hal_p->gfx);

    GFX_print(&hal_p->gfx, "      mSNAKEp432     ", 1, 0);
    GFX_print(&hal_p->gfx, "       Get it?       ", 2, 0);

    GFX_print(&hal_p->gfx, " ---- Main Menu ---- ", 4, 0);

    GFX_print(&hal_p->gfx, "       Slither       ", 6, 0);

    GFX_print(&hal_p->gfx, "     High Scores     ", 8, 0);

    App_Snake_drawCursor(app_p, hal_p);
}

/**
 * Shows the blank high score screen which will containing the top 3 scores. Uses
 * the GFX_print function, clears the screen first
 *
 * @param hal_p:  A pointer to the main HAL object
 */
void App_Snake_showScoreScreen(Application* app_p, HAL* hal_p) {
    GFX_clear(&hal_p->gfx);

    GFX_print(&hal_p->gfx, "High Scores          ", 0, 0);
    GFX_print(&hal_p->gfx, "-----------          ", 1, 0);
    GFX_print(&hal_p->gfx, "1. ", 2, 0);
    GFX_print(&hal_p->gfx, "2. ", 3, 0);
    GFX_print(&hal_p->gfx, "3. ", 4, 0);

    int i;
    for (i = 0; i < HIGH_SCORE_AMT; i++) {
        char number[SCORE_ARR_SIZE];
        snprintf(number, SCORE_ARR_SIZE, "%u", app_p->highScores[i]);
        GFX_print(&hal_p->gfx, number, 2 + i, 3);
    }
}

/**
 * Shows the blank game screen containing the ingredients and their boxes.
 * uses an array of rectangles to define the boxes holding the ingredients
 *
 * @param app_p:  A pointer to the main Application object.
 * @param hal_p:  A pointer to the main HAL object
 */
void App_Snake_showGameScreen(Application* app_p, HAL* hal_p) {
    GFX_clear(&hal_p->gfx);

}

/**
 * Shows the results screen which will clear the screen and show the results
 * of the game, victory if a new high score is reached, else, failure
 *
 * @param app_p:  A pointer to the main Application object.
 * @param hal_p:  A pointer to the main HAL object
 */
void App_Snake_showResults(Application* app_p, HAL* hal_p) {
    GFX_clear(&hal_p->gfx);

    GFX_print(&hal_p->gfx, "      Game Over!     ", 0, 0);
    GFX_print(&hal_p->gfx, "No code to determine ", 1, 0);
    GFX_print(&hal_p->gfx, "High score yet :(    ", 2, 0);
}

/*
 * Handles cursor control on main menu screen, only switches options when joystick exceeds threshold
 * and will circularly increment through the options
 *
 * @param app_p:  A pointer to the main Application object.
 * @param hal_p:  A pointer to the main HAL object
 */
void App_Snake_drawCursor(Application* app_p, HAL* hal_p) {
    // clear previous cursor positions
    GFX_print(&hal_p->gfx, "    ", 6, 0);
    GFX_print(&hal_p->gfx, "    ", 8, 0);

    GFX_print(&hal_p->gfx, "   >", 6 + 2 * app_p->cursor, 0);
}

/**
 * A helper function which increments a value with a maximum. If incrementing
 * the number causes the value to hit its maximum, the number wraps around
 * to 0.
 */
uint32_t CircularIncrement(uint32_t value, uint32_t maximum)
{
    return (value + 1) % maximum;
}

/**
 * A helper function which decrements a value with a maximum. If decrementing
 * the number causes the value to hit 0, the number wraps around
 * to its maximum.
 */
uint32_t CircularDecrement(uint32_t value, uint32_t maximum)
{
    return (value + maximum - 1) % maximum;
}

/**
 * Fills an array of unsigned ints with one value up to a size. Does not contain boundary checking
 * so use with caution
 */
void uintArrayFill(uint32_t* array, int32_t contents, uint32_t size) {
    int i;
    for (i = 0; i < size; i++) {
        array[i] = contents;
    }
}

/**
 * Create the seed for the random number generator. Uses the 4 least significant bits of both x and y
 * joystick ADC values to make as random of a seed as possible
 *
 * @param hal_p:  A pointer to the main HAL object
 */
void initRandomSeed(HAL* hal_p) {
    uint32_t seed;
    seed = (hal_p->joy.x & SEED_BITMASK) << 2;
    seed += hal_p->joy.y & SEED_BITMASK;
    srand(seed);
}
