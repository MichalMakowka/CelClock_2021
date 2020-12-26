/*
 * clk_menu.c
 *
 *  Created on: Dec 25, 2020
 *      Author: Michal Makowka
 */

#include "clk_menu.h"


char * main_options[4] = {"config", "alarm", "study", "back"};
char * config_options[2] = {"time", "back"};
char * alarm_options[3] = {"config", "enable", "back"};
char * study_options[4] = {"study", "break", "enable", "back"};


typedef struct menuInput {
	uint8_t temp_val_ten;
	uint8_t temp_val_units;
	uint8_t idx;
	uint8_t idx_default_ten;
	uint8_t idx_default_units;
	uint8_t idx_max;
	uint8_t idx_min;
	char display[4];
} TINPUT;



void menuInputService (TINPUT * in_wsk) {

	// Display which variable is being configured
	DisplayLEDStr(in_wsk->display);
	displayChar(((in_wsk->idx_default_ten + 48)), 4);	// Display initial values in line with the current idx
	displayChar(((in_wsk->idx_default_units + 48)), 5);

	in_wsk->temp_val_ten = in_wsk->idx_default_ten;
	in_wsk->temp_val_units = in_wsk->idx_default_units;
	in_wsk->idx = (in_wsk->idx_default_ten * 10) + in_wsk->idx_default_units;

	while(!button_flag[B_SET]) {
		if (button_flag[B_UP]) {
			button_flag[B_UP]=0;
			(in_wsk->idx)++;
			if (in_wsk->idx > (in_wsk->idx_max)) in_wsk->idx = in_wsk->idx_min;
			in_wsk->temp_val_ten = (decToBcd(in_wsk->idx)>>4);
			in_wsk->temp_val_units = (decToBcd(in_wsk->idx) & 0x0F);
			displayChar(((in_wsk->temp_val_ten)+48), 4);
			displayChar(((in_wsk->temp_val_units)+48), 5);
		}
		if (button_flag[B_DOWN]) {
			button_flag[B_DOWN]=0;
			(in_wsk->idx)--;
			if (in_wsk->idx > 250 || in_wsk->idx < in_wsk->idx_min) in_wsk->idx = in_wsk->idx_max;
			in_wsk->temp_val_ten=(decToBcd(in_wsk->idx)>>4);
			in_wsk->temp_val_units = (decToBcd(in_wsk->idx) & 0x0F);
			displayChar(((in_wsk->temp_val_ten)+48), 4);
			displayChar(((in_wsk->temp_val_units)+48), 5);
		}

		delay_ms(250);
	}
	button_flag[B_SET]=0;
}




void selectTime(void) {

	uint8_t temp_t_hour, temp_u_hour, temp_t_minute, temp_u_minute,
	temp_t_day, temp_u_day, temp_t_month, temp_u_month, temp_t_year, temp_u_year;


	TINPUT input;

	// Input hour
	input.display[0]='h'; input.display[1]='r';
	input.idx_max=23;
	input.idx_min=0;
	input.idx_default_ten = 0;
	input.idx_default_units = 0;
	menuInputService(&input);
	temp_t_hour = input.temp_val_ten;
	temp_u_hour = input.temp_val_units;

	// Input minute
	input.display[0]='m'; input.display[1]='i'; input.display[2]='n';
	input.idx_max=59;
	input.idx_min=0;
	input.idx_default_ten = 0;
	input.idx_default_units = 0;
	menuInputService(&input);
	temp_t_minute = input.temp_val_ten;
	temp_u_minute = input.temp_val_units;

	setTime(temp_t_hour, temp_u_hour, temp_t_minute, temp_u_minute);

	// Input day
	input.display[0]='d'; input.display[1]='a'; input.display[2]='y';
	input.idx_max=31;
	input.idx_min=1;
	input.idx_default_ten = 0;
	input.idx_default_units = 1;
	menuInputService(&input);
	temp_t_day = input.temp_val_ten;
	temp_u_day = input.temp_val_units;

	// Input month
	input.display[0]='m'; input.display[1]='o'; input.display[2]='n';
	input.idx_max=12;
	input.idx_min=1;
	input.idx_default_ten = 0;
	input.idx_default_units = 1;
	menuInputService(&input);
	temp_t_month = input.temp_val_ten;
	temp_u_month = input.temp_val_units;

	// Input year
	input.display[0]='y'; input.display[1]='e'; input.display[2]='a'; input.display[3]='r';
	input.idx_max=99;
	input.idx_min=0;
	input.idx_default_ten = 1;
	input.idx_default_units = 8;
	menuInputService(&input);
	temp_t_year = input.temp_val_ten;
	temp_u_year = input.temp_val_units;

	setDate(temp_t_day, temp_u_day, temp_t_month, temp_u_month, temp_t_year, temp_u_year);

}




