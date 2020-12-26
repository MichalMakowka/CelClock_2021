/*
 * main.c
 *
 *  Created on: Dec 15, 2020
 *      Author: Michal Makowka
 */

#include "stm32g071xx.h"
#include "config.h"
#include "ws_lib.h"
#include "segment_dsp.h"
#include "clock.h"
#include "clk_menu.h"


int main(void) {

	uint8_t red, green, blue;	// Digit colour channel values

	TMENU menu_strct;

	SystemCFG();	// Configure uC registers



	// Clear LCD Display (display SPACEs)
	LEDClr();

	// Clear WS2812B Display
	FillLEDArray(LED_buf, 0, 0, 0);
	SPI_SEND_WSBUF(LED_buf, sizeof(LED_buf));

	// Scroll welcome message
	ScrollLed("hi celine", medium, sizeof("hi celine"));


	// Register menu executive functions
	menu_strct.configuration = (void*)displayConfig;
	menu_strct.alarm = (void*)displayAlarm;
	menu_strct.study = (void*)displayStudy;

	button_flag[B_UP]=0;
	button_flag[B_DOWN]=0;
	button_flag[B_SET]=0;


	dot_enable[1]++; dot_enable[3]++;	// Enable LED dots (for displaying the date)


	while (1) {

		// IF ADC idle, start conversion
		if ((ADC1->CR & ADC_CR_ADSTART) == 0) {
			ADC1->CR |= ADC_CR_ADSTART;		// Start ADC conversion
		}


		// Display current date
		DisplayNumberSet(day_t, day_u, month_t, month_u, year_t, year_u);

		// Assign current color values
		assignColour(hour_t, hour_u, &red, &green, &blue);
		// Display the current date


		// Display the current time
		FillLEDArray(LED_buf, 0, 0, 0);
		FillLEDNumber(LED_buf, 0, hour_t, red, green, blue);
		FillLEDNumber(LED_buf, 1, hour_u, red, green, blue);
		FillLEDNumber(LED_buf, 2, minute_t, red, green, blue);
		FillLEDNumber(LED_buf, 3, minute_u, red, green, blue);
		SPI_SEND_WSBUF(LED_buf, sizeof(LED_buf));
		delay_ms(990);
		FillLEDSegment(LED_buf, 26, 27, 0x0f, 0x03, 0);
		SPI_SEND_WSBUF(LED_buf, sizeof(LED_buf));
		delay_ms(990);

		if (button_flag[B_SET]) {
			button_flag[B_SET]=0;
			dot_enable[1]=0; dot_enable[3]=0;
			displayMenu(&menu_strct);
			dot_enable[1]=1; dot_enable[3]=1;
		}


	}

}


