/*
 * display_thread.c
 *
 *  Created on: Apr 17, 2020
 *      Author: jonathan
 */

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include"cmsis_os.h"
#include"leds_control.h"
#include "semphr.h"
#include"main.h"
#include"game_p4.h"
#include"display_thread.h"
#include"numbers_module.h"
#include"timer_threads.h"

#define VICTORY_BLINK_COUNT 5
typedef enum {
	PLAY_TOKEN, VICTORY,
} blink_type;

typedef struct {
	int color;
	blink_type blink_type;
	int8_t blink_count;
	int8_t blink_status;
	int8_t* coords_table;
	timer_struct_t timer;
} blink_struct;

typedef struct {
	int color;
	int8_t* coords_table;
	timer_struct_t timer;
} valid_struct;



extern timer_struct_t play_token_blink;
extern timer_struct_t victory_blink;
extern timer_struct_t valid_move;

//Display queue, gets incoming commands from app.
extern osMessageQueueId_t display_queueHandle;

//Display matrixes.
static led matrix[DISPLAY_ROWS][DISPLAY_COLS];
static led old_matrix[DISPLAY_ROWS][DISPLAY_COLS];

void SendToDisplay(Msg_t* command, size_t size) {
	osMessageQueuePut(display_queueHandle, command, 0U, 0U);
}

extern SemaphoreHandle_t start_signalHandle;
void wait_for_display_init(void) {
	xSemaphoreTake(start_signalHandle, portMAX_DELAY);
}

extern SemaphoreHandle_t reset_signalHandle;
void wait_for_display_reset(void) {
	xSemaphoreTake(reset_signalHandle, portMAX_DELAY);
}
extern SemaphoreHandle_t game_end_animHandle;
extern SemaphoreHandle_t valid_animHandle;
//Send display matrix to display output.
static void SetLedMatrix(void) {
	for (uint8_t row = 0; row < DISPLAY_ROWS; row++) {
		for (uint8_t col = 0; col < DISPLAY_COLS; col++) {
			if ((matrix[row][col].RValue != old_matrix[row][col].RValue)
					|| (matrix[row][col].GValue != old_matrix[row][col].GValue)
					|| (matrix[row][col].BValue != old_matrix[row][col].BValue)) {
				setLedColor((row + 1), (col + 1), matrix[row][col].RValue,
						matrix[row][col].GValue, matrix[row][col].BValue);
			}
		}
	}
	for (uint8_t row = 0; row < DISPLAY_ROWS; row++) {
		for (uint8_t col = 0; col < DISPLAY_COLS; col++) {
			old_matrix[row][col].RValue = matrix[row][col].RValue;
			old_matrix[row][col].GValue = matrix[row][col].GValue;
			old_matrix[row][col].BValue = matrix[row][col].BValue;
		}
	}
}

//Set selected hexadecimal color on selected led struct token.
void SetMatrixTokenColor(led* token, int color) {
	token->RValue = (color & MASK_RED) >> 16;
	token->GValue = (color & MASK_GREEN) >> 8;
	token->BValue = color & MASK_BLUE;
}

//Flush down reset animation.
static void reset_matrix_animation(void) {
	for (uint8_t max_row = 1; max_row <= DISPLAY_ROWS; max_row++) {
		if (max_row < DISPLAY_ROWS) {
			for (uint8_t row = DISPLAY_ROWS - TOP_ROW_OFFSET; row > 0; row--) {
				for (uint8_t col = 0; col < DISPLAY_COLS; col++) {
					SetMatrixTokenColor(&matrix[row - 1][col],
					BACKGROUND_COLOR);
					matrix[row][col] = old_matrix[row - 1][col];
				}
			}
			SetLedMatrix();
			osDelay(150);
		} else {
			for (uint8_t col = 0; col < DISPLAY_COLS; col++) {
				SetMatrixTokenColor(&matrix[max_row - 1][col],
				BACKGROUND_COLOR);
			}
			SetLedMatrix();
			osDelay(100);
		}
	}
}

//Side moves animation.
static void gp4_move_sides_animation(Msg_t* tmp_message) {
	SetMatrixTokenColor(&matrix[0][tmp_message->move_coordinates.beg.c],
	BACKGROUND_COLOR);
	SetMatrixTokenColor(&matrix[0][tmp_message->move_coordinates.end.c],
			tmp_message->move_coordinates.player_color);
	SetLedMatrix();
}

