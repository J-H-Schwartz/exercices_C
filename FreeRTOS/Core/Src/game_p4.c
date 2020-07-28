/*
 * game_p4.c
 *
 *  Created on: Mar 30, 2020
 *      Author: jonathan
 */
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>


#include"game_p4.h"

//Tableau matrice.
static int8_t gp4_matrix[ROWS][COLS];

//Colonne du jeton en jeu.
static int8_t token_top_selector = 3;
static int8_t last_played_row = 0;

static move_t move;

//Variables joueur actif.
static int8_t active_player = NO_ACTIVE_PLAYER;
static int active_player_color = COLOR_P_1;

//Couleurs des joueurs.
static int color_player_one;
static int color_player_two;

//Couleur du fond.
static int color_background;

//Pilotage de la partie.
void gp4_pilot(void);

void gp4_initialize_message_struct(Msg_t * message) {
	message->mode = 0;
	message->type = 0;
	message->status = 0;
	message->pad = 0;
	message->move_command = 0;
	message->move_coordinates.player = 0;
	message->move_coordinates.player_color = 0;
	message->move_coordinates.beg.l = 0;
	message->move_coordinates.beg.c = 0;
	message->move_coordinates.end.l = 0;
	message->move_coordinates.end.c = 0;
}

// Victory struct initialization.
void gp4_initialize_victory_struct(victory_infos_t* victory_infos) {
	victory_infos->game_end = 0;
	victory_infos->winner_player = 0;
	victory_infos->victory_token_coord[0] = 0;
	victory_infos->victory_token_coord[1] = 0;
	victory_infos->victory_line_tokens_coord[0][0] = 0;
	victory_infos->victory_line_tokens_coord[0][1] = 0;
	victory_infos->victory_line_tokens_coord[1][0] = 0;
	victory_infos->victory_line_tokens_coord[1][1] = 0;
	victory_infos->victory_line_tokens_coord[2][0] = 0;
	victory_infos->victory_line_tokens_coord[2][1] = 0;
	victory_infos->victory_line_tokens_coord[3][0] = 0;
	victory_infos->victory_line_tokens_coord[3][1] = 0;
	victory_infos->victory_type = 0;
}

void gp4_initialize_move_struct(move_t* move) {
	move->player = PLAYER_ONE;
	move->player_color = COLOR_P_1;
	move->beg.c = 0;
	move->beg.l = 0;
	move->end.c = 0;
	move->end.l = 0;
}

//Initialise le jeu puissance 4.
void gp4_init(void) {
	//Mise en place de la matrice 7x7
	for (int8_t row = 0; row < ROWS; row++) {
		for (int8_t col = 0; col < COLS; col++) {
			gp4_matrix[row][col] = 0;
		}
	}
	token_top_selector = 3;
	color_player_one = COLOR_P_1;
	color_player_two = COLOR_P_2;
	color_background = BACKGROUND_COLOR;
	active_player = PLAYER_ONE; //Temporaire.
	active_player_color = COLOR_P_1;
	gp4_initialize_move_struct(&move);
}

//Affiche la matrice dans la console.
//void gp4_display(void) {
//	char top_line[80 + 1];
//	char tmp_add[10 + 1];
//	snprintf(top_line, 80, "  TOP   = ");
//	for (int8_t col = 0; col < COLS; col++) {
//		if (col == token_top_selector)
//			snprintf(tmp_add, 10, "%d ", active_player);
//		else
//			snprintf(tmp_add, 10, "0 ");
//		strcat(top_line, tmp_add);
//	}
//	snprintf(tmp_add, 10, "(%d)", active_player_color);
//	strcat(top_line, tmp_add);
//	debug_printf(1, "%s\n", top_line);
//	for (int8_t row = 0; row < ROWS; row++) {
//		debug_printf(1, " Line %d = %d %d %d %d %d %d %d\n", (row + 1),
//				gp4_matrix[row][0], gp4_matrix[row][1], gp4_matrix[row][2], gp4_matrix[row][3],
//				gp4_matrix[row][4], gp4_matrix[row][5], gp4_matrix[row][6]);
//	}
//	debug_printf(1, "\n");
//}

//Bouger le jeton en jeu vers la gauche.
move_t gp4_move_left(void) {
	for (int8_t col = (token_top_selector - 1); col >= -1; col--) {
		if (col == -1)
			col = COLS - 1;
		if (gp4_matrix[0][col] != 0) {
			continue;
		} else {
			move.beg.c = token_top_selector;
			move.beg.l = 0;
			move.end.c = col;
			move.end.l = 0;
			token_top_selector = col;
			return move;
		}
	}
	move.beg.c = 0;
	move.beg.l = 0;
	move.end.c = 0;
	move.end.l = 0;
	token_top_selector = 0;
	return move;
}

