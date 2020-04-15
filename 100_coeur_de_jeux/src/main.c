/*
 *****************************************************************************

 File        : main.cpp

 The MIT License (MIT)
 Copyright (c) 2019 STMicroelectronics

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

 *****************************************************************************
 */

#include<stdio.h>
#include"debug.h"
#include"game_p4.h"
#include<stdlib.h>
#include<stdint.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include"queue.h"
#include"leds_control.h"
#include<string.h>

// ASCII and OFFSETS regulators.
#define LED_PANEL_OFFSET 1
#define TOP_ROW_OFFSET 1

#define WRITE_QUEUE_BUFFER_SIZE 8
#define READ_BUTTON_BUFFER_SIZE 4
#define READ_QUEUE_BUFFER_SIZE 3

//Game pads controls
typedef enum{
	PAD_1 = 49,
	PAD_2 = 50,
	DOWN_PAD = 100,
	LEFT_PAD = 108,
	RIGHT_PAD = 114,
	UP_PAD = 117
}Pads_controls;

//Pad command Identifier
typedef enum {
	NO_PAD,
	PAD_1_COMMAND,
	PAD_2_COMMAND
}Pad_Identifier;

// Victory type command enum.
typedef enum {
	LINE_VICTORY,
	COL_VICTORY,
	RIGHT_DIAG_VICTORY,
	LEFT_DIAG_VICTORY
}Display_victory_command;

//Game mode enum.
typedef enum {
	NUMBERS,
	CONNECT_4
}Display_mode_command;

//Game Status enum.
typedef enum{
	RESET,
	PLAY,
	GAME_END
}Display_status_command;

//Game move commands enum.
typedef enum{
	DOWN_COMMAND,
	UP_COMMAND,
	LEFT_COMMAND,
	RIGHT_COMMAND,
	NEXT_PLAYER_COMMAND
}Move_command;

//Global Static Vars, inter-thread communication.
static struct Queue* read_queue;
static struct Queue* write_queue;
static pthread_t thread_id;
static pthread_t write_thread_id_set;
//static pthread_t write_thread_id_blink;
static int receive_count;
static char tmp_message_write[8];
static victory_infos_struct victory_infos;

//Defined colors table of structs.
static led colors[7] = { { COLOR_ON, COLOR_OFF, COLOR_OFF }, { COLOR_OFF,
COLOR_ON,
COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_ON }, { COLOR_ON,
COLOR_ON, COLOR_ON }, { COLOR_ON, COLOR_OFF, COLOR_ON }, { COLOR_ON,
COLOR_ON, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF } };


// Victory struct initialization.
void initialize_victory_struct(void) {
	victory_infos.game_end = 0;
	victory_infos.winner_player = 0;
	victory_infos.victory_token_coord[0] = 0;
	victory_infos.victory_token_coord[1] = 0;
	victory_infos.victory_line_tokens_coord[0][0] = 0;
	victory_infos.victory_line_tokens_coord[0][1] = 0;
	victory_infos.victory_line_tokens_coord[1][0] = 0;
	victory_infos.victory_line_tokens_coord[1][1] = 0;
	victory_infos.victory_line_tokens_coord[2][0] = 0;
	victory_infos.victory_line_tokens_coord[2][1] = 0;
	victory_infos.victory_line_tokens_coord[3][0] = 0;
	victory_infos.victory_line_tokens_coord[3][1] = 0;
	victory_infos.victory_type = 0;
}

//Send message through queues, inter-thread communication.
void SendMessage(Queue_id queue, char* message, int message_length) {

	if (queue == QUEUE_READ) {
		if (message_length <= READ_QUEUE_BUFFER_SIZE) {
			add_to_Queue(read_queue, message);
			sem_post(&read_queue->semaphore);
			printf("Message sent.\n");
		} else printf("Send message Failed ! Buffer Overflow !\n");
	} else if (queue == QUEUE_WRITE) {
		if (message_length <= WRITE_QUEUE_BUFFER_SIZE) {
			add_to_Queue(write_queue, message);
			sem_post(&write_queue->semaphore);
			printf("Message sent.\n");
		} else printf("Send message Failed ! Buffer Overflow !\n");
	} else
		printf("Send message Failed ! Queue unidentified !\n");
}

