/*
 * app_thread.c
 *
 *  Created on: Apr 17, 2020
 *      Author: jonathan
 */
#include "cmsis_os.h"

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>


#include"leds_control.h"
#include"queue.h"
#include"game_p4.h"
#include"app_thread.h"
#include"display_thread.h"
#include"read_thread.h"
#include"timer_threads.h"

static uint8_t pad_activation_status = PADS_DEACTIVATED;
extern osMessageQueueId_t app_queueHandle;
extern timer_struct_t short_timer;
extern timer_struct_t long_timer;

void TimeOutAutoplay(void* actual_player_pad) {
	uint8_t* tmp_pad = (uint8_t*) actual_player_pad;
	Msg_t tmp_message;
	tmp_message.type = TDM_TIMER;
	tmp_message.pad = *tmp_pad;
	tmp_message.move_command = DOWN_COMMAND;
	osMessageQueuePut(app_queueHandle, &tmp_message, 0U, 0U);
}

void SendInputToApp(void* command_table) {
	if (pad_activation_status == PADS_ACTIVATED) {
		uint8_t* command;
		command = command_table;
		Msg_t tmp_message;
		tmp_message.pad = command[1];
		tmp_message.move_command = (uint8_t) command[0];
		osMessageQueuePut(app_queueHandle, &tmp_message, 0U, 0U);
	}
}

