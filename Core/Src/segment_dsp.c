/*
 * segment_dsp.c
 *
 *  Created on: Dec 21, 2020
 *      Author: Michal Makowka
 */
#include "segment_dsp.h"


#define D0_SET GPIOC->BSRR = GPIO_BSRR_BR6
#define D1_SET GPIOC->BSRR = GPIO_BSRR_BR7
#define D2_SET GPIOD->BSRR = GPIO_BSRR_BR0
#define D3_SET GPIOD->BSRR = GPIO_BSRR_BR1
#define D4_SET GPIOD->BSRR = GPIO_BSRR_BR2
#define D5_SET GPIOD->BSRR = GPIO_BSRR_BR3

#define D0_CLR GPIOC->BSRR = GPIO_BSRR_BS6
#define D1_CLR GPIOC->BSRR = GPIO_BSRR_BS7
#define D2_CLR GPIOD->BSRR = GPIO_BSRR_BS0
#define D3_CLR GPIOD->BSRR = GPIO_BSRR_BS1
#define D4_CLR GPIOD->BSRR = GPIO_BSRR_BS2
#define D5_CLR GPIOD->BSRR = GPIO_BSRR_BS3



// *** Symbol values representation to be written to the GPIOB ODR register ***
const uint16_t ZERO = 32704;
const uint16_t ONE = 32761;
const uint16_t TWO = 28388;
const uint16_t THREE = 28400;
const uint16_t FOUR = 28377;
const uint16_t FIVE = 28370;
const uint16_t SIX = 28354;
const uint16_t SEVEN = 32760;
const uint16_t EIGHT = 28352;
const uint16_t NINE = 28368;

const uint16_t SPACE = 32767;

const uint16_t A = 28360;
const uint16_t B = 31408;
const uint16_t C = 32710;
const uint16_t D = 31664;
const uint16_t E = 28358;
const uint16_t F = 28366;
const uint16_t G = 32450;
const uint16_t H = 28361;
const uint16_t I = 31679;
const uint16_t J = 32737;
const uint16_t K = 27983;
const uint16_t L = 32711;
const uint16_t M = 16201;
const uint16_t N = 15817;
const uint16_t O = 32704;
const uint16_t P = 28364;
const uint16_t Q = 32192;
const uint16_t R = 27852;
const uint16_t S = 28370;
const uint16_t T = 31678;
const uint16_t U = 32705;
const uint16_t W = 30153;
const uint16_t V = 30543;
const uint16_t X = 13695;
const uint16_t Y = 15231;
const uint16_t Z = 63350;

// ******** ******** ******** ********

// Select for which DOT should be enabled
volatile uint8_t dot_enable[6] = {OFF, OFF, OFF, OFF, OFF, OFF};


void DisplayNumberSet(uint8_t n0, uint8_t n1, uint8_t n2, uint8_t n3, uint8_t n4, uint8_t n5) {
	displayChar((n0+48), 0);
	displayChar((n1+48), 1);
	displayChar((n2+48), 2);
	displayChar((n3+48), 3);
	displayChar((n4+48), 4);
	displayChar((n5+48), 5);
	// 48 must be added to reach the number char value from the ASCII table
}

void DisplayLEDStr(char * str) {
	uint8_t i;
	LEDClr();
	for (i=0; i<6; i++) {
		displayChar(str[i], i);
	}
}

void ScrollLed(char * data, uint16_t speed, uint8_t len) {
	uint8_t j=0;
	uint8_t k=0;
	char temp_buf[len+12];
	memset(temp_buf, 32, (len+12));
	for (j=6; j<(5+len); j++) {
		temp_buf[j] = data[k];
		k++;
	}
	k=0;
	for (j=0; j<=(6+len); j++) {
		displayChar(temp_buf[k+j], 0); k++;
		displayChar(temp_buf[k+j], 1); k++;
		displayChar(temp_buf[k+j], 2); k++;
		displayChar(temp_buf[k+j], 3); k++;
		displayChar(temp_buf[k+j], 4); k++;
		displayChar(temp_buf[k+j], 5); k=0;
		delay_ms(speed);
	}
}