//Receive message through queues, inter-thread communication.
int ReceiveMessage(Queue_id queue, char* message, int message_length) {
	if (queue == QUEUE_WRITE) {
		if (message_length >= WRITE_QUEUE_BUFFER_SIZE) {
			//sem_wait(&write_queue->semaphore);
			int status = del_from_Queue(write_queue, message);
			printf("Message received.\n");
			return status;
		} else {
			printf("Receive message failed ! Buffer Overflow !\n");
			return -1;
		}
	} else if (queue == QUEUE_READ) {
		if (message_length >= READ_QUEUE_BUFFER_SIZE) {
			sem_wait(&read_queue->semaphore);
			del_from_Queue(read_queue, message);
			printf("Message received.\n");
			return 0;
		} else {
			printf("Receive message failed ! Buffer Overflow !\n");
			return -1;
		}
	}
	printf("Receive message failed ! Queue unidentified !\n");
	return -1;
}

//Input reading and decoding thread handler.
void *thread_handler_read(void* args) {
	char button_read[READ_BUTTON_BUFFER_SIZE];
	int error;
	char command[READ_QUEUE_BUFFER_SIZE];

	while (1) {
		//Get input.
		error = readbutton(button_read, (READ_BUTTON_BUFFER_SIZE + 1));
		if (error != 0)
			printf("%d\n", error);

		//Debug input print.
		if (button_read[3] == DOWN_PAD)
			printf("Button press read: %d %d %d %d\n", button_read[0],
					button_read[1], button_read[2], button_read[3]);

		//Process input.
		if (button_read[3] == DOWN_PAD && button_read[2] == UP_PAD) {
			if (button_read[1] == PAD_1) {
				command[0] = UP_COMMAND;
				command[1] = PAD_1_COMMAND;
			} else if (button_read[1] == PAD_2){
				command[0] = UP_COMMAND;
				command[1] = PAD_2_COMMAND;
			}
		} else if (button_read[3] == DOWN_PAD && button_read[2] == DOWN_PAD) {
			if (button_read[1] == PAD_1) {
				command[0] = DOWN_COMMAND;
				command[1] = PAD_1_COMMAND;
			} else if (button_read[1] == PAD_2){
				command[0] = DOWN_COMMAND;
				command[1] = PAD_2_COMMAND;
			}
		} else if (button_read[3] == DOWN_PAD && button_read[2] == LEFT_PAD) {
			if (button_read[1] == PAD_1) {
				command[0] = LEFT_COMMAND;
				command[1] = PAD_1_COMMAND;
			} else if (button_read[1] == PAD_2){
				command[0] = LEFT_COMMAND;
				command[1] = PAD_2_COMMAND;
			}
		} else if (button_read[3] == DOWN_PAD && button_read[2] == RIGHT_PAD) {
			if (button_read[1] == PAD_1) {
				command[0] = RIGHT_COMMAND;
				command[1] = PAD_1_COMMAND;
			} else if (button_read[1] == PAD_2){
				command[0] = RIGHT_COMMAND;
				command[1] = PAD_2_COMMAND;
			}
		} else
			continue;

		//Send processed command to read queue.
		SendMessage(QUEUE_READ, command, READ_QUEUE_BUFFER_SIZE);
	}
	pthread_detach(pthread_self());
	pthread_exit(NULL);
	return 0;
}

