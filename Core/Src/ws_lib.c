/*
 * ws_lib.c
 *
 *  Created on: Dec 20, 2020
 *      Author: Michal Makowka
 */
#include "ws_lib.h"

// Auxiliary arrays to fill the SPI LED buffer
uint8_t b_colour[8];
uint8_t b_led[24];

volatile uint8_t brg_div = 18;

volatile uint16_t ADC_result;	// ADC result for the DEBUG purpose

// This function updates LED data array (first argument) with a number "nbr" on a given position 0 <= "pos" <= 3
void FillLEDNumber(volatile uint8_t * array, uint8_t pos, uint8_t nbr, uint8_t red, uint8_t green, uint8_t blue) {

	uint8_t shift;

	if (pos>1) shift = ((pos*13)+2);
	else shift = (pos*13);

	switch(nbr) {
		case 0:
			FillLEDSegment(array, (0+shift), (11+shift), red, green, blue);
			break;
		case 1:
			FillLEDSegment(array, (2+shift), (6+shift), red, green, blue);
			break;
		case 2:
			FillLEDSegment(array, (0+shift), (4+shift), red, green, blue);
			FillLEDSegment(array, (6+shift), (10+shift), red, green, blue);
			FillLEDSegment(array, (12+shift), (12+shift), red, green, blue);
			break;
		case 3:
			FillLEDSegment(array, (0+shift), (8+shift), red, green, blue);
			FillLEDSegment(array, (10+shift), (10+shift), red, green, blue);
			FillLEDSegment(array, (12+shift), (12+shift), red, green, blue);
			break;
		case 4:
			FillLEDSegment(array, (0+shift), (0+shift), red, green, blue);
			FillLEDSegment(array, (2+shift), (6+shift), red, green, blue);
			FillLEDSegment(array, (10+shift), (12+shift), red, green, blue);
			break;
		case 5:
			FillLEDSegment(array, (0+shift), (2+shift), red, green, blue);
			FillLEDSegment(array, (4+shift), (8+shift), red, green, blue);
			FillLEDSegment(array, (10+shift), (12+shift), red, green, blue);
			break;
		case 6:
			FillLEDSegment(array, (0+shift), (2+shift), red, green, blue);
			FillLEDSegment(array, (4+shift), (12+shift), red, green, blue);
			break;
		case 7:
			FillLEDSegment(array, (0+shift), (6+shift), red, green, blue);
			break;
		case 8:
			FillLEDSegment(array, (0+shift), (12+shift), red, green, blue);
			break;
		case 9:
			FillLEDSegment(array, (0+shift), (8+shift), red, green, blue);
			FillLEDSegment(array, (10+shift), (12+shift), red, green, blue);
			break;
		default:
			break;
	}
}



// This function updates the entire LED data buffer (first argument)
void FillLEDArray(volatile uint8_t * array, uint8_t red, uint8_t green, uint8_t blue) {		// * array is the array to be entirely filled with LED data
	uint16_t a, b, c;
	b=0;
	for (a=0; a<(WS_NUMBER*24); a++) {
		getBaseLed(green, red, blue);
		c=0;
		while(c<23) {
			array[a] = b_led[b]; a++; b++;
			c++;
		}
		array[a] = b_led[b]; b++;
		b=0;
	}
}
// This function updates LED data array (first argument) within specified indexes: start_idx and end_idx
void FillLEDSegment(volatile uint8_t * array, uint8_t start_idx, uint8_t end_idx, uint8_t red, uint8_t green, uint8_t blue) {
	uint16_t a, b, c;
	b=0;
	for (a=(start_idx*24); a<=(end_idx*24); a++) {
		getBaseLed(green, red, blue);
		c=0;
		while(c<23) {
			array[a] = b_led[b]; a++; b++;
			c++;
		}
		array[a] = b_led[b]; 	  b++;
		b=0;
	}
}

// This function updates b_led[24] array
void getBaseLed(uint8_t green, uint8_t red, uint8_t blue) {		// *bld is the array to be updated
	uint8_t k, i;

	getBaseColour(green);
	i=7;
	for (k=0; k<8; k++) {
		b_led[k] = b_colour[i];
		i--;
	}
	getBaseColour(red);
	i=7;
	for (k=8; k<16; k++) {
		b_led[k] = b_colour[i];
		i--;
	}
	getBaseColour(blue);
	i=7;
	for (k=16; k<24; k++) {
		b_led[k] = b_colour[i];
		i--;
	}
}