void displayMenu (TMENU * str_wsk) {
	LEDClr();

	FillLEDArray(LED_buf, 0, 0, 0);
	SPI_SEND_WSBUF(LED_buf, sizeof(LED_buf));

	button_flag[B_UP]=0;
	button_flag[B_DOWN]=0;
	button_flag[B_SET]=0;

	str_wsk->nxt_level = 0;
	str_wsk->menu_pos = 0;
	str_wsk->nxt_level = 0;
	str_wsk->back = 0;

	uint8_t temp_pos;

	while(!str_wsk->back) {

	if (button_flag[B_UP]) {
		button_flag[B_UP]=0;
		str_wsk->menu_pos++;
		if(str_wsk->menu_pos>3) str_wsk->menu_pos=0;
	}

	if (button_flag[B_DOWN]) {
		button_flag[B_DOWN]=0;
		str_wsk->menu_pos--;
		if(str_wsk->menu_pos>250) str_wsk->menu_pos=3;
	}


	if (button_flag[B_SET]) {
		button_flag[B_SET]=0;
		str_wsk->nxt_level=1;
	}

	if (temp_pos != str_wsk->menu_pos) {
		DisplayLEDStr(main_options[str_wsk->menu_pos]);
		temp_pos = str_wsk->menu_pos;
	}


		if(str_wsk->nxt_level) {
			if (str_wsk->menu_pos == p_main_back) return;
			if ((str_wsk->configuration) && (str_wsk->menu_pos == p_main_config)) str_wsk->configuration(str_wsk);
			if ((str_wsk->alarm) && (str_wsk->menu_pos == p_main_alarm)) str_wsk->alarm();
			if ((str_wsk->study) && (str_wsk->menu_pos == p_main_study)) str_wsk->study();
		}
	delay_ms(250);
	}
}


void displayConfig (TMENU * str_wsk) {
	LEDClr();

	FillLEDArray(LED_buf, 0, 0, 0);
	SPI_SEND_WSBUF(LED_buf, sizeof(LED_buf));

	DisplayLEDStr(config_options[str_wsk->menu_pos]);

	button_flag[B_UP]=0;
	button_flag[B_DOWN]=0;
	button_flag[B_SET]=0;

	str_wsk->nxt_level = 0;
	str_wsk->menu_pos = 0;
	str_wsk->nxt_level = 0;
	str_wsk->back = 0;

	uint8_t temp_pos;

	while(!str_wsk->back) {

		if (button_flag[B_UP]) {
			delay_ms(80);
			button_flag[B_UP]=0;
			str_wsk->menu_pos++;
			if(str_wsk->menu_pos>1) str_wsk->menu_pos=0;
		}

		if (button_flag[B_DOWN]) {
			delay_ms(80);
			button_flag[B_DOWN]=0;
			str_wsk->menu_pos--;
			if(str_wsk->menu_pos>250) str_wsk->menu_pos=1;
		}


		if (button_flag[B_SET]) {
			delay_ms(80);
			button_flag[B_SET]=0;
			str_wsk->nxt_level=1;
		}

		if (temp_pos != str_wsk->menu_pos) {
			DisplayLEDStr(config_options[str_wsk->menu_pos]);
			temp_pos = str_wsk->menu_pos;
		}


			if(str_wsk->nxt_level) {
				if (str_wsk->menu_pos == p_conf_back) return;
				if (str_wsk->menu_pos == p_conf_time) {selectTime(); str_wsk->back=1;}

			}
			delay_ms(250);
		}
}




void displayAlarm (TMENU * str_wsk) {

	// To be done

}

void displayStudy (TMENU * str_wsk) {

	// To be done

}


uint8_t decToBcd(uint8_t val) {
  return ((val/10*16) + (val%10));
}