void validation_anim_handler(void* callback_param) {
	valid_struct* valid_infos = (valid_struct*) callback_param;
	int8_t row;
	int8_t previous_row;
	int8_t col = valid_infos->coords_table[1];
	static int8_t coords_table_row_index = 2;
	if (valid_infos->coords_table[coords_table_row_index] != -1) {
		row = valid_infos->coords_table[coords_table_row_index];
		previous_row = row - 1;
		SetMatrixTokenColor(&matrix[previous_row][col],
		BACKGROUND_COLOR);
		SetMatrixTokenColor(&matrix[row][col], valid_infos->color);
		SetLedMatrix();
		coords_table_row_index += 2;

	} else {
		valid_move.timer_force = FORCE_OFF;
		coords_table_row_index = 2;
		xSemaphoreGive(valid_animHandle);
	}
}

//Down (Validation) move animation.
static void gp4_move_down_animation(Msg_t* tmp_message_display) {
	valid_struct valid_infos;
	int8_t validation_table[15];
	valid_move.Timer_Callback = validation_anim_handler;
	valid_move.callback_param = &valid_infos;
	valid_infos.timer = valid_move;
	valid_infos.coords_table = validation_table;
	valid_infos.color = tmp_message_display->move_coordinates.player_color;
	uint8_t col = tmp_message_display->move_coordinates.end.c;
	uint8_t validation_table_index = 0;
	for (uint8_t row = 0; row <= tmp_message_display->move_coordinates.end.l;
			row++) {
		validation_table[validation_table_index] = row;
		validation_table[validation_table_index + 1] = col;
		validation_table_index += 2;
	}
	validation_table[validation_table_index] = BLINK_TABLE_STOP_INT;
	start_new_timer(&valid_infos.timer, 1);
	xSemaphoreTake(valid_animHandle, portMAX_DELAY);
	stop_new_timer(&valid_infos.timer);
}

//Next player animation. Replace token to start position and change color.
static void gp4_next_player_animation(Msg_t* tmp_message) {
	SetMatrixTokenColor(&matrix[0][tmp_message->move_coordinates.end.c],
			tmp_message->move_coordinates.player_color);
	SetLedMatrix();
}

//Blink animation. Takes an array of leds coords and make them blink with the wanted color.
static void blink_animation(int8_t coords_table[], int color, int blink_delay,
		uint8_t cycles_count) {
	uint8_t row;
	uint8_t col;
	while (cycles_count > 0) {
		row = 0;
		col = 1;
		while (coords_table[row] != BLINK_TABLE_STOP_INT) {
			SetMatrixTokenColor(&matrix[coords_table[row]][coords_table[col]],
			BACKGROUND_COLOR);
			row += 2;
			col += 2;
		}
		SetLedMatrix();
		osDelay(blink_delay);
		row = 0;
		col = 1;
		while (coords_table[row] != BLINK_TABLE_STOP_INT) {
			SetMatrixTokenColor(&matrix[coords_table[row]][coords_table[col]],
					color);
			row += 2;
			col += 2;
		}
		SetLedMatrix();
		osDelay(blink_delay);
		cycles_count -= 1;
	}
}

// Play token and victory blink animations callback.

static void blink_on(void* blink_infos_ptr);
static void blink_off(void* blink_infos_ptr);

void blink_callback(void* blink_infos_ptr) {
	blink_struct* blink_infos = (blink_struct*) blink_infos_ptr;
	if (blink_infos->blink_status == 0) {
		blink_on(blink_infos);
		blink_infos->blink_status = 1;
	} else {
		blink_off(blink_infos);
		blink_infos->blink_status = 0;
	}
}

