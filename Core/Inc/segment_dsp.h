/*
 * segment_dsp.h
 *
 *  Created on: Dec 21, 2020
 *      Author: Michal Makowka
 */

#ifndef INC_SEGMENT_DSP_H_
#define INC_SEGMENT_DSP_H_

#include <stdio.h>
#include <string.h>
#include "stm32g071xx.h"

#include "config.h"


volatile uint8_t dot_enable[6];
volatile uint16_t led_dsp[6];	// Array holding the LED display value


enum {slow = 350, medium = 200, fast = 100};	// LED display scroll speed
enum {OFF, ON};		// LCD dot enabled, disabled


void ScrollLed(char * data, uint16_t speed, uint8_t len);
void DisplayNumberSet(uint8_t n0, uint8_t n1, uint8_t n2, uint8_t n3, uint8_t n4, uint8_t n5);
void DisplayLEDStr(char * str);
void displayChar(char c, uint8_t pos);
void setDigit (uint8_t digit);

void LEDClr(void);


#endif /* INC_SEGMENT_DSP_H_ */