void *thread_handler_app(void* args) {

	//Game initializations.
	initialize_victory_struct();
	debug_setlevel(2);
	gp4_init();
	move_t move;
	move.player = 1;
	char tmp_message[WRITE_QUEUE_BUFFER_SIZE];
	char tmp_command[READ_QUEUE_BUFFER_SIZE];
	while (1) {
		//Wait message from reading queue.
		ReceiveMessage(QUEUE_READ, tmp_command, READ_QUEUE_BUFFER_SIZE);
		printf("App handler received: %d %d %d\n", tmp_command[0], tmp_command[1], tmp_command[2]);

		//Process message from reading queue.

		//If DOWN button is pushed.
		if (tmp_command[0] == DOWN_COMMAND && tmp_command[1] == move.player) {
			move = gp4_play_token();
			tmp_message[0] = CONNECT_4;
			tmp_message[1] = PLAY;
			tmp_message[2] = DOWN_COMMAND;
			tmp_message[3] = move.player_color;
			tmp_message[4] = move.beg.l;
			tmp_message[5] = move.beg.c;
			tmp_message[6] = move.end.l;
			tmp_message[7] = move.end.c;
			SendMessage(QUEUE_WRITE, tmp_message, WRITE_QUEUE_BUFFER_SIZE);
			sleep(1);
			victory_infos = gp4_check_winner();
			if (victory_infos.game_end != 0) {
				break;
			}
			move = gp4_next_player();
			tmp_message[0] = CONNECT_4;
			tmp_message[1] = PLAY;
			tmp_message[2] = NEXT_PLAYER_COMMAND;
			tmp_message[3] = move.player_color;
			tmp_message[4] = move.beg.l;
			tmp_message[5] = move.beg.c;
			tmp_message[6] = move.end.l;
			tmp_message[7] = move.end.c;
			SendMessage(QUEUE_WRITE, tmp_message, WRITE_QUEUE_BUFFER_SIZE);
			continue;
		//If LEFT button is pushed.
		} else if (tmp_command[0] == LEFT_COMMAND && tmp_command[1] == move.player) {
			move = gp4_move_left();
			tmp_message[0] = CONNECT_4;
			tmp_message[1] = PLAY;
			tmp_message[2] = LEFT_COMMAND;
			tmp_message[3] = move.player_color;
			tmp_message[4] = move.beg.l;
			tmp_message[5] = move.beg.c;
			tmp_message[6] = move.end.l;
			tmp_message[7] = move.end.c;
			SendMessage(QUEUE_WRITE, tmp_message, WRITE_QUEUE_BUFFER_SIZE);
			continue;
		//If RIGHT button is pushed.
		} else if (tmp_command[0] == RIGHT_COMMAND && tmp_command[1] == move.player) {
			move = gp4_move_right();
			tmp_message[0] = CONNECT_4;
			tmp_message[1] = PLAY;
			tmp_message[2] = RIGHT_COMMAND;
			tmp_message[3] = move.player_color;
			tmp_message[4] = move.beg.l;
			tmp_message[5] = move.beg.c;
			tmp_message[6] = move.end.l;
			tmp_message[7] = move.end.c;
			SendMessage(QUEUE_WRITE, tmp_message, WRITE_QUEUE_BUFFER_SIZE);
			continue;
		}
	}
	//Victory processing
	if (victory_infos.game_end == 1) {
		printf("Game end : %d\nWinner: %d\nVictory Type : %d\n",
				victory_infos.game_end, victory_infos.winner_player,
				victory_infos.victory_type);
		printf("Coordonnées du jeton de victoire: Ligne %d Colonne %d\n",
				victory_infos.victory_token_coord[0] + 1,
				victory_infos.victory_token_coord[1] + 1);
		printf(
				"Alignement victorieux:\nJeton 1: Ligne %d Colonne %d\nJeton 2: Ligne %d Colonne %d\nJeton 3: Ligne"
						" %d Colonne %d\nJeton 4: Ligne %d Colonne %d\n",
				victory_infos.victory_line_tokens_coord[0][0] + 1,
				victory_infos.victory_line_tokens_coord[0][1] + 1,
				victory_infos.victory_line_tokens_coord[1][0] + 1,
				victory_infos.victory_line_tokens_coord[1][1] + 1,
				victory_infos.victory_line_tokens_coord[2][0] + 1,
				victory_infos.victory_line_tokens_coord[2][1] + 1,
				victory_infos.victory_line_tokens_coord[3][0] + 1,
				victory_infos.victory_line_tokens_coord[3][1] + 1);
		printf("\n");
		//Victory Blink coordinates processing.
		if (victory_infos.victory_type == HORIZONTAL) {
			tmp_message[2] = LINE_VICTORY;
		} else if (victory_infos.victory_type == VERTICAL) {
			tmp_message[2] = COL_VICTORY;
		} else if (victory_infos.victory_type == RIGHT_DIAGONAL) {
			tmp_message[2] = RIGHT_DIAG_VICTORY;
		} else if (victory_infos.victory_type == LEFT_DIAGONAL) {
			tmp_message[2] = LEFT_DIAG_VICTORY;
		}
		tmp_message[0] = CONNECT_4;
		tmp_message[1] = GAME_END;
		tmp_message[3] = move.player_color;
		tmp_message[4] = victory_infos.victory_line_tokens_coord[0][0];
		tmp_message[5] = victory_infos.victory_line_tokens_coord[0][1];
		tmp_message[6] = victory_infos.victory_line_tokens_coord[3][0];
		tmp_message[7] = victory_infos.victory_line_tokens_coord[3][1];
		SendMessage(QUEUE_WRITE, tmp_message, WRITE_QUEUE_BUFFER_SIZE);
	} else if (victory_infos.game_end == 2) {
		printf("Draw. Game Over.\n");
	}
	sleep(60);
	//Emptying Grid (turn leds off).
	tmp_message[0] = CONNECT_4;
	tmp_message[1] = RESET;
	tmp_message[2] = 0;
	tmp_message[3] = 0;
	tmp_message[4] = 0;
	tmp_message[5] = 0;
	tmp_message[6] = 0;
	tmp_message[7] = 0;
	SendMessage(QUEUE_WRITE, tmp_message, WRITE_QUEUE_BUFFER_SIZE);
	sleep(5);
	pthread_detach(pthread_self());
	pthread_exit(NULL);
	return 0;
}

