/*
 * clock.h
 *
 *  Created on: Dec 22, 2020
 *      Author: Michal Makowka
 */

#ifndef INC_CLOCK_H_
#define INC_CLOCK_H_

#include <stdio.h>
#include <string.h>
#include "stm32g071xx.h"


volatile uint8_t hour_t;
volatile uint8_t hour_u;
volatile uint8_t minute_t;
volatile uint8_t minute_u;

volatile uint8_t year_t;
volatile uint8_t year_u;
volatile uint8_t month_t;
volatile uint8_t month_u;
volatile uint8_t day_t;
volatile uint8_t day_u;

// *** COLOUR DEFINITIONS ***
// Each time of the day has a corresponding colour of the clock.
// The sum of all three channels per T(X) should add up to 255! Otherwise, auto-brightness will be slightly disturbed!
// T1: 6:00 - 12:00
#define R_T1 160
#define G_T1 95
#define B_T1 0
// T2: 12:00 - 18:00
#define R_T2 100
#define G_T2 155
#define B_T2 0
// T3: 18:00 - 21:00
#define R_T3 170
#define G_T3 50
#define B_T3 45
// T4: 21:00 - 23:00
#define R_T4 155
#define G_T4 0
#define B_T4 100
// T5: 23:00 - 5:00
#define R_T5 0
#define G_T5 0
#define B_T5 170
// T6: 5:00 - 6:00
#define R_T6 0
#define G_T6 0
#define B_T6 170
// **************************


void setTime(uint8_t t_hour, uint8_t u_hour, uint8_t t_minute, uint8_t u_minute);
void setDate(uint8_t t_day, uint8_t u_day, uint8_t t_month, uint8_t u_month, uint8_t t_year, uint8_t u_year);

void checkIfAlarm(void);
void checkIfStudy(void);

#endif /* INC_CLOCK_H_ */
