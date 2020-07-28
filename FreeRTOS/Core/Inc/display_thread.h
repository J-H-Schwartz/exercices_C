/*
 * display_thread.h
 *
 *  Created on: Apr 17, 2020
 *      Author: jonathan
 */

#ifndef DISPLAY_THREAD_H_
#define DISPLAY_THREAD_H_
#include"game_p4.h"

#define PLAY_TOKEN_STARTING_COL 3
#define WAIT_ANIM_COLORS_NUMBER 3

#define DISPLAY_ROWS 7
#define DISPLAY_COLS 7

#define BLINK_DELAY_PLAY_TOKEN 250 //µseconds
#define BLINK_COUNT_PLAY_TOKEN 1
#define BLINK_DELAY_VICTORY_TOKENS 500 //µseconds
#define BLINK_COUNT_VICTORY_TOKENS 15
#define BLINK_DELAY_VICTORY_W 500 //µseconds
#define BLINK_COUNT_VICTORY_W 10
#define W_BLINK_TABLE_SIZE 23
#define VICTORY_BLINK_TABLE_SIZE 9
#define PLAY_TOKEN_BLINK_TABLE_SIZE 3
#define BLINK_TABLE_STOP_INT -1

#define IDLE_DELAY 500 //µseconds

#define WAIT_COLOR_1 BLUE_COLOR
#define WAIT_COLOR_2 WHITE_COLOR
#define WAIT_COLOR_3 RED_COLOR

//Color bitwise mask.
#define MASK_RED 0xFF0000
#define MASK_GREEN 0xFF00
#define MASK_BLUE 0xFF

typedef struct { /*Définition du type led qui contient les valeurs des couleurs*/
	int RValue; /*Rouge, Verte, et Bleue.					  						   */
	int GValue;
	int BValue;
} led;

void wait_for_display_init(void);
void wait_for_display_reset(void);
void SetMatrixTokenColor(led* token, int color);
void SendToSimu(Msg_t* command, size_t size);
void thread_handler_display(void* args);

#endif /* DISPLAY_THREAD_H_ */
