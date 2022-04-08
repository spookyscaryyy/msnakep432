/*
 * UART.h
 *
 *  Created on: Dec 31, 2019
 *      Author: Matthew Zhong
 *  Supervisor: Leyla Nazhand-Ali
 */

#ifndef HAL_UART_H_
#define HAL_UART_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// Ever since the new API update in mid 2019, the UART configuration struct's
// name changed from [eUSCI_UART_Config] to [eUSCI_UART_ConfigV1]. If your code
// does not compile, uncomment the alternate #define and comment the original
// one.
// -----------------------------------------------------------------------------
// #define UART_Config eUSCI_UART_Config
#define UART_Config eUSCI_UART_ConfigV1


#define USB_UART_PORT GPIO_PORT_P1
#define USB_UART_PINS (GPIO_PIN2 | GPIO_PIN3) // The pins are given to you for guidance. Also, because many students miss the parentheses
#define USB_UART_INSTANCE  EUSCI_A0_BASE


// An enum outlining what baud rates the UART_construct() function can use in
// its initialization.
enum _UART_Baudrate
{
    BAUD_9600, BAUD_19200, BAUD_38400, BAUD_57600, NUM_BAUD_CHOICES
};
typedef enum _UART_Baudrate UART_Baudrate;

/**=============================================================================
 * A simple UART object, implemented in the C object-oriented style. Use the
 * constructor [UART_construct()] to create an UART object. Afterwards, when
 * accessing each method of the UART object, pass in a pointer to the UART
 * object as the first argument of the method.
 * =============================================================================
 * USAGE WARNINGS
 * =============================================================================
 * When using this UART object, DO NOT DIRECTLY ACCESS ANY MEMBER VARIABLES of
 * the UART struct. Treat all members as PRIVATE - that is, you should ONLY
 * access a member of the UART struct if your function name starts with "UART_"!
 *
 * When receiving a character, ALWAYS USE THE UART_hasChar FUNCTION FIRST
 * Doing so will prevent the processor from being interrupted if there is no
 * character currently received
 *
 * When sending a character, ALWAYS USE THE UART_canSend FUNCTION FIRST
 * Doing so will prevent the processor from being interrupted if there is no
 * character currently to send.
 *
 */
struct _UART
{
    UART_Config config;

    uint32_t moduleInstance;
    uint32_t port;
    uint32_t pins;
};
typedef struct _UART UART;

// Constructs a uart using the moduleInstance at the given port and pin
UART UART_construct(uint32_t moduleInstance, uint32_t port, uint32_t pins);

// Sets the baudrate and enables a UART based on the given baudrate enum
void UART_SetBaud_Enable(UART *, UART_Baudrate baudrate);

// Determines if UART has received a character
bool UART_hasChar(UART* uart_p);

// Grabs a character received from UART
char UART_getChar(UART* uart_p);

// Determines if UART can send a character
bool UART_canSend(UART* uart_p);

// Sends a character through UART
void UART_sendChar(UART* uart_p, char c);

// Updates the UART baudrate to use the new baud choice.
void UART_updateBaud(UART* uart_p, UART_Baudrate baudChoice);

#endif /* HAL_UART_H_ */
