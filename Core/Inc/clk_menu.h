/*
 * clk_menu.h
 *
 *  Created on: Dec 25, 2020
 *      Author: Michal Makowka
 */

#ifndef INC_CLK_MENU_H_
#define INC_CLK_MENU_H_

#include <stdio.h>
#include <string.h>
#include "stm32g071xx.h"
#include "ws_lib.h"
#include "segment_dsp.h"
#include "config.h"



typedef struct menu {
	// Menu variables
	uint8_t menu_pos;
	uint8_t nxt_level;
	uint8_t back;

	uint8_t menu_pos_max;

	// Executive functions (main menu)
	void (*configuration)(void * wsk);
	void (*study)(void * wsk);
	void (*alarm)(void * wsk);

} TMENU;


uint8_t al_enable_flag;
uint8_t al_hour_t, al_hour_u, al_min_t, al_min_u;



// Menu options: position order
enum{p_main_config, p_main_alarm, p_main_study, p_main_back};	// Main menu
enum{p_conf_time, p_conf_back};	// Config menu
enum{p_al_time, p_al_enable, p_al_back};	// Alarm menu
enum{p_std_studytime, p_std_breaktime, p_std_enable, p_std_back};	// Study menu

// Menu options: position char (max. 6 characters)
char * main_options[4];
char * config_options[2];
char * alarm_options[3];
char * study_options[4];



void displayMenu (TMENU * str_wsk);
void displayConfig (TMENU * str_wsk);
void displayAlarm (TMENU * str_wsk);
void displayStudy (TMENU * str_wsk);

uint8_t decToBcd(uint8_t val);

#endif /* INC_CLK_MENU_H_ */
