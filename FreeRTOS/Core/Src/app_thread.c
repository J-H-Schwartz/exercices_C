/*
 * app_thread.c
 *
 *  Created on: Apr 17, 2020
 *      Author: jonathan
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include "cmsis_os.h"
#include"game_p4.h"
#include"app_thread.h"
#include"display_thread.h"
#include"read_thread.h"
#include"timer_threads.h"

static uint8_t pad_activation_status = PADS_DEACTIVATED;

osMessageQueueId_t receiveFromInputHandle;
const osMessageQueueAttr_t receiveFromInput_attributes = { .name =
		"receiveFromInput" };

void TimeOutAutoplay(void* actual_player_pad) {
	uint8_t* tmp_pad = (uint8_t*) actual_player_pad;
	Msg_t tmp_message;
	tmp_message.pad = *tmp_pad;
	tmp_message.move_command = DOWN_COMMAND;
	osMessageQueuePut(receiveFromInputHandle, &tmp_message, 1, osWaitForever);
}

void SendInputToApp(void* command_table) {
	if (pad_activation_status == PADS_ACTIVATED) {
		uint8_t* command;
		command = command_table;
		Msg_t tmp_message;
		tmp_message.type = TDM_TIMER;
		tmp_message.pad = command[1];
		tmp_message.move_command = (uint8_t) command[0];
		osMessageQueuePut(receiveFromInputHandle, &tmp_message, 1, osWaitForever);
	}
}

void thread_handler_app(void* args) {
	//Game initializations.
	receiveFromInputHandle = osMessageQueueNew(16, sizeof(Msg_t),
				&receiveFromInput_attributes);
	victory_infos_t victory_infos;
	move_t move;
	Msg_t tmp_message;
	Msg_t tmp_command;
	uint8_t actual_player_pad;
	Timer_Struct short_timer;
	Timer_Struct long_timer;
	timer_init(&short_timer, SHORT_TIMER_SETTING, &actual_player_pad);
	timer_init(&long_timer, LONG_TIMER_SETTING, &actual_player_pad);
	if (registerCallback(SendInputToApp) != 0) {
		//error_printf("Input callback register failed.");
		exit(EXIT_FAILURE);
	}
	short_timer.Timer_Callback = TimeOutAutoplay;
	long_timer.Timer_Callback = TimeOutAutoplay;
	while (1) {
		gp4_init();
		gp4_initialize_victory_struct(&victory_infos);
		gp4_initialize_move_struct(&move);
		gp4_initialize_message_struct(&tmp_command);
		gp4_initialize_message_struct(&tmp_message);
		actual_player_pad = NO_PLAYER_PAD;
		//Console Display
		//gp4_display();
		while (1) {
			pad_activation_status = PADS_ACTIVATED;
			//debug_printf(2, "Actual player pad : %d\n", actual_player_pad);
			if (actual_player_pad != 0) {
				start_timer(&short_timer);
				start_timer(&long_timer);
			}
			//Wait message from reading queue.
			osMessageQueueGet(receiveFromInputHandle, &tmp_command, NULL, osWaitForever);
			//debug_printf(2, "App handler received: %d %d\n", tmp_command.pad,
			//		tmp_command.move_command);
			pad_activation_status = PADS_DEACTIVATED;
			// Game start player pad attribution.
			if (actual_player_pad == 0) {
				if (tmp_command.pad == PAD_1) {
					actual_player_pad = PLAYER_PAD_1;
				} else if (tmp_command.pad == PAD_2) {
					actual_player_pad = PLAYER_PAD_2;
				}
				tmp_message.mode = COUNTDOWN;
				SendToSimu(&tmp_message, sizeof(tmp_message));
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
				stop_timer(&short_timer);
				stop_timer(&long_timer);
				//gp4_display();
				SendToSimu(&tmp_message, sizeof(tmp_message));
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
				//gp4_display();
				SendToSimu(&tmp_message, sizeof(tmp_message));
				if (actual_player_pad == PLAYER_PAD_1)
					actual_player_pad = PLAYER_PAD_2;
				else
					actual_player_pad = PLAYER_PAD_1;
				continue;
				//If LEFT button is pushed.
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
				stop_timer(&short_timer);
				//gp4_display();
				SendToSimu(&tmp_message, sizeof(tmp_message));
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
				stop_timer(&short_timer);
				//gp4_display();
				SendToSimu(&tmp_message, sizeof(tmp_message));
				continue;
			}

		}
		//gp4_display();
		tmp_message.mode = CONNECT_4;
		tmp_message.type = TDM_DISPLAY;
		tmp_message.status = GAME_END;
		//Victory processing
		if (victory_infos.game_end == 1) {
//			debug_printf(1, "Game end : %d\nWinner: %d\nVictory Type : %d\n",
//					victory_infos.game_end, victory_infos.winner_player,
//					victory_infos.victory_type);
//			debug_printf(1,
//					"Coordonnées du jeton de victoire: Ligne %d Colonne %d\n",
//					victory_infos.victory_token_coord[0] + 1,
//					victory_infos.victory_token_coord[1] + 1);
//			debug_printf(1,
//					"Alignement victorieux:\nJeton 1: Ligne %d Colonne %d\nJeton 2: Ligne %d Colonne %d\nJeton 3: Ligne" " %d Colonne %d\nJeton 4: Ligne %d Colonne %d\n",
//					victory_infos.victory_line_tokens_coord[0][0] + 1,
//					victory_infos.victory_line_tokens_coord[0][1] + 1,
//					victory_infos.victory_line_tokens_coord[1][0] + 1,
//					victory_infos.victory_line_tokens_coord[1][1] + 1,
//					victory_infos.victory_line_tokens_coord[2][0] + 1,
//					victory_infos.victory_line_tokens_coord[2][1] + 1,
//					victory_infos.victory_line_tokens_coord[3][0] + 1,
//					victory_infos.victory_line_tokens_coord[3][1] + 1);
//			debug_printf(1, "\n");
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
			SendToSimu(&tmp_message, sizeof(tmp_message));
		} else if (victory_infos.game_end == DRAW) {
			//debug_printf(1, "Draw. Game Over.\n");
			tmp_message.victory_type = DRAW_END;
			SendToSimu(&tmp_message, sizeof(tmp_message));
		}
		wait_for_display_reset();
	}
	return;
}
