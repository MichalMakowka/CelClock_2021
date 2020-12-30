/*
 * config.c
 *
 *  Created on: Dec 15, 2020
 *      Author: Michal Makowka
 */

#include "config.h"
#include "ws_lib.h"





void SystemCFG (void)	{

	// *** Configure System Clock (48MHz for each system BUS) ***
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN_4;	// Clear (as it's 1 by default)
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_2 | RCC_PLLCFGR_PLLN_3;	// Set PLLN Mult. for 12 (0000 1100)
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLR_0;	// Set PLLR Div. for 2 (001)
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_0 | RCC_PLLCFGR_PLLSRC_1;	// Fpllin = HSE
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;	// Enable output of the PLL
	RCC->CFGR |= RCC_CFGR_SW_1;	// PLLRCLK for SYSCLK
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLPEN;
	FLASH->ACR |= FLASH_ACR_LATENCY_1;
	RCC->CR |= RCC_CR_HSEON;	// HSE ON
	while (!(RCC->CR & RCC_CR_HSERDY)); // Wait for HSE ON
	RCC->CR |= RCC_CR_PLLON;	// PLL ON
	while ((RCC->CR & RCC_CR_PLLRDY)); // Check if PLL not locked
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);	// Check for a correct source set
	RCC->CR &= ~RCC_CR_HSION;	// 16MHz HSI OFF
	// *********************************


	// *** Configure GPIO: Techled, LED Display, BUZZ, BATTERY CONTROL ***
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOBEN | RCC_IOPENR_GPIOCEN | RCC_IOPENR_GPIODEN | RCC_IOPENR_GPIOFEN;	// GPIO: ABCDF

	GPIOB->MODER = GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0 | GPIO_MODER_MODE2_0 | GPIO_MODER_MODE3_0 |
					GPIO_MODER_MODE4_0 | GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0 | GPIO_MODER_MODE7_0 |
					GPIO_MODER_MODE8_0 | GPIO_MODER_MODE9_0 | GPIO_MODER_MODE10_0 | GPIO_MODER_MODE11_0 |
					GPIO_MODER_MODE12_0 | GPIO_MODER_MODE13_0 | GPIO_MODER_MODE14_0;	// PB0-PB14: Output

	// LED Cathodes
	GPIOC->ODR |= (GPIO_ODR_OD6 | GPIO_ODR_OD7);
	GPIOC->MODER &= ~(GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1);	// PC6-PC7: Output
	GPIOD->ODR |= (GPIO_ODR_OD0 | GPIO_ODR_OD1 | GPIO_ODR_OD2 | GPIO_ODR_OD3);
	GPIOD->MODER &= ~(GPIO_MODER_MODE0_1 | GPIO_MODER_MODE1_1 | GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1);	// PD0-PD3: Output
	// Techled, BUZZ
	GPIOA->MODER &= ~(GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1);	// PA6-PA7: Output
	// *********************************



	// *** Configure TIM14 for delay subroutine ***
	NVIC_EnableIRQ(TIM14_IRQn);
	RCC->APBENR2 |= RCC_APBENR2_TIM14EN;	// Enable TIM14 clock
	TIM14->CCMR1 |= TIM_CCMR1_OC2M_0;	// Set channel 1 to active level on match
	TIM14->DIER |= TIM_DIER_CC1IE;
	TIM14->PSC = 47999;	/// 1000 Hz	(psc = 47999), F_CPU = 48MHz
	// *********************************


	// *** Configure TIM15 for Segment Multiplexing ***
	NVIC_EnableIRQ(TIM15_IRQn);
	RCC->APBENR2 |= RCC_APBENR2_TIM15EN;	// Enable TIM15 clock
	TIM15->CCMR1 |= TIM_CCMR1_OC2M_0;	// Set channel 1 to active level on match
	TIM15->DIER |= TIM_DIER_CC1IE;
	TIM15->PSC = 1;	/// 1000 Hz	(psc = 47999), F_CPU = 48MHz
	TIM15->CR1 |= TIM_CR1_CEN;	// Counter enable
	// *********************************


	// *** Configure RTC Clock ***
	RCC->APBENR1 |= RCC_APBENR1_RTCAPBEN | RCC_APBENR1_PWREN;
	PWR->CR1 |= PWR_CR1_DBP;	// Disable RTC write protection
	RCC->BDCR |= RCC_BDCR_LSEON;	// LSE eabled
	while(!(RCC->BDCR & RCC_BDCR_LSERDY));	// Wait for LSE ready
	RCC->BDCR |= RCC_BDCR_RTCSEL_0 | RCC_BDCR_RTCEN;	// LSE for the RTC, RTC enable


	// RTC CALIBRATION PROCESS
	// Update RTC_PRER register
	uint32_t tmp_prer = RTC->PRER;
	tmp_prer &= ~(0x7f7fff);		// Set zeros to the tmp_pres Prediv_A and Prediv_S register
	tmp_prer |= (PREDIV_A<<16);		// SET PREDIV_A [val: 0-127]
	tmp_prer |= PREDIV_S;		// SET PREDIV_S [val: 0-32767]
	RTC->WPR = 0xca;
	RTC->WPR = 0x53;	// Write protection disabled
	// Enter clock init mode
	RTC->ICSR |= RTC_ICSR_INIT;
	while ((RTC->ICSR & RTC_ICSR_INITF) == 0);	// Wait until ready to update
	RTC->PRER = tmp_prer;		// Update PRER register
	RTC->ICSR &= ~RTC_ICSR_INIT;	// Disable RTC init mode

	// Update RTC_CALR register
	uint32_t tmp_calr = RTC->CALR;
	tmp_calr &= ~(0x81ff);	// CALP and CALM mask (setting to zero)
	tmp_calr |= (CALP<<15);	// SET CALP [val: 0-1]
	tmp_calr |= (CALM);		// SET CALM [val: 0-511]
	RTC->WPR = 0xca;
	RTC->WPR = 0x53;	// Write protection disabled
	RTC->CALR = tmp_calr;	// Update CALR resgister

	#if (RTC_OSC_CALIB_OUTPUT_EN)
	// Map the RTC clock on the PA4 pin (RTC_OUT2)
	RTC->CR |= RTC_CR_COE;	// Calibration output enabled
	RTC->CR |= RTC_CR_COSEL;	// F_calibration = 1Hz
	GPIOA->MODER &= ~GPIO_MODER_MODE4_1;
	RTC->CR |= RTC_CR_OUT2EN;	// RTC clock available on PA4 - RTC_OUT2
	#endif
	#if (!RTC_OSC_CALIB_OUTPUT_EN)
	RTC->CR &= ~RTC_CR_COE;	// Calibration output disbled
	RTC->CR &= ~RTC_CR_COSEL;	// F_calibration 1Hz disabled
	GPIOA->MODER |= GPIO_MODER_MODE4_1;
	RTC->CR &= ~RTC_CR_OUT2EN;	// RTC clock disabled on PA4 - RTC_OUT2
	#endif
	// END OF RTC CALIBRATION PROCESS
	// *********************************


	// *** Configure TIM16 for RTC clock data update ***
	NVIC_EnableIRQ(TIM16_IRQn);
	RCC->APBENR2 |= RCC_APBENR2_TIM16EN;	// Enable TIM16 clock
	TIM16->PSC = 47999;
	TIM16->CCR1 = 250;
	TIM16->DIER |= TIM_DIER_CC1IE;	// CaptureCompare1 Interrupt Enable	(2Hz)
	TIM16->CR1 |= TIM_CR1_CEN;	// Counter enable
	// *********************************


	// *** Configure SPI ***
	NVIC_EnableIRQ(SPI1_IRQn);
	RCC->APBENR2 |= RCC_APBENR2_SPI1EN;
	GPIOA->MODER &= ~GPIO_MODER_MODE2_0;	// MODER: 10 - Alternative function
	// GPIOA->AFR[0] 	// AFDEL set to 0000 automatically (SPI1_MOSI)		LEAVE THE COMMENT
	SPI1->CR1 |= SPI_CR1_BR_1;	// SPI psc (010) : (8), 48/8 = 6Mhz set
	SPI1->CR1 |= (SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI);	// Set as Master (also, enable software slave management)
	SPI1->CR1 |= SPI_CR1_CPHA;	// The second clock transition is the first data capture edge (as a result, first LED will always be active)
	// 8-bit data length set initially by the hardware
	SPI1->CR1 |= SPI_CR1_SPE;	// SPI enable
	// *********************************


	// *** Configure ADC ***
	NVIC_EnableIRQ(ADC1_COMP_IRQn);
	RCC->APBENR2 |= RCC_APBENR2_ADCEN;
	ADC1->IER |= ADC_IER_EOCIE;		// End of conversion interrupt enable
	ADC1->CR |= ADC_CR_ADVREGEN;	// ADC voltage regulator enable
	ADC1->CFGR1 |= ADC_CFGR1_DISCEN;	// ADC discontinuous mode enabled
	ADC1->CFGR1 |= ADC_CFGR1_OVRMOD;	// Overwire with the last conversion result
	ADC1->CHSELR |= ADC_CHSELR_CHSEL5;	// Channel 5 selected for conversion

	ADC1->CR |= ADC_CR_ADEN;	// Start ADC
	while((ADC1->ISR & ADC_ISR_ADRDY) == 0);
	ADC1->ISR |= ADC_ISR_ADRDY;		// Clear flag
	// *********************************

	// *** Configure USART1 ***
	GPIOA->MODER &= ~(GPIO_MODER_MODE9_1 | GPIO_MODER_MODE10_1);	// GPIO Output mode
	// *********************************

	// *** Configure EXTI (switches) ***
	// SW2 (SET) - PA11 | SW3 (DOWN) - PA12 | SW4 (UP) - PB15
	GPIOA->MODER &= ~(GPIO_MODER_MODE11_0 | GPIO_MODER_MODE11_1);
	GPIOA->MODER &= ~(GPIO_MODER_MODE12_0 | GPIO_MODER_MODE12_1);
	GPIOB->MODER &= ~(GPIO_MODER_MODE15_0 | GPIO_MODER_MODE15_1);
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD11_0;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD12_0;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD15_0;

	NVIC_EnableIRQ(EXTI4_15_IRQn);
	EXTI->EXTICR[2] |= ((0x00)<<24);	// Set PA11
	EXTI->EXTICR[3] |= ((0x00)<<0);		// Set PA12
	EXTI->EXTICR[3] |= ((0x01)<<24);	// Set PB15


	EXTI->FTSR1 |= EXTI_FTSR1_FT11 | EXTI_FTSR1_FT12 | EXTI_FTSR1_FT15;		// Falling edge event

	EXTI->IMR1 |= EXTI_IMR1_IM11 | EXTI_IMR1_IM12 | EXTI_IMR1_IM15;
	// *********************************
}