//Led panel controlling handler and blinking handler. 2 THREADS !.
void *thread_handler_write(void* args) {
	while (1) {
		//if (pthread_self() == write_thread_id_set) {
			//Wait message from writing queue.
		if (ReceiveMessage(QUEUE_WRITE, tmp_message_write, WRITE_QUEUE_BUFFER_SIZE) == 0){
			printf("Write handler received: %d %d %d %d %d %d %d %d\n", tmp_message_write[0], tmp_message_write[1], tmp_message_write[2], tmp_message_write[3], tmp_message_write[4], tmp_message_write[5], tmp_message_write[6], tmp_message_write[7]);

			if (tmp_message_write[0] == CONNECT_4) {

				//Console Display
				gp4_display();
				//Empty grid condition.
				if (tmp_message_write[1] == RESET) {
					for (int row = 2; row < 8; row++) {
						for (int col = 1; col < 8; col++) {
							setLedColor(row, col, colors[BLACK].RValue,
									colors[BLACK].GValue, colors[BLACK].BValue);
						}
					}
					break;
				}
				receive_count += 1;
				if (tmp_message_write[1] == PLAY) {
					//If DOWN button pressed. Validation animation.
					if (tmp_message_write[2] == DOWN_COMMAND) {
						printf("VALIDATION\n");
						receive_count = 0;
						for (int row = 2;
								row <= (tmp_message_write[6] + LED_PANEL_OFFSET);
								row++) {
							setLedColor(row - 1,
									(tmp_message_write[7] + LED_PANEL_OFFSET),
									colors[BLACK].RValue, colors[BLACK].GValue,
									colors[BLACK].BValue);
							setLedColor(row,
									(tmp_message_write[7] + LED_PANEL_OFFSET),
									colors[(int) tmp_message_write[3]].RValue,
									colors[(int) tmp_message_write[3]].GValue,
									colors[(int) tmp_message_write[3]].BValue);
							usleep(500000);
						}
						//If LEFT or RIGHT button pressed. Side move animation.
					} else if (tmp_message_write[2] == LEFT_COMMAND
							|| tmp_message_write[2] == RIGHT_COMMAND) {
						setLedColor(1,
								(tmp_message_write[5] + LED_PANEL_OFFSET),
								colors[BLACK].RValue, colors[BLACK].GValue,
								colors[BLACK].BValue);
						setLedColor(1,
								(tmp_message_write[7] + LED_PANEL_OFFSET),
								colors[(int) tmp_message_write[3]].RValue,
								colors[(int) tmp_message_write[3]].GValue,
								colors[(int) tmp_message_write[3]].BValue);
						//Next Player animation, change color and replace play token.
					} else if (tmp_message_write[2] == NEXT_PLAYER_COMMAND) {
						setLedColor(1,
								(tmp_message_write[7] + LED_PANEL_OFFSET),
								colors[(int) tmp_message_write[3]].RValue,
								colors[(int) tmp_message_write[3]].GValue,
								colors[(int) tmp_message_write[3]].BValue);
					}
					//Line victory blink.
				} else if (tmp_message_write[1] == GAME_END) {
					if (tmp_message_write[2] == LINE_VICTORY) {
						receive_count = 0;
						for (int count = 0; count < 30; count++) {
							for (int col = (tmp_message_write[5]
									+ LED_PANEL_OFFSET);
									col
											<= (tmp_message_write[7]
													+ LED_PANEL_OFFSET);
									col++) {
								setLedColor(
										tmp_message_write[4] + LED_PANEL_OFFSET
												+ TOP_ROW_OFFSET, col,
										colors[BLACK].RValue,
										colors[BLACK].GValue,
										colors[BLACK].BValue);
							}
							sleep(1);
							for (int col = (tmp_message_write[5]
									+ LED_PANEL_OFFSET);
									col
											<= (tmp_message_write[7]
													+ LED_PANEL_OFFSET);
									col++) {
								setLedColor(
										tmp_message_write[4] + LED_PANEL_OFFSET
												+ TOP_ROW_OFFSET, col,
										colors[(int) tmp_message_write[3]].RValue,
										colors[(int) tmp_message_write[3]].GValue,
										colors[(int) tmp_message_write[3]].BValue);
							}
							sleep(1);
						}
						//Column victory blink.
					} else if (tmp_message_write[2] == COL_VICTORY) {
						receive_count = 0;
						for (int count = 0; count < 30; count++) {
							for (int row = (tmp_message_write[4]
									+ LED_PANEL_OFFSET + TOP_ROW_OFFSET);
									row
											<= (tmp_message_write[6]
													+ LED_PANEL_OFFSET
													+ TOP_ROW_OFFSET); row++) {
								setLedColor(row,
										tmp_message_write[5] + LED_PANEL_OFFSET,
										colors[BLACK].RValue,
										colors[BLACK].GValue,
										colors[BLACK].BValue);
							}
							sleep(1);
							for (int row = (tmp_message_write[4]
									+ LED_PANEL_OFFSET + TOP_ROW_OFFSET);
									row
											<= (tmp_message_write[6]
													+ LED_PANEL_OFFSET
													+ TOP_ROW_OFFSET); row++) {
								setLedColor(row,
										tmp_message_write[5] + LED_PANEL_OFFSET,
										colors[(int) tmp_message_write[3]].RValue,
										colors[(int) tmp_message_write[3]].GValue,
										colors[(int) tmp_message_write[3]].BValue);
							}
							sleep(1);
						}
						//Right diagonal victory blink.
					} else if (tmp_message_write[2] == RIGHT_DIAG_VICTORY) {
						receive_count = 0;
						int win_row, win_col;
						for (int count = 0; count < 30; count++) {
							win_row = tmp_message_write[4] + LED_PANEL_OFFSET
									+ TOP_ROW_OFFSET;
							win_col = tmp_message_write[5] + LED_PANEL_OFFSET;
							for (int tokens = 0; tokens < 4; tokens++) {
								setLedColor(win_row, win_col,
										colors[BLACK].RValue,
										colors[BLACK].GValue,
										colors[BLACK].BValue);
								win_row++, win_col++;
							}
							sleep(1);
							win_row = tmp_message_write[4] + LED_PANEL_OFFSET
									+ TOP_ROW_OFFSET;
							win_col = tmp_message_write[5] + LED_PANEL_OFFSET;
							for (int tokens = 0; tokens < 4; tokens++) {
								setLedColor(win_row, win_col,
										colors[(int) tmp_message_write[3]].RValue,
										colors[(int) tmp_message_write[3]].GValue,
										colors[(int) tmp_message_write[3]].BValue);
								win_row++, win_col++;
							}
							sleep(1);
						}
						//Left diagonal victory blink.
					} else if (tmp_message_write[2] == LEFT_DIAG_VICTORY) {
						receive_count = 0;
						int win_row, win_col;
						for (int count = 0; count < 30; count++) {
							win_row = tmp_message_write[4] + LED_PANEL_OFFSET
									+ TOP_ROW_OFFSET;
							win_col = tmp_message_write[5] + LED_PANEL_OFFSET;
							for (int tokens = 0; tokens < 4; tokens++) {
								setLedColor(win_row, win_col,
										colors[BLACK].RValue,
										colors[BLACK].GValue,
										colors[BLACK].BValue);
								win_row--, win_col++;
							}
							sleep(1);
							win_row = tmp_message_write[4] + LED_PANEL_OFFSET
									+ TOP_ROW_OFFSET;
							win_col = tmp_message_write[5] + LED_PANEL_OFFSET;
							for (int tokens = 0; tokens < 4; tokens++) {
								setLedColor(win_row, win_col,
										colors[(int) tmp_message_write[3]].RValue,
										colors[(int) tmp_message_write[3]].GValue,
										colors[(int) tmp_message_write[3]].BValue);
								win_row--, win_col++;
							}
							sleep(1);
						}
					}
				}
			}
			//Play token blinking thread. receive_count is used to sync/stop blinking when necessary.
		} else{// if (pthread_self() == write_thread_id_blink) {
			if (receive_count > 0) {
				setLedColor(1, (tmp_message_write[7] + LED_PANEL_OFFSET),
						colors[(int) tmp_message_write[3]].RValue,
						colors[(int) tmp_message_write[3]].GValue,
						colors[(int) tmp_message_write[3]].BValue);
				usleep(250000);
				setLedColor(1, (tmp_message_write[7] + LED_PANEL_OFFSET),
						colors[BLACK].RValue, colors[BLACK].GValue,
						colors[BLACK].BValue);
				usleep(250000);
			} else
				usleep(500000);
		}
	}
	pthread_detach(pthread_self());
	pthread_exit(NULL);
	return 0;
}


