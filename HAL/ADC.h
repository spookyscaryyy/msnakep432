/*
 * ADC.h
 *
 *  Created on: Apr 1, 2022
 *      Author: Will
 */

#ifndef HAL_ADC_H_
#define HAL_ADC_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>

void initADC(uint32_t mem1, uint32_t mem2);
void startADC();


#endif /* HAL_ADC_H_ */
