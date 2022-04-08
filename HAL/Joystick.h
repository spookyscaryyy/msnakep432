/*
 * Joystick.h
 *
 *  Created on: Apr 1, 2022
 *      Author: Will
 */

#ifndef HAL_JOYSTICK_H_
#define HAL_JOYSTICK_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

struct _Joystick
{
    unsigned x;
    unsigned y;
};
typedef struct _Joystick Joystick;

void initJoyStick();
void Joystick_refresh();


#endif /* HAL_JOYSTICK_H_ */
