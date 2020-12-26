/*
 * config.h
 *
 *  Created on: Dec 15, 2020
 *      Author: Michal Makowka
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

#include <stdio.h>
#include <string.h>
#include "stm32g071xx.h"

// RTC Calibration values
// By default, the value of PREDIV_A = 127, PREDIV_S = 255, CALP = 0, CALM = 0
#define PREDIV_A 127	// [val: 0-127]
#define PREDIV_S 251	// [val: 0-32767]
#define CALP 0			// [val: 0-1]
#define CALM 511		// [val: 0-511]
/*
 * Set RTC_OSC_CALIB_OUTPUT_EN to 1 if RTC_OUT clk signal
 * needs to be available on the PA4 (RTC_OUT2 - 1Hz); otherwise, set to 0.
 * This feature is useful to sample the RTC clk source by a logic analyser.
 * Set PREDIV_S, CALP, and CALM bits, so that the F_RTC_OUT2 = 1.000Hz
*/
#define RTC_OSC_CALIB_OUTPUT_EN 0


uint8_t d_flag;		// Delay flag

volatile uint8_t button_flag[3];

enum{B_SET, B_DOWN, B_UP};	// Intexes for the button_flag[3] array

void SystemCFG (void);
void delay_ms (uint16_t ms);








#endif /* INC_CONFIG_H_ */