void displayChar(char c, uint8_t pos) {
	if (pos<0 && pos>5) return;
	if ((c>47 && c<58) || (c>96 && c<123) || (c==32)) {
		if (c=='0') {led_dsp[pos] = ZERO; return;}
		if (c=='1') {led_dsp[pos] = ONE; return;}
		if (c=='2') {led_dsp[pos] = TWO; return;}
		if (c=='3') {led_dsp[pos] = THREE; return;}
		if (c=='4') {led_dsp[pos] = FOUR; return;}
		if (c=='5') {led_dsp[pos] = FIVE; return;}
		if (c=='6') {led_dsp[pos] = SIX; return;}
		if (c=='7') {led_dsp[pos] = SEVEN; return;}
		if (c=='8') {led_dsp[pos] = EIGHT; return;}
		if (c=='9') {led_dsp[pos] = NINE; return;}

		if (c==' ') {led_dsp[pos] = SPACE; return;}

		if (c=='a') {led_dsp[pos] = A; return;}
		if (c=='b') {led_dsp[pos] = B; return;}
		if (c=='c') {led_dsp[pos] = C; return;}
		if (c=='d') {led_dsp[pos] = D; return;}
		if (c=='e') {led_dsp[pos] = E; return;}
		if (c=='f') {led_dsp[pos] = F; return;}
		if (c=='g') {led_dsp[pos] = G; return;}
		if (c=='h') {led_dsp[pos] = H; return;}
		if (c=='i') {led_dsp[pos] = I; return;}
		if (c=='j') {led_dsp[pos] = J; return;}
		if (c=='k') {led_dsp[pos] = K; return;}
		if (c=='l') {led_dsp[pos] = L; return;}
		if (c=='m') {led_dsp[pos] = M; return;}
		if (c=='n') {led_dsp[pos] = N; return;}
		if (c=='o') {led_dsp[pos] = O; return;}
		if (c=='p') {led_dsp[pos] = P; return;}
		if (c=='q') {led_dsp[pos] = Q; return;}
		if (c=='r') {led_dsp[pos] = R; return;}
		if (c=='s') {led_dsp[pos] = S; return;}
		if (c=='t') {led_dsp[pos] = T; return;}
		if (c=='u') {led_dsp[pos] = U; return;}
		if (c=='w') {led_dsp[pos] = W; return;}
		if (c=='v') {led_dsp[pos] = V; return;}
		if (c=='x') {led_dsp[pos] = X; return;}
		if (c=='y') {led_dsp[pos] = Y; return;}
		if (c=='z') {led_dsp[pos] = Z; return;}
	}
}


void setDigit(uint8_t digit) {
	switch(digit) {
		case 0:
			D0_SET;
			D1_CLR;
			D2_CLR;
			D3_CLR;
			D4_CLR;
			D5_CLR;
			break;
		case 1:
			D0_CLR;
			D1_SET;
			D2_CLR;
			D3_CLR;
			D4_CLR;
			D5_CLR;
			break;
		case 2:
			D0_CLR;
			D1_CLR;
			D2_SET;
			D3_CLR;
			D4_CLR;
			D5_CLR;
			break;
		case 3:
			D0_CLR;
			D1_CLR;
			D2_CLR;
			D3_SET;
			D4_CLR;
			D5_CLR;
			break;
		case 4:
			D0_CLR;
			D1_CLR;
			D2_CLR;
			D3_CLR;
			D4_SET;
			D5_CLR;
			break;
		case 5:
			D0_CLR;
			D1_CLR;
			D2_CLR;
			D3_CLR;
			D4_CLR;
			D5_SET;
			break;
	}
}

void LEDClr(void) {
	displayChar(' ', 0);
	displayChar(' ', 1);
	displayChar(' ', 2);
	displayChar(' ', 3);
	displayChar(' ', 4);
	displayChar(' ', 5);
}

// *** INTERRUPT ROUTINE SECTION ***
// LED display multiplexing
__attribute__((interrupt)) void TIM15_IRQHandler(void)	{
	if (TIM15->SR & TIM_SR_CC1IF)	{
		TIM15->SR &= ~TIM_SR_CC1IF;	// Clear flag
			static uint8_t pos = 0;
			if (pos>5) pos = 0;
			if (dot_enable[pos] == 1) led_dsp[pos] -= 8192;		// Subtract (ENABLE) the bit value responsible for DOT display (note: active low)
			GPIOB->ODR = ((0x7FFF) & led_dsp[pos]);		// 0x7FFF masks the button value on PB15
			if (dot_enable[pos] == 1) led_dsp[pos] += 8192;		// Add (DISABLE) the bit value responsible for DOT display (note: active low)
			setDigit(pos);
			pos++;

	}
}