void *thread_handler_app(void* args) {
	//Game initializations.
	victory_infos_t victory_infos;
	move_t move;
	Msg_t tmp_message;
	Msg_t tmp_command;
	uint8_t actual_player_pad;
	if (registerCallback(SendInputToApp) != 0) {
		exit(EXIT_FAILURE);
	}
	short_timer.Timer_Callback = TimeOutAutoplay;
	short_timer.callback_param = &actual_player_pad;
	long_timer.Timer_Callback = TimeOutAutoplay;
	long_timer.callback_param = &actual_player_pad;
	while (1) {
		gp4_init();
		gp4_initialize_victory_struct(&victory_infos);
		gp4_initialize_move_struct(&move);
		gp4_initialize_message_struct(&tmp_command);
		gp4_initialize_message_struct(&tmp_message);
		actual_player_pad = NO_PLAYER_PAD;
		while (1) {
			pad_activation_status = PADS_ACTIVATED;
			if (actual_player_pad != 0) {
				start_new_timer(&short_timer, 0);
				start_new_timer(&long_timer, 0);
			}
			//Wait message from reading queue.
			osMessageQueueGet(app_queueHandle, &tmp_command, 0U, osWaitForever);
			pad_activation_status = PADS_DEACTIVATED;
			// Game start player pad attribution if needed, else it sends message into command processing.
			if (actual_player_pad == 0) {
				if (tmp_command.pad == PAD_1) {
					actual_player_pad = PLAYER_PAD_1;
				} else if (tmp_command.pad == PAD_2) {
					actual_player_pad = PLAYER_PAD_2;
				}
				tmp_message.mode = COUNTDOWN;
				SendToDisplay(&tmp_message, sizeof(tmp_message));
				wait_for_display_init();
				continue;
			}
			//Process message from reading queue.

			//If DOWN button is pushed.
			if (tmp_command.move_command == DOWN_COMMAND
					&& tmp_command.pad == actual_player_pad) {
				move = gp4_play_token();
				tmp_message.mode = CONNECT_4;
				tmp_message.type = TDM_DISPLAY;
				tmp_message.status = PLAY;
				tmp_message.move_coordinates.player = move.player;
				tmp_message.move_coordinates.player_color = move.player_color;
				tmp_message.move_coordinates.beg.l = move.beg.l;
				tmp_message.move_coordinates.beg.c = move.beg.c;
				tmp_message.move_coordinates.end.l = move.end.l;
				tmp_message.move_coordinates.end.c = move.end.c;
				tmp_message.move_command = DOWN_COMMAND;
				stop_new_timer(&short_timer);
				stop_new_timer(&long_timer);
				SendToDisplay(&tmp_message, sizeof(tmp_message));
				victory_infos = gp4_check_winner();
				if (victory_infos.game_end != LIVE) {
					break;
				}
				move = gp4_next_player();
				tmp_message.mode = CONNECT_4;
				tmp_message.type = TDM_DISPLAY;
				tmp_message.status = PLAY;
				tmp_message.move_coordinates.player = move.player;
				tmp_message.move_coordinates.player_color = move.player_color;
				tmp_message.move_coordinates.beg.l = move.beg.l;
				tmp_message.move_coordinates.beg.c = move.beg.c;
				tmp_message.move_coordinates.end.l = move.end.l;
				tmp_message.move_coordinates.end.c = move.end.c;
				tmp_message.move_command = NEXT_PLAYER_COMMAND;
				SendToDisplay(&tmp_message, sizeof(tmp_message));
				if (actual_player_pad == PLAYER_PAD_1)
					actual_player_pad = PLAYER_PAD_2;
				else
					actual_player_pad = PLAYER_PAD_1;
				continue;
			} else if (tmp_command.move_command == LEFT_COMMAND
					&& tmp_command.pad == actual_player_pad) {
				move = gp4_move_left();
				tmp_message.mode = CONNECT_4;
				tmp_message.type = TDM_DISPLAY;
				tmp_message.status = PLAY;
				tmp_message.move_coordinates.player = move.player;
				tmp_message.move_coordinates.player_color = move.player_color;
				tmp_message.move_coordinates.beg.l = move.beg.l;
				tmp_message.move_coordinates.beg.c = move.beg.c;
				tmp_message.move_coordinates.end.l = move.end.l;
				tmp_message.move_coordinates.end.c = move.end.c;
				tmp_message.move_command = LEFT_COMMAND;
				stop_new_timer(&short_timer);
				SendToDisplay(&tmp_message, sizeof(tmp_message));
				continue;
				//If RIGHT button is pushed.
			} else if (tmp_command.move_command == RIGHT_COMMAND
					&& tmp_command.pad == actual_player_pad) {
				move = gp4_move_right();
				tmp_message.mode = CONNECT_4;
				tmp_message.type = TDM_DISPLAY;
				tmp_message.status = PLAY;
				tmp_message.move_coordinates.player = move.player;
				tmp_message.move_coordinates.player_color = move.player_color;
				tmp_message.move_coordinates.beg.l = move.beg.l;
				tmp_message.move_coordinates.beg.c = move.beg.c;
				tmp_message.move_coordinates.end.l = move.end.l;
				tmp_message.move_coordinates.end.c = move.end.c;
				tmp_message.move_command = RIGHT_COMMAND;
				stop_new_timer(&short_timer);
				SendToDisplay(&tmp_message, sizeof(tmp_message));
				continue;
			}

		}
		tmp_message.mode = CONNECT_4;
		tmp_message.type = TDM_DISPLAY;
		tmp_message.status = GAME_END;
		//Game end processing processing
		if (victory_infos.game_end == 1) {
			//Victory Blink coordinates processing.
			if (victory_infos.victory_type == HORIZONTAL) {
				tmp_message.victory_type = LINE_VICTORY;
			} else if (victory_infos.victory_type == VERTICAL) {
				tmp_message.victory_type = COL_VICTORY;
			} else if (victory_infos.victory_type == RIGHT_DIAGONAL) {
				tmp_message.victory_type = RIGHT_DIAG_VICTORY;
			} else if (victory_infos.victory_type == LEFT_DIAGONAL) {
				tmp_message.victory_type = LEFT_DIAG_VICTORY;
			}
			tmp_message.move_coordinates.player_color = move.player_color;
			tmp_message.move_coordinates.beg.l =
					victory_infos.victory_line_tokens_coord[0][0];
			tmp_message.move_coordinates.beg.c =
					victory_infos.victory_line_tokens_coord[0][1];
			tmp_message.move_coordinates.end.l =
					victory_infos.victory_line_tokens_coord[3][0];
			tmp_message.move_coordinates.end.c =
					victory_infos.victory_line_tokens_coord[3][1];
			SendToDisplay(&tmp_message, sizeof(tmp_message));
		} else if (victory_infos.game_end == DRAW) {
			// Draw end processing.
			tmp_message.victory_type = DRAW_END;
			SendToDisplay(&tmp_message, sizeof(tmp_message));
		}
		wait_for_display_reset();
	}
	osThreadExit();
	return 0;
}
