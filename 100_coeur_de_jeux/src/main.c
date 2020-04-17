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

//Display matrixes.
static led line_1[7] = { { COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF,
COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF,
COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF } };
static led line_2[7] = { { COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF,
COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF,
COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF } };
static led line_3[7] = { { COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF,
COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF,
COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF } };
static led line_4[7] = { { COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF,
COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF,
COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF } };
static led line_5[7] = { { COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF,
COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF,
COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF } };
static led line_6[7] = { { COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF,
COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF,
COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF } };
static led line_7[7] = { { COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF,
COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF,
COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF } };

static led line_1_1[7] = { { COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF,
COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF,
COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF } };
static led line_2_1[7] = { { COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF,
COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF,
COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF } };
static led line_3_1[7] = { { COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF,
COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF,
COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF } };
static led line_4_1[7] = { { COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF,
COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF,
COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF } };
static led line_5_1[7] = { { COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF,
COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF,
COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF } };
static led line_6_1[7] = { { COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF,
COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF,
COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF } };
static led line_7_1[7] = { { COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF,
COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF,
COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF } };


static led *matrice[7] = { line_1, line_2, line_3, line_4, line_5, line_6,
		line_7 };
static led *old_matrice [7]= { line_1_1, line_2_1, line_3_1, line_4_1, line_5_1, line_6_1,
		line_7_1 };

//Game pads controls
typedef enum {
	PAD_1 = 49,
	PAD_2 = 50,
	DOWN_PAD = 100,
	LEFT_PAD = 108,
	RIGHT_PAD = 114,
	UP_PAD = 117
} Pads_controls;

//Pad command Identifier
typedef enum {
	NO_PAD, PAD_1_COMMAND, PAD_2_COMMAND
} Pad_Identifier;

// Victory type command enum.
typedef enum {
	LINE_VICTORY, COL_VICTORY, RIGHT_DIAG_VICTORY, LEFT_DIAG_VICTORY
} Display_victory_command;

//Game mode enum.
typedef enum {
	NUMBERS, CONNECT_4
} Display_mode_command;

//Game Status enum.
typedef enum {
	RESET, PLAY, GAME_END
} Display_status_command;

//Game move commands enum.
typedef enum {
	DOWN_COMMAND, UP_COMMAND, LEFT_COMMAND, RIGHT_COMMAND, NEXT_PLAYER_COMMAND
} Move_command;

typedef enum{
	NO_WAIT, WAIT
}Receive_Wait_Status;

//Global Static queues, inter-thread communication.
static struct Queue* read_queue;
static struct Queue* write_queue;

static victory_infos_struct victory_infos;

//Defined colors table of structs.
/*static led colors[7] = { { COLOR_ON, COLOR_OFF, COLOR_OFF }, { COLOR_OFF,
COLOR_ON,
COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_ON }, { COLOR_ON,
COLOR_ON, COLOR_ON }, { COLOR_ON, COLOR_OFF, COLOR_ON }, { COLOR_ON,
COLOR_ON, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF } };*/

static const int mask_red = 0xFF0000;
static const int mask_green = 0xFF00;
static const int mask_blue = 0xFF;


void victory_W(int color);

void SetLedMatrice(void) {
	for (int row = 0; row < 7; row++) {
		for (int col = 0; col < 7; col++){
			if ((matrice[row][col].RValue != old_matrice[row][col].RValue) || (matrice[row][col].GValue != old_matrice[row][col].GValue) || (matrice[row][col].BValue != old_matrice[row][col].BValue)){
				setLedColor((row + 1), (col + 1), matrice[row][col].RValue,
						matrice[row][col].GValue, matrice[row][col].BValue);
			}
		}
	}
	for (int row = 0; row < 7; row++){
		for (int col = 0; col < 7; col++){
			old_matrice[row][col].RValue = matrice[row][col].RValue;
			old_matrice[row][col].GValue = matrice[row][col].GValue;
			old_matrice[row][col].BValue = matrice[row][col].BValue;
		}
	}
}