//Bouger le jeton en jeu vers la droite.
move_t gp4_move_right(void) {
	for (int8_t col = (token_top_selector + 1); col <= COLS; col++) {
		if (col == COLS)
			col = 0;
		if (gp4_matrix[0][col] != 0) {
			continue;
		} else {
			move.beg.c = token_top_selector;
			move.beg.l = 0;
			move.end.c = col;
			move.end.l = 0;
			token_top_selector = col;
			return move;
		}
	}
	move.beg.c = 0;
	move.beg.l = 0;
	move.end.c = 0;
	move.end.l = 0;
	token_top_selector = 0;
	return move;
}

//Selectionne le joueur actif pour le tour suivant.
move_t gp4_next_player(void) {
	if (active_player == PLAYER_ONE) {
		active_player = PLAYER_TWO;
		move.player = PLAYER_TWO;
		active_player_color = COLOR_P_2;
		move.player_color = COLOR_P_2;
	} else {
		active_player = PLAYER_ONE;
		move.player = PLAYER_ONE;
		active_player_color = COLOR_P_1;
		move.player_color = COLOR_P_1;
	}
	for (int8_t col = 3; col <= COLS; col++) {
		if (col == COLS)
			col = 0;
		if (gp4_matrix[0][col] == 0) {
			move.beg.c = col;
			move.beg.l = 0;
			move.end.c = col;
			move.end.l = 0;
			token_top_selector = col;
			return move;
		}
	}
	move.beg.c = 0;
	move.beg.l = 0;
	move.end.c = 0;
	move.end.l = 0;
	token_top_selector = 0;
	return move;
}

//Validation jeton.
move_t gp4_play_token(void) {
	for (int8_t row = 0; row < ROWS; row++) {
		if (gp4_matrix[0][token_top_selector] != 0) {
			move.beg.c = token_top_selector;
			move.beg.l = 0;
			move.end.c = token_top_selector;
			move.end.l = 0;
			return move;
		} else if (gp4_matrix[row][token_top_selector] != 0) {
			move.beg.c = token_top_selector;
			move.beg.l = 0;
			move.end.c = token_top_selector;
			move.end.l = row;
			gp4_matrix[row - 1][token_top_selector] = active_player;
			last_played_row = row - 1;
			return move;
		}
	}
	move.beg.c = token_top_selector;
	move.beg.l = 0;
	move.end.c = token_top_selector;
	move.end.l = ROWS;
	gp4_matrix[ROWS - 1][token_top_selector] = active_player;
	last_played_row = ROWS - 1;
	return move;
}

