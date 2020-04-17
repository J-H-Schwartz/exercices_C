/*
 * game_ttt.c
 *
 *  Created on: Apr 15, 2020
 *      Author: jonathan
 */
#include<stdio.h>
#include"game_ttt.h"

typedef struct token_coord_t{
	int row, col;
}token_coord_t;

int matrix[MATRIX_SIZE][MATRIX_SIZE];
token_coord_t token_pos;

void ttt_init(void){
	for (int row = 0; row < MATRIX_SIZE; row++){
		for (int col = 0; col < MATRIX_SIZE; col++){
			matrix[row][col] = 0;
		}
	}
	token_pos.row = 0;
	token_pos.col = 0;
}

void ttt_move_side(void){
	int row = token_pos.row;
	for (int col = token_pos.col; col <= MATRIX_SIZE; col++) {
		if (col == MATRIX_SIZE) {
			col = 0;
			row += 1;
			if (row == MATRIX_SIZE)
				row = 0;
		}
		if (matrix[row][col] == 0) {
			token_pos.row = row;
			token_pos.col = col;
			return;
		}
	}
}

void ttt_move_vert(void){
	int col = token_pos.col;
	for (int row = token_pos.row; row <= MATRIX_SIZE; row++) {
		if (row == MATRIX_SIZE) {
			row = 0;
			col += 1;
			if (col == MATRIX_SIZE)
				col = 0;
		}
		if (matrix[row][col] == 0) {
			token_pos.row = row;
			token_pos.col = col;
			return;
		}
	}
}