int main(void) {

	openLink();
	read_queue = create_Queue();
	write_queue = create_Queue();

	pthread_create(&thread_id, NULL, thread_handler_read, (void*) &thread_id);
	pthread_create(&write_thread_id_set, NULL, thread_handler_write,
			(void*) &write_thread_id_set);
	//pthread_create(&write_thread_id_blink, NULL, thread_handler_write,
	//		(void*) &write_thread_id_blink);
	pthread_create(&thread_id, NULL, thread_handler_app, (void*) &thread_id);
	pthread_join(thread_id, NULL);
	printf("Fin du programme.");
	closeLink();

	return 0;
	/*
	 gp4_debug_set_line(1, "0000000");
	 gp4_debug_set_line(2, "0000001");
	 gp4_debug_set_line(3, "0000010");
	 gp4_debug_set_line(4, "0000000");
	 gp4_debug_set_line(5, "0001100");
	 gp4_debug_set_line(6, "0000000");
	 gp4_move_right();
	 gp4_move_right();
	 gp4_move_right();
	 gp4_move_right();
	 gp4_play_token();
	 gp4_display();
	 printf("\n");
	 victory_infos = gp4_check_winner();
	 printf("Game end : %d\nWinner: %d\nVictory Type : %d\n",
	 victory_infos.game_end, victory_infos.winner_player,
	 victory_infos.victory_type);
	 printf("Coordonnées du jeton de victoire: Ligne %d Colonne %d\n",
	 victory_infos.victory_token_coord[0] + 1,
	 victory_infos.victory_token_coord[1] + 1);
	 printf(
	 "Alignement victorieux:\nJeton 1: Ligne %d Colonne %d\nJeton 2: Ligne %d Colonne %d\nJeton 3: Ligne"
	 " %d Colonne %d\nJeton 4: Ligne %d Colonne %d\n",
	 victory_infos.victory_line_tokens_coord[0][0] + 1,
	 victory_infos.victory_line_tokens_coord[0][1] + 1,
	 victory_infos.victory_line_tokens_coord[1][0] + 1,
	 victory_infos.victory_line_tokens_coord[1][1] + 1,
	 victory_infos.victory_line_tokens_coord[2][0] + 1,
	 victory_infos.victory_line_tokens_coord[2][1] + 1,
	 victory_infos.victory_line_tokens_coord[3][0] + 1,
	 victory_infos.victory_line_tokens_coord[3][1] + 1);
	 printf("\n");

	 return 0;
	 */
}