//Vérification de victoire.
victory_infos_t gp4_check_winner(void) {
	victory_infos_t victory_infos;
	gp4_initialize_victory_struct(&victory_infos);
	//Vérifie en ligne.
	for (int8_t col = token_top_selector - 3; col <= token_top_selector;
			col++) {
		if (col < 0)
			continue;
		else if ((col + 3) > (COLS - 1))
			break;
		else if (gp4_matrix[last_played_row][col] == active_player
				&& gp4_matrix[last_played_row][col + 1] == active_player
				&& gp4_matrix[last_played_row][col + 2] == active_player
				&& gp4_matrix[last_played_row][col + 3] == active_player) {
			victory_infos.game_end = PLAYER_VICTORY;
			victory_infos.winner_player = active_player;
			victory_infos.victory_token_coord[0] = last_played_row;
			victory_infos.victory_token_coord[1] = token_top_selector;
			victory_infos.victory_line_tokens_coord[0][0] = last_played_row;
			victory_infos.victory_line_tokens_coord[0][1] = col;
			victory_infos.victory_line_tokens_coord[1][0] = last_played_row;
			victory_infos.victory_line_tokens_coord[1][1] = (col + 1);
			victory_infos.victory_line_tokens_coord[2][0] = last_played_row;
			victory_infos.victory_line_tokens_coord[2][1] = (col + 2);
			victory_infos.victory_line_tokens_coord[3][0] = last_played_row;
			victory_infos.victory_line_tokens_coord[3][1] = (col + 3);
			victory_infos.victory_type = HORIZONTAL;
			return victory_infos;
		}
	}
	//Vérifie en colonne.
	for (int8_t row = last_played_row - 3; row <= last_played_row; row++) {
		if (row < 0)
			continue;
		else if ((row + 3) > (ROWS - 1))
			break;
		else if (gp4_matrix[row][token_top_selector] == active_player
				&& gp4_matrix[row + 1][token_top_selector] == active_player
				&& gp4_matrix[row + 2][token_top_selector] == active_player
				&& gp4_matrix[row + 3][token_top_selector] == active_player) {
			victory_infos.game_end = PLAYER_VICTORY;
			victory_infos.winner_player = active_player;
			victory_infos.victory_token_coord[0] = last_played_row;
			victory_infos.victory_token_coord[1] = token_top_selector;
			victory_infos.victory_line_tokens_coord[0][0] = row;
			victory_infos.victory_line_tokens_coord[0][1] = token_top_selector;
			victory_infos.victory_line_tokens_coord[1][0] = (row + 1);
			victory_infos.victory_line_tokens_coord[1][1] = token_top_selector;
			victory_infos.victory_line_tokens_coord[2][0] = (row + 2);
			victory_infos.victory_line_tokens_coord[2][1] = token_top_selector;
			victory_infos.victory_line_tokens_coord[3][0] = (row + 3);
			victory_infos.victory_line_tokens_coord[3][1] = token_top_selector;
			victory_infos.victory_type = VERTICAL;
			return victory_infos;
		}
	}
	//Vérifie en diagonale Haut->Bas/Gauche->Droite
	int8_t col = token_top_selector - 3;
	for (int8_t row = last_played_row - 3;
			row <= last_played_row && col <= token_top_selector; row++) {
		if (row < 0 || col < 0) {
			col += 1;
			continue;
		} else if ((row + 3) > (ROWS - 1) || (col + 3) > (COLS - 1)) {
			break;
		} else if (gp4_matrix[row][col] == active_player
				&& gp4_matrix[row + 1][col + 1] == active_player
				&& gp4_matrix[row + 2][col + 2] == active_player
				&& gp4_matrix[row + 3][col + 3] == active_player) {
			victory_infos.game_end = PLAYER_VICTORY;
			victory_infos.winner_player = active_player;
			victory_infos.victory_token_coord[0] = last_played_row;
			victory_infos.victory_token_coord[1] = token_top_selector;
			victory_infos.victory_line_tokens_coord[0][0] = row;
			victory_infos.victory_line_tokens_coord[0][1] = col;
			victory_infos.victory_line_tokens_coord[1][0] = (row + 1);
			victory_infos.victory_line_tokens_coord[1][1] = (col + 1);
			victory_infos.victory_line_tokens_coord[2][0] = (row + 2);
			victory_infos.victory_line_tokens_coord[2][1] = (col + 2);
			victory_infos.victory_line_tokens_coord[3][0] = (row + 3);
			victory_infos.victory_line_tokens_coord[3][1] = (col + 3);
			victory_infos.victory_type = RIGHT_DIAGONAL;
			return victory_infos;
		}
		col += 1;
	}
	//Vérifie en diagonale Bas->Haut/Gauche->Droite
	col = token_top_selector - 3;
	for (int8_t row = last_played_row + 3;
			row >= last_played_row && col <= token_top_selector; row--) {
		if (row > 6 || col < 0) {
			col += 1;
			continue;
		} else if ((row - 3) < 0 || (col + 3) > (COLS - 1)) {
			break;
		} else if (gp4_matrix[row][col] == active_player
				&& gp4_matrix[row - 1][col + 1] == active_player
				&& gp4_matrix[row - 2][col + 2] == active_player
				&& gp4_matrix[row - 3][col + 3] == active_player) {
			victory_infos.game_end = PLAYER_VICTORY;
			victory_infos.winner_player = active_player;
			victory_infos.victory_token_coord[0] = last_played_row;
			victory_infos.victory_token_coord[1] = token_top_selector;
			victory_infos.victory_line_tokens_coord[0][0] = row;
			victory_infos.victory_line_tokens_coord[0][1] = col;
			victory_infos.victory_line_tokens_coord[1][0] = (row - 1);
			victory_infos.victory_line_tokens_coord[1][1] = (col + 1);
			victory_infos.victory_line_tokens_coord[2][0] = (row - 2);
			victory_infos.victory_line_tokens_coord[2][1] = (col + 2);
			victory_infos.victory_line_tokens_coord[3][0] = (row - 3);
			victory_infos.victory_line_tokens_coord[3][1] = (col + 3);
			victory_infos.victory_type = LEFT_DIAGONAL;
			return victory_infos;
		}
		col += 1;
	}
	for (int8_t col = 0; col < COLS; col++) {
		if (gp4_matrix[0][col] == 0) {
			return victory_infos;
		}
	}
	victory_infos.game_end = DRAW;
	return victory_infos;
}

void gp4_debug_set_line(int line, char *RawLine) {
	line = line - 1;
	gp4_matrix[line][0] = RawLine[0] - 48;
	gp4_matrix[line][1] = RawLine[1] - 48;
	gp4_matrix[line][2] = RawLine[2] - 48;
	gp4_matrix[line][3] = RawLine[3] - 48;
	gp4_matrix[line][4] = RawLine[4] - 48;
	gp4_matrix[line][5] = RawLine[5] - 48;
	gp4_matrix[line][6] = RawLine[6] - 48;
}
