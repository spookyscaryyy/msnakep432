/*
 * Application.h
 *
 *  Created on: Mar 31, 2022
 *      Author: willbonner
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <HAL/HAL.h>
#include <HAL/Graphics.h>
#include <HAL/Timer.h>

#define GAME_TIME_SECONDS    30      // Game time length in seconds
#define SEED_BITMASK         0x6     // Bitmask for 2 least significant bits

#define HIGH_SCORE_AMT       3       // Amount of High Scores
#define SINGLE_CHAR_ARR_SIZE 2       // Array size for displaying a single character
#define TIMER_ARR_SIZE       10      // Max size for timer display array
#define SCORE_ARR_SIZE       5       // Max size for score display array
#define RECIPE_NAME_SIZE     15      // Max size for recipe name
#define NUM_CURSOR_LOCATIONS 2       // Amount of options on main menu

#define MENU_UP_THRESHOLD    15000   // Joystick thresholds used for menu selection
#define MENU_DOWN_THRESHOLD  1500

#define GAME_LEFT_THRESHOLD  3000    // Joystick thresholds used for game selection
#define GAME_RIGHT_THRESHOLD 12000
#define GAME_UP_THRESHOLD    12000
#define GAME_DOWN_THRESHOLD  12000

#define SCORE_INCREASE       5       // Money added when successful recipe

#define MILLI_IN_SECOND      1000    // Amount of milliseconds in a second

enum _GameState
{
    MAIN_MENU, HIGH_SCORE_SCREEN, GAME_SCREEN, RESULTS_SCREEN
};
typedef enum _GameState GameState;

enum _Cursor
{
    CURSOR_0 = 0, CURSOR_1 = 1
};
typedef enum _Cursor Cursor;

struct _Application
{
    // General admin stuff
    GameState state;    // Track what screen the game is on
    SWTimer gameTimer;  // Timer for game screen
    Cursor cursor;      // Used for the main menu selection

    // Arrays - don't typically get overwritten much
    uint32_t highScores[HIGH_SCORE_AMT];                        // Array of high scores

    // Game data stuff - overwritten every game
    uint32_t score;         // Current game score
    uint32_t time;          // Current game time
};
typedef struct _Application Application;


// Called only a single time - inside of main(), where the application is constructed
Application App_Snake_construct();

// Called once per super-loop of the main application.
void App_Snake_loop(Application* app, HAL* hal);

// Handle callback functions used for each state of the App_WordMaster_loop()
// function. State transitions are embedded inside of each of these functions.
void App_Snake_handleMenuScreen(Application* app, HAL* hal);
void App_Snake_handleScoreScreen(Application* app, HAL* hal);
void App_Snake_handleGameScreen(Application* app, HAL* hal);
void App_Snake_handleResultsScreen(Application* app, HAL* hal);

// Helper functions which clear the screen and draw the text for each state
void App_Snake_showMenuScreen(Application* app, HAL* hal);
void App_Snake_showScoreScreen(Application* app, HAL* hal);
void App_Snake_showGameScreen(Application* app, HAL* hal);
void App_Snake_showResults(Application* app, HAL* hal);

// Function to draw cursor
void App_Snake_drawCursor(Application* app, HAL* hal);

// Generic circular increment and decrement functions
uint32_t CircularIncrement(uint32_t value, uint32_t maximum);
uint32_t CircularDecrement(uint32_t value, uint32_t maximum);

// Generic function to fill an array with one value
void uintArrayFill(uint32_t* array, int32_t contents, uint32_t size);

// Function to create random seed
void initRandomSeed(HAL* hal_p);

#endif /* APPLICATION_H_ */
