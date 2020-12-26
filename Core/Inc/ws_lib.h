/*
 * ws_lib.h
 *
 *  Created on: Dec 20, 2020
 *      Author: Michal Makowka
 */

#ifndef INC_WS_LIB_H_
#define INC_WS_LIB_H_

#include <stdio.h>
#include <string.h>
#include "stm32g071xx.h"
#include "segment_dsp.h"
#include "clock.h"


#define WS_NUMBER 54	// Number of ALL WS2812B LEDs


uint8_t LED_buf[WS_NUMBER*24];	// (WS_NUMBER * 3 colors * 8 bytes/color)
//volatile uint8_t DMA_LED_buf[WS_NUMBER*24];	// (WS_NUMBER * 3 colors * 8 bytes/color)


#define SPI_zero 0b11000000	// HEX: 0xC0
#define SPI_one 0b11111000	// HEX: 0xF8
#define SPI_reset 0b00000000	// HEX: 0x00


void FillLEDArray(volatile uint8_t * array, uint8_t green, uint8_t red, uint8_t blue);
void FillLEDSegment(volatile uint8_t * array, uint8_t start_idx, uint8_t end_idx, uint8_t red, uint8_t green, uint8_t blue);
void FillLEDNumber(volatile uint8_t * array, uint8_t pos, uint8_t nbr, uint8_t red, uint8_t green, uint8_t blue);
void getBaseColour(uint8_t col_val);
void getBaseLed(uint8_t green, uint8_t red, uint8_t blue);

void SPI_SEND_WSBUF(uint8_t * buf, uint16_t size);
void SpiLed_Send(uint8_t data);

void assignColour(uint8_t ht, uint8_t hu, uint8_t * r, uint8_t * g, uint8_t * b);


void loveyou(char * msg);


#endif /* INC_WS_LIB_H_ */