static void blink_on(void* blink_infos_ptr) {
	blink_struct* blink_infos = (blink_struct*) blink_infos_ptr;
	uint8_t row = 0;
	uint8_t col = 1;
	while (blink_infos->coords_table[row] != BLINK_TABLE_STOP_INT) {
		SetMatrixTokenColor(
				&matrix[blink_infos->coords_table[row]][blink_infos->coords_table[col]],
				blink_infos->color);
		row += 2;
		col += 2;
	}
	SetLedMatrix();
	if (blink_infos->blink_count == -1) {
		if (blink_infos->timer.timer_signal != STOP_TIMER) {
			blink_infos->timer.timer_force = FORCE_ON;
			blink_infos->timer.Timer_Callback = blink_off;
		} else
			blink_infos->timer.timer_force = FORCE_OFF;
	} else {
		if (blink_infos->blink_count > 0) {
			blink_infos->timer.timer_force = FORCE_ON;
			blink_infos->blink_count -= 1;
			blink_infos->timer.Timer_Callback = blink_off;
		} else {
			blink_infos->timer.timer_signal = STOP_TIMER;
			blink_infos->timer.timer_force = FORCE_OFF;
			if (blink_infos->blink_type == VICTORY){
				xSemaphoreGive(game_end_animHandle);
			}
		}
	}
}

static void blink_off(void* blink_infos_ptr) {
	blink_struct* blink_infos = (blink_struct*) blink_infos_ptr;
	uint8_t row = 0;
	uint8_t col = 1;
	while (blink_infos->coords_table[row] != BLINK_TABLE_STOP_INT) {
		SetMatrixTokenColor(
				&matrix[blink_infos->coords_table[row]][blink_infos->coords_table[col]],
				BACKGROUND_COLOR);
		row += 2;
		col += 2;
	}
	SetLedMatrix();
	if (blink_infos->blink_count == -1) {
		if (blink_infos->timer.timer_signal != STOP_TIMER) {
			blink_infos->timer.timer_force = FORCE_ON;
			blink_infos->timer.Timer_Callback = blink_on;
		} else
			blink_infos->timer.timer_force = FORCE_OFF;
	} else {
		if (blink_infos->blink_count > 0) {
			blink_infos->timer.timer_force = FORCE_ON;
			blink_infos->timer.Timer_Callback = blink_on;
		} else {
			blink_infos->timer.timer_signal = STOP_TIMER;
			blink_infos->timer.timer_force = FORCE_OFF;
		}
	}
}

static void gp4_line_victory_animation(Msg_t* tmp_message) {
	uint8_t row = tmp_message->move_coordinates.beg.l + TOP_ROW_OFFSET;
	int8_t blink_table[VICTORY_BLINK_TABLE_SIZE];
	uint8_t blink_table_index = 0;
	blink_struct blink_infos;
	blink_infos.blink_count = VICTORY_BLINK_COUNT;
	blink_infos.color = tmp_message->move_coordinates.player_color;
	blink_infos.coords_table = blink_table;
	blink_infos.blink_type = VICTORY;
	victory_blink.callback_param = &blink_infos;
	victory_blink.Timer_Callback = blink_callback;
	blink_infos.timer = victory_blink;
	for (uint8_t col = tmp_message->move_coordinates.beg.c;
			col <= tmp_message->move_coordinates.end.c; col++) {

		blink_table[blink_table_index] = row;
		blink_table[blink_table_index + 1] = col;
		blink_table_index += 2;
	}
	blink_table[blink_table_index] = BLINK_TABLE_STOP_INT;
	start_new_timer(&blink_infos.timer, 1);
	xSemaphoreTake(game_end_animHandle, portMAX_DELAY);
	stop_new_timer(&blink_infos.timer);
}

static void gp4_column_victory_animation(Msg_t* tmp_message) {
	uint8_t col = tmp_message->move_coordinates.beg.c;
	int8_t blink_table[VICTORY_BLINK_TABLE_SIZE];
	uint8_t blink_table_index = 0;
	blink_struct blink_infos;
	blink_infos.blink_count = VICTORY_BLINK_COUNT;
	blink_infos.color = tmp_message->move_coordinates.player_color;
	blink_infos.coords_table = blink_table;
	blink_infos.blink_type = VICTORY;
	victory_blink.callback_param = &blink_infos;
	victory_blink.Timer_Callback = blink_callback;
	blink_infos.timer = victory_blink;
	for (uint8_t row = (tmp_message->move_coordinates.beg.l + TOP_ROW_OFFSET);
			row <= (tmp_message->move_coordinates.end.l + TOP_ROW_OFFSET);
			row++) {

		blink_table[blink_table_index] = row;
		blink_table[blink_table_index + 1] = col;
		blink_table_index += 2;
	}
	blink_table[blink_table_index] = BLINK_TABLE_STOP_INT;
	start_new_timer(&blink_infos.timer, 1);
	xSemaphoreTake(game_end_animHandle, portMAX_DELAY);
	stop_new_timer(&blink_infos.timer);
}

