/*
 * clock.c
 *
 *  Created on: Dec 22, 2020
 *      Author: Michal Makowka
 */

#include "clock.h"
#include "ws_lib.h"
#include "clk_menu.h"

#define TEMP_TR_MASK 0x7f7f7f
#define TEMP_DR_MASK 0xffff3f


void setTime(uint8_t t_hour, uint8_t u_hour, uint8_t t_minute, uint8_t u_minute) {
	if(t_hour > 2 || u_hour > 9 || t_minute > 5 || u_minute > 9) return;	// Verify the input

	uint32_t TR_TEMP = 0;	// Temporary RTC TR register


	// Load appropriate values to the temporary time register (TR)
	TR_TEMP |= (t_hour<<20);
	TR_TEMP |= (u_hour<<16);
	TR_TEMP |= (t_minute<<12);
	TR_TEMP |= (u_minute<<8);

	// RTC write protection keyes
	RTC->WPR = 0xca;
	RTC->WPR = 0x53;

	// Enter clock init mode
	RTC->ICSR |= RTC_ICSR_INIT;
	while ((RTC->ICSR & RTC_ICSR_INITF) == 0);	// Wait until ready to update
	RTC->TR = (TR_TEMP & TEMP_TR_MASK);		// Update clock registers
	RTC->ICSR &= ~RTC_ICSR_INIT;	// Disable RTC init mode
}




void setDate(uint8_t t_day, uint8_t u_day, uint8_t t_month, uint8_t u_month, uint8_t t_year, uint8_t u_year) {
	if(t_day > 3 || u_day > 9 || t_month > 1 || u_month > 9 || t_year>9 || u_year>9) return;	// Verify input

	uint32_t DR_TEMP = 0;	// Temporary RTC DR register

	// Load appropriate values to the temporary data register (DR)
	DR_TEMP |= (t_day<<4);
	DR_TEMP |= (u_day<<0);
	DR_TEMP |= (t_month<<12);
	DR_TEMP |= (u_month<<8);
	DR_TEMP |= (t_year<<20);
	DR_TEMP |= (u_year<<16);

	// RTC write protection keyes
	RTC->WPR = 0xca;
	RTC->WPR = 0x53;

	// Enter clock init mode
	RTC->ICSR |= RTC_ICSR_INIT;
	while ((RTC->ICSR & RTC_ICSR_INITF) == 0);	// Wait until ready to update
	RTC->DR = (DR_TEMP & TEMP_DR_MASK);		// Update clock registers
	RTC->ICSR &= ~RTC_ICSR_INIT;	// Disable RTC init mode
}

void checkIfAlarm(void) {
	if (al_enable_flag == 1 && al_hour_t == hour_t && al_hour_u == hour_u && al_min_t == minute_t && al_min_u == minute_u) {	// Ring alarm if above is true
			// Clear LCD Display (display SPACEs)
			LEDClr();
			// Clear WS2812B Display
			FillLEDArray(LED_buf, 30, 0, 0);
			SPI_SEND_WSBUF(LED_buf, sizeof(LED_buf));
			DisplayLEDStr("alarm");
			dot_enable[1]=0; dot_enable[3]=0;
			while(!button_flag[B_SET]) {
				FillLEDArray(LED_buf, 30, 0, 0);
				SPI_SEND_WSBUF(LED_buf, sizeof(LED_buf));
				buzz(250);
				FillLEDArray(LED_buf, 0, 0, 30);
				SPI_SEND_WSBUF(LED_buf, sizeof(LED_buf));
				delay_ms(250);
			}
			LEDClr();
			button_flag[B_SET]=0;
			dot_enable[1]=1; dot_enable[3]=1;
			al_enable_flag = 0;
			GPIOA->ODR &= ~GPIO_ODR_OD6;
	}
}

void checkIfStudy(void) {

	// Not functioning in the software 1.0 release

}



// *** INTERRUPT ROUTINE SECTION ***
__attribute__((interrupt)) void TIM16_IRQHandler(void)	{
	if (TIM16->SR & TIM_SR_CC1IF)	{
		TIM16->SR &= ~TIM_SR_CC1IF;	// Clear flag
		TIM16->EGR |= TIM_EGR_UG;	// Reinitialise the counter

		while((RTC->ICSR & RTC_ICSR_RSF)==0);	// See if the data was updated by the RTC
		hour_t = ((RTC->TR & RTC_TR_HT)>>20);
		hour_u = ((RTC->TR & RTC_TR_HU)>>16);
		minute_t = ((RTC->TR & RTC_TR_MNT)>>12);
		minute_u = ((RTC->TR & RTC_TR_MNU)>>8);


		year_t = ((RTC->DR & RTC_DR_YT)>>20);
		year_u = ((RTC->DR & RTC_DR_YU)>>16);
		month_t = ((RTC->DR & RTC_DR_MT)>>12);
		month_u = ((RTC->DR & RTC_DR_MU)>>8);
		day_t = ((RTC->DR & RTC_DR_DT)>>4);
		day_u = ((RTC->DR & RTC_DR_DU)>>0);

		RTC->ICSR &= ~RTC_ICSR_RSF;		// Clear the SR update bit
	}
}