void SetTokenColor(led* token, unsigned int color){
	token->RValue = (color & mask_red) >> 16;
	token->GValue = (color & mask_green) >> 8;
	token->BValue = color & mask_blue;
}

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
		if (message_length == READ_QUEUE_BUFFER_SIZE) {
			add_to_Queue(read_queue, message);
			sem_post(&read_queue->semaphore);
			printf("Message sent.\n");
		} else
			printf("Send message Failed ! Buffer Overflow !\n");
	} else if (queue == QUEUE_WRITE) {
		if (message_length == WRITE_QUEUE_BUFFER_SIZE) {
			add_to_Queue(write_queue, message);
			sem_post(&write_queue->semaphore);
			printf("Message sent.\n");
		} else
			printf("Send message Failed ! Buffer Overflow !\n");
	} else
		printf("Send message Failed ! Queue unidentified !\n");
}

//Receive message through queues, inter-thread communication.
int ReceiveMessage(Queue_id queue, char* message, int message_length, int wait_status) {
	if (queue == QUEUE_WRITE) {
		if (message_length == WRITE_QUEUE_BUFFER_SIZE) {
			if (wait_status == WAIT){
				sem_wait(&write_queue->semaphore);
				printf("Message received.\n");
			}
			int status = del_from_Queue(write_queue, message);
			return status;
		} else {
			printf("Receive message failed ! Buffer Overflow !\n");
			return -1;
		}
	} else if (queue == QUEUE_READ) {
		if (message_length == READ_QUEUE_BUFFER_SIZE) {
			if (wait_status == WAIT){
				sem_wait(&read_queue->semaphore);
				printf("Message received.\n");
			}
			del_from_Queue(read_queue, message);
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
			} else if (button_read[1] == PAD_2) {
				command[0] = UP_COMMAND;
				command[1] = PAD_2_COMMAND;
			}
		} else if (button_read[3] == DOWN_PAD && button_read[2] == DOWN_PAD) {
			if (button_read[1] == PAD_1) {
				command[0] = DOWN_COMMAND;
				command[1] = PAD_1_COMMAND;
			} else if (button_read[1] == PAD_2) {
				command[0] = DOWN_COMMAND;
				command[1] = PAD_2_COMMAND;
			}
		} else if (button_read[3] == DOWN_PAD && button_read[2] == LEFT_PAD) {
			if (button_read[1] == PAD_1) {
				command[0] = LEFT_COMMAND;
				command[1] = PAD_1_COMMAND;
			} else if (button_read[1] == PAD_2) {
				command[0] = LEFT_COMMAND;
				command[1] = PAD_2_COMMAND;
			}
		} else if (button_read[3] == DOWN_PAD && button_read[2] == RIGHT_PAD) {
			if (button_read[1] == PAD_1) {
				command[0] = RIGHT_COMMAND;
				command[1] = PAD_1_COMMAND;
			} else if (button_read[1] == PAD_2) {
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
		ReceiveMessage(QUEUE_READ, tmp_command, READ_QUEUE_BUFFER_SIZE, WAIT);
		printf("App handler received: %d %d\n", tmp_command[0], tmp_command[1]);

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
		} else if (tmp_command[0] == LEFT_COMMAND
				&& tmp_command[1] == move.player) {
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
		} else if (tmp_command[0] == RIGHT_COMMAND
				&& tmp_command[1] == move.player) {
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

//Led panel controlling handler.
void *thread_handler_write(void* args) {
	int receive_count = 1;
	char tmp_message_write[8];
	tmp_message_write[3] = 1;
	unsigned int color = GREEN_COLOR;
	SetTokenColor(&matrice[0][0], GREEN);
	SetLedMatrice();
	while (1) {
		//Wait message from writing queue.
		if (ReceiveMessage(QUEUE_WRITE, tmp_message_write,
				WRITE_QUEUE_BUFFER_SIZE, NO_WAIT) == 0) {
			switch (tmp_message_write[3]) {
			case RED:
				color = RED_COLOR;
				break;
			case GREEN:
				color = GREEN_COLOR;
				break;
			case BLUE:
				color = BLUE_COLOR;
				break;
			case WHITE:
				color = WHITE_COLOR;
				break;
			case PURPLE:
				color = PURPLE_COLOR;
				break;
			case YELLOW:
				color = YELLOW_COLOR;
				break;
			case BLACK:
				color = BLACK_COLOR;
				break;
			default : color = BLACK_COLOR;
			}
			printf("Write handler received: %d %d %d %d %d %d %d %d\n",
					tmp_message_write[0], tmp_message_write[1],
					tmp_message_write[2], tmp_message_write[3],
					tmp_message_write[4], tmp_message_write[5],
					tmp_message_write[6], tmp_message_write[7]);

			if (tmp_message_write[0] == CONNECT_4) {

				//Console Display
				gp4_display();
				//Empty grid condition.
				if (tmp_message_write[1] == RESET) {

					for (int row = 1; row < 7; row++){
						for (int col = 0; col < 7; col++){

							SetTokenColor(&matrice[row][col], BLACK_COLOR);
						}
					}
					SetLedMatrice();
					break;
				}
				receive_count += 1;
				if (tmp_message_write[1] == PLAY) {
					//If DOWN button pressed. Validation animation.
					if (tmp_message_write[2] == DOWN_COMMAND) {
						printf("VALIDATION\n");
						receive_count = 0;
						int previous_row;
						int col = (int)tmp_message_write[7];
						for (int row = 2;
								row < (tmp_message_write[6] + LED_PANEL_OFFSET);
								row++) {
							previous_row = row - 1;

							SetTokenColor(&matrice[previous_row][col], BLACK_COLOR);

							SetTokenColor(&matrice[row][col], color);
							SetLedMatrice();
							usleep(150000);
						}



						//If LEFT or RIGHT button pressed. Side move animation.
					} else if (tmp_message_write[2] == LEFT_COMMAND
							|| tmp_message_write[2] == RIGHT_COMMAND) {

						SetTokenColor(&matrice[0][(int) tmp_message_write[5]], BLACK_COLOR);

						SetTokenColor(&matrice[0][(int) tmp_message_write[7]], color);
					//Next Player animation, change color and replace play token.
					} else if (tmp_message_write[2] == NEXT_PLAYER_COMMAND) {

						SetTokenColor(&matrice[0][(int) tmp_message_write[7]], color);
					}
					//Line victory blink.
				} else if (tmp_message_write[1] == GAME_END) {
					if (tmp_message_write[2] == LINE_VICTORY) {
						receive_count = 0;
						int row = (int)tmp_message_write[4] + TOP_ROW_OFFSET;
						for (int count = 0; count < 15; count++) {
							for (int col = tmp_message_write[5];
									col
											<= tmp_message_write[7];
									col++) {

								SetTokenColor(&matrice[row][col], BLACK_COLOR);
							}
							SetLedMatrice();
							usleep(500000);
							for (int col = tmp_message_write[5];
									col
											<= tmp_message_write[7];
									col++) {

								SetTokenColor(&matrice[row][col], color);
							}
							SetLedMatrice();
							usleep(500000);
						}
						victory_W(color);
						//Column victory blink.
					} else if (tmp_message_write[2] == COL_VICTORY) {
						receive_count = 0;
						int col = (int)tmp_message_write[5];
						for (int count = 0; count < 15; count++) {
							for (int row = (tmp_message_write[4] + TOP_ROW_OFFSET);
									row
											<= (tmp_message_write[6]
													+ TOP_ROW_OFFSET);
									row++) {

								SetTokenColor(&matrice[row][col], BLACK_COLOR);
							}
							SetLedMatrice();
							usleep(500000);
							for (int row = (tmp_message_write[4] + TOP_ROW_OFFSET);
									row
											<= (tmp_message_write[6]
													+ TOP_ROW_OFFSET);
									row++) {

								SetTokenColor(&matrice[row][col], color);
							}
							SetLedMatrice();
							usleep(500000);
						}
						victory_W(color);
						//Right diagonal victory blink.
					} else if (tmp_message_write[2] == RIGHT_DIAG_VICTORY) {
						receive_count = 0;
						int win_row, win_col;
						for (int count = 0; count < 15; count++) {
							win_row = tmp_message_write[4] + TOP_ROW_OFFSET;
							win_col = tmp_message_write[5];
							for (int tokens = 0; tokens < 4; tokens++) {

								SetTokenColor(&matrice[win_row][win_col], BLACK_COLOR);
								win_row++, win_col++;
							}
							SetLedMatrice();
							usleep(500000);
							win_row = tmp_message_write[4] + TOP_ROW_OFFSET;
							win_col = tmp_message_write[5];
							for (int tokens = 0; tokens < 4; tokens++) {

								SetTokenColor(&matrice[win_row][win_col], color);
								win_row++, win_col++;
							}
							SetLedMatrice();
							usleep(500000);
						}
						victory_W(color);
						//Left diagonal victory blink.
					} else if (tmp_message_write[2] == LEFT_DIAG_VICTORY) {
						receive_count = 0;
						int win_row, win_col;
						for (int count = 0; count < 15; count++) {
							win_row = tmp_message_write[4] + TOP_ROW_OFFSET;
							win_col = tmp_message_write[5];
							for (int tokens = 0; tokens < 4; tokens++) {

								SetTokenColor(&matrice[win_row][win_col], BLACK_COLOR);
								win_row--, win_col++;
							}
							SetLedMatrice();
							usleep(500000);
							win_row = tmp_message_write[4] + TOP_ROW_OFFSET;
							win_col = tmp_message_write[5];
							for (int tokens = 0; tokens < 4; tokens++) {

								SetTokenColor(&matrice[win_row][win_col], color);
								win_row--, win_col++;
							}
							SetLedMatrice();
							usleep(500000);
						}
						victory_W(color);
					}
				}
			}
			SetLedMatrice();
		} else {
			if (receive_count > 0) {

				SetTokenColor(&matrice[0][(int)tmp_message_write[7]], color);
				SetLedMatrice();
				usleep(250000);

				SetTokenColor(&matrice[0][(int)tmp_message_write[7]], BLACK_COLOR);
				SetLedMatrice();
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
	pthread_t thread_id;
	read_queue = create_Queue();
	write_queue = create_Queue();

	openLink();
	pthread_create(&thread_id, NULL, thread_handler_read, (void*) &thread_id);
	pthread_create(&thread_id, NULL, thread_handler_write, (void*) &thread_id);
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

void victory_W(int color){
	for (int row = 0; row < 7; row++) {
		for (int col = 0; col < 7; col++) {
			SetTokenColor(&matrice[row][col], BLACK_COLOR);
		}
	}
	SetLedMatrice();
	for (int count = 0; count < 30; count++) {

		SetTokenColor(&matrice[1][1], BLACK_COLOR);
		SetTokenColor(&matrice[2][1], BLACK_COLOR);
		SetTokenColor(&matrice[3][1], BLACK_COLOR);
		SetTokenColor(&matrice[4][1], BLACK_COLOR);
		SetTokenColor(&matrice[5][2], BLACK_COLOR);
		SetTokenColor(&matrice[4][3], BLACK_COLOR);
		SetTokenColor(&matrice[5][4], BLACK_COLOR);
		SetTokenColor(&matrice[4][5], BLACK_COLOR);
		SetTokenColor(&matrice[3][5], BLACK_COLOR);
		SetTokenColor(&matrice[2][5], BLACK_COLOR);
		SetTokenColor(&matrice[1][5], BLACK_COLOR);
		SetLedMatrice();
		/*setLedColor(2, 2, ((BLACK_COLOR & mask_red) >> 16),
				((BLACK_COLOR & mask_green) >> 8), (BLACK_COLOR & mask_blue));
		setLedColor(3, 2, ((BLACK_COLOR & mask_red) >> 16),
				((BLACK_COLOR & mask_green) >> 8), (BLACK_COLOR & mask_blue));
		setLedColor(4, 2, ((BLACK_COLOR & mask_red) >> 16),
				((BLACK_COLOR & mask_green) >> 8), (BLACK_COLOR & mask_blue));
		setLedColor(5, 2, ((BLACK_COLOR & mask_red) >> 16),
				((BLACK_COLOR & mask_green) >> 8), (BLACK_COLOR & mask_blue));
		setLedColor(6, 3, ((BLACK_COLOR & mask_red) >> 16),
				((BLACK_COLOR & mask_green) >> 8), (BLACK_COLOR & mask_blue));
		setLedColor(5, 4, ((BLACK_COLOR & mask_red) >> 16),
				((BLACK_COLOR & mask_green) >> 8), (BLACK_COLOR & mask_blue));
		setLedColor(6, 5, ((BLACK_COLOR & mask_red) >> 16),
				((BLACK_COLOR & mask_green) >> 8), (BLACK_COLOR & mask_blue));
		setLedColor(5, 6, ((BLACK_COLOR & mask_red) >> 16),
				((BLACK_COLOR & mask_green) >> 8), (BLACK_COLOR & mask_blue));
		setLedColor(4, 6, ((BLACK_COLOR & mask_red) >> 16),
				((BLACK_COLOR & mask_green) >> 8), (BLACK_COLOR & mask_blue));
		setLedColor(3, 6, ((BLACK_COLOR & mask_red) >> 16),
				((BLACK_COLOR & mask_green) >> 8), (BLACK_COLOR & mask_blue));
		setLedColor(2, 6, ((BLACK_COLOR & mask_red) >> 16),
				((BLACK_COLOR & mask_green) >> 8), (BLACK_COLOR & mask_blue));*/
		usleep(500000);
		SetTokenColor(&matrice[1][1], color);
		SetTokenColor(&matrice[2][1], color);
		SetTokenColor(&matrice[3][1], color);
		SetTokenColor(&matrice[4][1], color);
		SetTokenColor(&matrice[5][2], color);
		SetTokenColor(&matrice[4][3], color);
		SetTokenColor(&matrice[5][4], color);
		SetTokenColor(&matrice[4][5], color);
		SetTokenColor(&matrice[3][5], color);
		SetTokenColor(&matrice[2][5], color);
		SetTokenColor(&matrice[1][5], color);
		SetLedMatrice();
		/*setLedColor(2, 2, ((color & mask_red) >> 16),
				((color & mask_green) >> 8), (color & mask_blue));
		setLedColor(3, 2, ((color & mask_red) >> 16),
				((color & mask_green) >> 8), (color & mask_blue));
		setLedColor(4, 2, ((color & mask_red) >> 16),
				((color & mask_green) >> 8), (color & mask_blue));
		setLedColor(5, 2, ((color & mask_red) >> 16),
				((color & mask_green) >> 8), (color & mask_blue));
		setLedColor(6, 3, ((color & mask_red) >> 16),
				((color & mask_green) >> 8), (color & mask_blue));
		setLedColor(5, 4, ((color & mask_red) >> 16),
				((color & mask_green) >> 8), (color & mask_blue));
		setLedColor(6, 5, ((color & mask_red) >> 16),
				((color & mask_green) >> 8), (color & mask_blue));
		setLedColor(5, 6, ((color & mask_red) >> 16),
				((color & mask_green) >> 8), (color & mask_blue));
		setLedColor(4, 6, ((color & mask_red) >> 16),
				((color & mask_green) >> 8), (color & mask_blue));
		setLedColor(3, 6, ((color & mask_red) >> 16),
				((color & mask_green) >> 8), (color & mask_blue));
		setLedColor(2, 6, ((color & mask_red) >> 16),
				((color & mask_green) >> 8), (color & mask_blue));*/
		usleep(500000);
	}
	for (int row = 0; row < 7; row++) {
			for (int col = 0; col < 7; col++) {
				SetTokenColor(&matrice[row][col], BLACK_COLOR);
			}
		}
		SetLedMatrice();
}