static void gp4_right_diag_victory_animation(Msg_t* tmp_message) {
	int8_t win_row, win_col;
	win_row = tmp_message->move_coordinates.beg.l + TOP_ROW_OFFSET;
	win_col = tmp_message->move_coordinates.beg.c;
	int8_t blink_table[VICTORY_BLINK_TABLE_SIZE];
	uint8_t blink_table_index = 0;
	blink_struct blink_infos;
	blink_infos.blink_count = VICTORY_BLINK_COUNT;
	blink_infos.color = tmp_message->move_coordinates.player_color;
	blink_infos.coords_table = blink_table;
	blink_infos.blink_type = VICTORY;
	victory_blink.callback_param = &blink_infos;
	victory_blink.Timer_Callback = blink_callback;
	blink_infos.timer = victory_blink;
	for (uint8_t tokens = 0; tokens < 4; tokens++) {

		blink_table[blink_table_index] = win_row;
		blink_table[blink_table_index + 1] = win_col;
		blink_table_index += 2;
		win_row++, win_col++;
	}
	blink_table[blink_table_index] = BLINK_TABLE_STOP_INT;
	start_new_timer(&blink_infos.timer, 1);
	xSemaphoreTake(game_end_animHandle, portMAX_DELAY);
	stop_new_timer(&blink_infos.timer);
}

static void gp4_left_diag_victory_animation(Msg_t* tmp_message) {
	int8_t win_row, win_col;
	int8_t blink_table[VICTORY_BLINK_TABLE_SIZE];
	win_row = tmp_message->move_coordinates.beg.l + TOP_ROW_OFFSET;
	win_col = tmp_message->move_coordinates.beg.c;
	uint8_t blink_table_index = 0;
	blink_struct blink_infos;
	blink_infos.blink_count = VICTORY_BLINK_COUNT;
	blink_infos.color = tmp_message->move_coordinates.player_color;
	blink_infos.coords_table = blink_table;
	blink_infos.blink_type = VICTORY;
	victory_blink.callback_param = &blink_infos;
	victory_blink.Timer_Callback = blink_callback;
	blink_infos.timer = victory_blink;
	for (uint8_t tokens = 0; tokens < 4; tokens++) {

		blink_table[blink_table_index] = win_row;
		blink_table[blink_table_index + 1] = win_col;
		blink_table_index += 2;
		win_row--, win_col++;
	}
	blink_table[blink_table_index] = BLINK_TABLE_STOP_INT;
	start_new_timer(&blink_infos.timer, 1);
	xSemaphoreTake(game_end_animHandle, portMAX_DELAY);
	stop_new_timer(&blink_infos.timer);
}

static void victory_W(int color) {
	reset_matrix_animation();
	int8_t blink_table[W_BLINK_TABLE_SIZE];
	//W Led 1 Row/Col
	blink_table[0] = 1;
	blink_table[1] = 1;
	//W Led 2 Row/Col
	blink_table[2] = 2;
	blink_table[3] = 1;
	//W Led 3 Row/Col
	blink_table[4] = 3;
	blink_table[5] = 1;
	//W Led 4 Row/Col
	blink_table[6] = 4;
	blink_table[7] = 1;
	//W Led 5 Row/Col
	blink_table[8] = 5;
	blink_table[9] = 2;
	//W Led 6 Row/Col
	blink_table[10] = 4;
	blink_table[11] = 3;
	//W Led 7 Row/Col
	blink_table[12] = 5;
	blink_table[13] = 4;
	//W Led 8 Row/Col
	blink_table[14] = 4;
	blink_table[15] = 5;
	//W Led 9 Row/Col
	blink_table[16] = 3;
	blink_table[17] = 5;
	//W Led 10 Row/Col
	blink_table[18] = 2;
	blink_table[19] = 5;
	//W Led 11 Row/Col
	blink_table[20] = 1;
	blink_table[21] = 5;
	//Leds isolation stopping int.
	blink_table[22] = BLINK_TABLE_STOP_INT;
	blink_animation(blink_table, color,
	BLINK_DELAY_VICTORY_W, BLINK_COUNT_VICTORY_W);
	reset_matrix_animation();
}