void delay_ms (uint16_t ms)	{
	d_flag = 0;
	TIM14->CCR1 = ms;
	TIM14->CR1 |= TIM_CR1_CEN;	// Counter enable
	while(!d_flag);
}



void buzz (uint8_t time) {
	GPIOA->ODR |= GPIO_ODR_OD7;
	delay_ms(time);
	GPIOA->ODR &= ~GPIO_ODR_OD7;
}


// *** INTERRUPT ROUTINE SECTION ***
__attribute__((interrupt)) void TIM14_IRQHandler(void)	{
	if (TIM14->SR & TIM_SR_CC1IF)	{
		TIM14->SR &= ~TIM_SR_CC1IF;	// Clear flag
		TIM14->CR1 &= ~TIM_CR1_CEN;	// Counter disabled
		TIM14->EGR |= TIM_EGR_UG;	// Reinitialise the counter
		d_flag = 1;
	}
}


__attribute__((interrupt)) void EXTI4_15_IRQHandler(void)	{
	if ((EXTI->FPR1 & EXTI_FPR1_FPIF11))	{	// SET button
		EXTI->FPR1 |= EXTI_FPR1_FPIF11;
		button_flag[B_SET] = 1;
		}

	if ((EXTI->FPR1 & EXTI_FPR1_FPIF12))	{	// DOWN button
		EXTI->FPR1 |= EXTI_FPR1_FPIF12;
		button_flag[B_DOWN] = 1;
		}

	if ((EXTI->FPR1 & EXTI_FPR1_FPIF15))	{	// UP button
		EXTI->FPR1 |= EXTI_FPR1_FPIF15;
		button_flag[B_UP] = 1;
		}

}