// This function updates b_colour[8] array (one colour out of three/led)
void getBaseColour(uint8_t col_val) {	// *bcl is the array to be updated

	uint8_t temp;
	for (uint8_t j=0; j<8; j++) {
		temp = (col_val>>j);
		if ((temp%2) == 0) b_colour[j] = SPI_zero;
		else b_colour[j] = SPI_one;
	}
}

void SPI_SEND_WSBUF(uint8_t * buf, uint16_t size) {
	for(uint16_t i = 0; i<(WS_NUMBER*24); i++) {
		SpiLed_Send(LED_buf[i]);
	}
}

void SpiLed_Send(uint8_t data) {
	while( !(SPI1->SR & SPI_SR_TXE) );
	SPI1->DR = data;
}

// Update colour variable passed depending on a time (#define in clock.h file)
void assignColour(uint8_t ht, uint8_t hu, uint8_t * r, uint8_t * g, uint8_t * b) {
	uint8_t hr = 10*(ht) + hu;	// Hour in decimal format
	*r=0; *g=0; *b=0;
	if (hr >= 6 && hr < 12) *r = (R_T1/brg_div);
	else if (hr >= 12 && hr < 18) *r = (R_T2/brg_div);
	else if (hr >= 18 && hr < 21) *r = (R_T3/brg_div);
	else if (hr >= 21 && hr < 23) *r = (R_T4/brg_div);
	else if (hr >= 23 && hr < 5) *r = (R_T5/brg_div);
	else if (hr >= 5 && hr < 6) *r = (R_T6/brg_div);

	if (hr >= 6 && hr < 12) *g = (G_T1/brg_div);
	else if (hr >= 12 && hr < 18) *g = (G_T2/brg_div);
	else if (hr >= 18 && hr < 21) *g = (G_T3/brg_div);
	else if (hr >= 21 && hr < 23) *g = (G_T4/brg_div);
	else if (hr >= 23 && hr < 5) *g = (G_T5/brg_div);
	else if (hr >= 5 && hr < 6) *g = (G_T6/brg_div);

	if (hr >= 6 && hr < 12) *b = (B_T1/brg_div);
	else if (hr >= 12 && hr < 18) *b = (B_T2/brg_div);
	else if (hr >= 18 && hr < 21) *b = (B_T3/brg_div);
	else if (hr >= 21 && hr < 23) *b = (B_T4/brg_div);
	else if (hr >= 23 || hr < 5) *b = (B_T5/brg_div);
	else if (hr >= 5 && hr < 6) *b = (B_T6/brg_div);
}

uint8_t ADCCompress (uint16_t adc) {

	if (adc > 0 && adc < 1801) return 2;		// Much light
	if (adc > 1800 && adc < 2101) return 3;
	if (adc > 2100 && adc < 2401) return 5;
	if (adc > 2400 && adc < 2701) return 7;
	if (adc > 2700 && adc < 3001) return 8;
	if (adc > 3000 && adc < 3301) return 10;
	if (adc > 3300 && adc < 3601) return 12;
	if (adc > 3600 && adc < 3901) return 14;
	if (adc > 3900 && adc < 4201) return 20;	// No light
	if (adc > 4200) return 22;

	return 16;	// If LDR malfunction, return default (16)
}


void loveyou(char * msg) {
	// Display heart
	FillLEDArray(LED_buf, 0, 0, 0);
	FillLEDSegment(LED_buf, 14, 14, 100, 0, 0);
	FillLEDSegment(LED_buf, 16, 16, 100, 0, 0);
	FillLEDSegment(LED_buf, 20, 20, 100, 0, 0);
	FillLEDSegment(LED_buf, 22, 24, 100, 0, 0);
	FillLEDSegment(LED_buf, 24, 24, 100, 0, 0);
	FillLEDSegment(LED_buf, 26, 26, 100, 0, 0);
	FillLEDSegment(LED_buf, 29, 29, 100, 0, 0);
	FillLEDSegment(LED_buf, 31, 33, 100, 0, 0);
	FillLEDSegment(LED_buf, 35, 35, 100, 0, 0);
	FillLEDSegment(LED_buf, 39, 39, 100, 0, 0);
	SPI_SEND_WSBUF(LED_buf, sizeof(LED_buf));
	// Scroll loveyou message
	if(msg != NULL) ScrollLed(msg, 400, sizeof(msg));
	ScrollLed("love you celjy", 400, sizeof("love you celjy"));
	ScrollLed("you are the best", 400, sizeof("you are the best"));

}

__attribute__((interrupt)) void ADC1_COMP_IRQHandler(void)	{
	ADC1->ISR |= ADC_ISR_EOC;	// Clear end of conversion flag
	ADC_result = ADC1->DR;
	brg_div = ADCCompress(ADC_result);

}