static void wait_animation(int color) {
	int8_t blink_table[39];

	blink_table[0] = 0;
	blink_table[1] = 0;

	blink_table[2] = 0;
	blink_table[3] = 1;

	blink_table[4] = 0;
	blink_table[5] = 2;

	blink_table[6] = 0;
	blink_table[7] = 3;

	blink_table[8] = 1;
	blink_table[9] = 0;

	blink_table[10] = 2;
	blink_table[11] = 0;

	blink_table[12] = 3;
	blink_table[13] = 0;

	blink_table[14] = 3;
	blink_table[15] = 1;

	blink_table[16] = 3;
	blink_table[17] = 2;

	blink_table[18] = 3;
	blink_table[19] = 6;

	blink_table[20] = 4;
	blink_table[21] = 3;

	blink_table[22] = 4;
	blink_table[23] = 4;

	blink_table[24] = 4;
	blink_table[25] = 6;

	blink_table[26] = 5;
	blink_table[27] = 3;

	blink_table[28] = 5;
	blink_table[29] = 5;

	blink_table[30] = 5;
	blink_table[31] = 6;

	blink_table[32] = 6;
	blink_table[33] = 3;

	blink_table[34] = 6;
	blink_table[35] = 6;

	blink_table[36] = 3;
	blink_table[37] = 3;

	blink_table[38] = -1;
	blink_animation(blink_table, color,
	BLINK_DELAY_VICTORY_TOKENS, 3);
}

static void countdown(void) {
	int color = COLOR_P_1;
	for (int number = 3; number >= 0; number--) { /*Boucle de décompte de 9 à 0*/
		Setnumber(matrix, number, color);
		SetLedMatrix();
		osDelay(1000);
		reset_matrix_animation();
		if (color == COLOR_P_1)
			color = COLOR_P_2;
		else
			color = COLOR_P_1;
	}
}

static void display_message_init(Msg_t* message) {
	message->mode = 0;
	message->type = 0;
	message->status = 0;
	message->pad = 0;
	message->victory_type = 0;
	message->move_command = 0;
	message->move_coordinates.player = 1;
	message->move_coordinates.player_color = COLOR_P_1;
	message->move_coordinates.beg.l = 0;
	message->move_coordinates.beg.c = 0;
	message->move_coordinates.end.l = 0;
	message->move_coordinates.end.c = PLAY_TOKEN_STARTING_COL;
}

static void display_init() {

	for (uint8_t row = 0; row < DISPLAY_ROWS; row++) {
		for (uint8_t col = 0; col < DISPLAY_COLS; col++) {
			SetMatrixTokenColor(&matrix[row][col], BACKGROUND_COLOR);
		}
	}
	SetMatrixTokenColor(&matrix[0][PLAY_TOKEN_STARTING_COL], COLOR_P_1);
	SetLedMatrix();
}

void play_message_processing(Msg_t* tmp_message_display,
		blink_struct* blink_infos, uint8_t* play_token_refresh_security) {
	//If DOWN button pressed. Validation animation.
	if (tmp_message_display->move_command == DOWN_COMMAND) {
		*play_token_refresh_security = 0;
		gp4_move_down_animation(tmp_message_display);
		//If LEFT or RIGHT button pressed. Side move animation.
	} else if (tmp_message_display->move_command == LEFT_COMMAND
			|| tmp_message_display->move_command == RIGHT_COMMAND) {
		gp4_move_sides_animation(tmp_message_display);
		//Next Player animation, change color and replace play token.
	} else if (tmp_message_display->move_command == NEXT_PLAYER_COMMAND) {
		gp4_next_player_animation(tmp_message_display);
	}
}

