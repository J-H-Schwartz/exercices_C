/*
 * game_p4.c
 *
 *  Created on: Mar 30, 2020
 *      Author: jonathan
 */
#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>
#include"debug.h"
#include"game_p4.h"

//Tableau matrice.
static int8_t matrix[ROWS][COLS];

//Colonne du jeton en jeu.
static int8_t token_top_selector = 0;
static int8_t last_played_row = 0;

static move_t move;

//Structure de victoire.
static victory_infos_struct victory_infos;

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

//Initialise le jeu puissance 4.
void gp4_init(void) {
	//Mise en place de la matrice 7x7
	for (int8_t row = 0; row < ROWS; row++) {
		for (int8_t col = 0; col < COLS; col++) {
			matrix[col][row] = 0;
		}
	}
	color_player_one = COLOR_P_1;
	color_player_two = COLOR_P_2;
	color_background = BACKGROUND_COLOR;
	active_player = PLAYER_ONE; //Temporaire.
	active_player_color = COLOR_P_1;
	move.player = PLAYER_ONE;
	move.player_color = COLOR_P_1;
	move.beg.c = 0;
	move.beg.l = 0;
	move.end.c = 0;
	move.end.l = 0;

	//Mise à zero des variables de structure de victoire.
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
	victory_infos.victory_type = NO_VICTORY;
}

//Affiche la matrice dans la console.
void gp4_display(void) {
	char top_line[80 + 1];
	char tmp_add[10 + 1];
	snprintf(top_line, 80, "  TOP   = ");
	for (int8_t col = 0; col < COLS; col++) {
		if (col == token_top_selector)
			snprintf(tmp_add, 10, "%d ", active_player);
		else
			snprintf(tmp_add, 10, "0 ");
		strcat(top_line, tmp_add);
	}
	snprintf(tmp_add, 10, "(%d)", active_player_color);
	strcat(top_line, tmp_add);
	debug_printf(2, "%s\n", top_line);
	for (int8_t row = 0; row < ROWS; row++) {
		debug_printf(2, " Line %d = %d %d %d %d %d %d %d\n", (row + 1),
				matrix[row][0], matrix[row][1], matrix[row][2], matrix[row][3],
				matrix[row][4], matrix[row][5], matrix[row][6]);
	}
}

//Bouger le jeton en jeu vers la gauche.
move_t gp4_move_left(void) {
	for (int8_t col = (token_top_selector - 1); col >= -1; col--) {
		if (col == -1)
			col = COLS - 1;
		if (matrix[0][col] != 0) {
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
		if (matrix[0][col] != 0) {
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
		if (matrix[0][col] == 0) {
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
		if (matrix[0][token_top_selector] != 0){
			move.beg.c = token_top_selector;
			move.beg.l = 0;
			move.end.c = token_top_selector;
			move.end.l = 0;
			return move;
		}
		else if (matrix[row][token_top_selector] != 0) {
			move.beg.c = token_top_selector;
			move.beg.l = 0;
			move.end.c = token_top_selector;
			move.end.l = row;
			matrix[row - 1][token_top_selector] = active_player;
			last_played_row = row - 1;
			return move;
		}
	}
	move.beg.c = token_top_selector;
	move.beg.l = 0;
	move.end.c = token_top_selector;
	move.end.l = ROWS;
	matrix[ROWS - 1][token_top_selector] = active_player;
	last_played_row = ROWS - 1;
	return move;
}

//Vérification de victoire.
victory_infos_struct gp4_check_winner(void) {
	//Vérifie en ligne.
	for (int8_t col = token_top_selector - 3; col <= token_top_selector;
			col++) {
		if (col < 0)
			continue;
		else if ((col + 3) > (COLS - 1))
			break;
		else if (matrix[last_played_row][col] == active_player
				&& matrix[last_played_row][col + 1] == active_player
				&& matrix[last_played_row][col + 2] == active_player
				&& matrix[last_played_row][col + 3] == active_player) {
			victory_infos.game_end = VICTORY;
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
		else if (matrix[row][token_top_selector] == active_player
				&& matrix[row + 1][token_top_selector] == active_player
				&& matrix[row + 2][token_top_selector] == active_player
				&& matrix[row + 3][token_top_selector] == active_player) {
			victory_infos.game_end = VICTORY;
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
		} else if (matrix[row][col] == active_player
				&& matrix[row + 1][col + 1] == active_player
				&& matrix[row + 2][col + 2] == active_player
				&& matrix[row + 3][col + 3] == active_player) {
			victory_infos.game_end = VICTORY;
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
		} else if (matrix[row][col] == active_player
				&& matrix[row - 1][col + 1] == active_player
				&& matrix[row - 2][col + 2] == active_player
				&& matrix[row - 3][col + 3] == active_player) {
			victory_infos.game_end = VICTORY;
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
		if (matrix[0][col] == 0) {
			return victory_infos;
		}
	}
	victory_infos.game_end = GAMEOVER;
	return victory_infos;
}

void gp4_debug_set_line(int line, char *RawLine) {
	line = line - 1;
	matrix[line][0] = RawLine[0] - 48;
	matrix[line][1] = RawLine[1] - 48;
	matrix[line][2] = RawLine[2] - 48;
	matrix[line][3] = RawLine[3] - 48;
	matrix[line][4] = RawLine[4] - 48;
	matrix[line][5] = RawLine[5] - 48;
	matrix[line][6] = RawLine[6] - 48;
}