void game_end_message_processing(Msg_t* tmp_message_display) {
	if (tmp_message_display->victory_type == LINE_VICTORY) {
		//Line victory blink.
		gp4_line_victory_animation(tmp_message_display);
		victory_W(tmp_message_display->move_coordinates.player_color);
		//Column victory blink.
	} else if (tmp_message_display->victory_type == COL_VICTORY) {
		gp4_column_victory_animation(tmp_message_display);
		victory_W(tmp_message_display->move_coordinates.player_color);
		//Right diagonal victory blink.
	} else if (tmp_message_display->victory_type == RIGHT_DIAG_VICTORY) {
		gp4_right_diag_victory_animation(tmp_message_display);
		victory_W(tmp_message_display->move_coordinates.player_color);
		//Left diagonal victory blink.
	} else if (tmp_message_display->victory_type == LEFT_DIAG_VICTORY) {
		gp4_left_diag_victory_animation(tmp_message_display);
		victory_W(tmp_message_display->move_coordinates.player_color);
	} else if (tmp_message_display->victory_type == DRAW_END) {
		reset_matrix_animation();
	}
}

void connect_4_message_processing(Msg_t* tmp_message_display,
		blink_struct* blink_infos, uint8_t* display_restart,
		uint8_t* play_token_refresh_security) {
	//Empty grid condition.
	if (tmp_message_display->status == RESTART) {
		reset_matrix_animation();
		return;
	}
	*play_token_refresh_security = 1;
	if (tmp_message_display->status == PLAY) {
		play_message_processing(tmp_message_display, blink_infos,
				&(*play_token_refresh_security));
	} else if (tmp_message_display->status == GAME_END) {
		game_end_message_processing(tmp_message_display);
		*display_restart = 1;
		xSemaphoreGive(reset_signalHandle);
	}
}

//Led panel controlling handler.
void *thread_handler_display(void* args) {

	int8_t blink_table[PLAY_TOKEN_BLINK_TABLE_SIZE];
	Msg_t tmp_message_display;
	uint8_t play_token_refresh_security = 1;
	uint8_t display_restart;
	uint8_t wait_color_index = 0;

	blink_struct blink_infos;
	blink_infos.blink_count = -1;
	blink_infos.color = COLOR_P_1;
	blink_infos.blink_type = PLAY_TOKEN;
	blink_infos.coords_table = blink_table;
	play_token_blink.callback_param = &blink_infos;
	play_token_blink.Timer_Callback = blink_callback;
	blink_infos.timer = play_token_blink;

	int wait_anim_colors[WAIT_ANIM_COLORS_NUMBER] = { WAIT_COLOR_1,
	WAIT_COLOR_2, WAIT_COLOR_3 };

	while (1) {
		display_restart = 0;
		display_message_init(&tmp_message_display);
		// Wait for first message, stopping idle anim and initializing game
		if (osMessageQueueGet(display_queueHandle, &tmp_message_display, 0U, 0U)
				== osOK) {
			display_init();
			countdown();
			tmp_message_display.move_coordinates.player = 1;
			tmp_message_display.move_coordinates.player_color =
			COLOR_P_1;
			tmp_message_display.move_coordinates.end.c =
			PLAY_TOKEN_STARTING_COL;
			xSemaphoreGive(start_signalHandle);
			while (display_restart == 0) {
				blink_table[0] = 0;
				blink_table[1] = tmp_message_display.move_coordinates.end.c;
				blink_table[2] = BLINK_TABLE_STOP_INT;
				blink_infos.coords_table = blink_table;
				blink_infos.color =
						tmp_message_display.move_coordinates.player_color;
				start_new_timer(&blink_infos.timer, 1);
				//Wait message from writing queue.
				if (osMessageQueueGet(display_queueHandle, &tmp_message_display,
						0U, osWaitForever) == osOK) {
					stop_new_timer(&blink_infos.timer);
					if (tmp_message_display.mode == CONNECT_4) {
						connect_4_message_processing(&tmp_message_display,
								&blink_infos, &display_restart,
								&play_token_refresh_security);
					}
				}
			}
		} else {
			// Idle Animations :
			wait_animation(wait_anim_colors[wait_color_index]);
			reset_matrix_animation();
			wait_color_index += 1;
			if (wait_color_index > 2)
				wait_color_index = 0;
		}
	}
	osThreadDetach(osThreadGetId());
	osThreadExit();
	return 0;
}
