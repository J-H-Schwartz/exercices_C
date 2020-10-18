/*
 * game_p4.h
 *
 *  Created on: Mar 30, 2020
 *      Author: jonathan
 */

#ifndef GAME_P4_H_
#define GAME_P4_H_

#include<stdint.h>

//Dimensions de la matrice.
#define ROWS 6
#define COLS 7

//Joueurs.
#define PLAYER_ONE 1
#define PLAYER_TWO 2
#define NO_ACTIVE_PLAYER 0

//Couleurs des joueurs.
#define COLOR_P_1 GREEN_COLOR
#define COLOR_P_2 RED_COLOR

//Couleur du fond.
#define BACKGROUND_COLOR BLACK_COLOR

#define RED_COLOR 0xFF0000
#define GREEN_COLOR 0x00FF00
#define BLUE_COLOR 0x0000FF
#define WHITE_COLOR 0xFFFFFF
#define PURPLE_COLOR 0xFF00FF
#define YELLOW_COLOR 0xFFFF00
#define BLACK_COLOR 0x000000
#define ORANGE_COLOR 0xFFA500
#define PINK_COLOR 0xFF69B4

// ASCII and OFFSETS regulators.
#define LED_PANEL_OFFSET 1
#define TOP_ROW_OFFSET 1

//Victory type enum.
typedef enum {
	NO_VICTORY, HORIZONTAL, VERTICAL, RIGHT_DIAGONAL, LEFT_DIAGONAL,
} victory_t;

typedef struct {
	int8_t l;
	int8_t c;
} point_t;

//Structure de renvoi de mouvement.
typedef struct {
	int player;
	int player_color;
	point_t beg, end;
} move_t;

typedef struct Msg_t {
	uint8_t mode;
	uint8_t type;
	uint8_t status;
	uint8_t pad;
	uint8_t move_command;
	uint8_t victory_type;
	move_t move_coordinates;
} Msg_t;

typedef enum {
	TDM_APP, TDM_DISPLAY, TDM_TIMER
} Type_De_Message;

// Victory type command enum.
typedef enum {
	LINE_VICTORY, COL_VICTORY, RIGHT_DIAG_VICTORY, LEFT_DIAG_VICTORY, DRAW_END
} Display_victory_type;

//Game mode enum.
typedef enum {
	NUMBERS, CONNECT_4, COUNTDOWN
} Display_mode_command;

//Game Status enum.
typedef enum {
	RESTART, PLAY, GAME_END
} Game_status;

typedef enum {
	LIVE, PLAYER_VICTORY, DRAW
} Game_ends;
//Game move commands enum.
typedef enum {
	DOWN_COMMAND, UP_COMMAND, LEFT_COMMAND, RIGHT_COMMAND, NEXT_PLAYER_COMMAND
} Move_command;

/********************************************************************************/
/* Structure de victoire. 														*/
/* 		int8_t game_end : 0, 1 ou 2. Type de fin de partie.						*/
/* 										\\ 0 -> Partie non terminée.			*/
/* 										\\ 1 -> Victoire de l'un des joueurs. 	*/
/* 										\\ 2 -> Partie nulle.					*/
/*																				*/
/*		int8_t winner_player : 0, 1 ou 2. Joueur victorieux.					*/
/*										\\ 0 -> Pas de vainqueur.				*/
/*										\\ 1 -> Joueur 1 victorieux.			*/
/*										\\ 2 -> Joueur 2 victorieux.			*/
/*																				*/
/* 		int8_t victory_token_coord[2] : Coordonnées du jeton de victoire en		*/
/*										format Ligne - Colonne.					*/
/*																				*/
/*		int8_t victory_line_tokens_coord[4][2] : Quartet de paires de 			*/
/* 													coordonnées des jetons 		*/
/* 													composant la ligne 			*/
/* 													victorieuse en format		*/
/*													Ligne - Colonne.			*/
/*																				*/
/*		int8_t victory_type : 0, 1, 2, 3, ou 4.	Type de victoire.				*/
/*										\\ 0 -> Pas de victoire.				*/
/*										\\ 1 -> Victoire en ligne.				*/
/*										\\ 2 -> Victoire en colonne.			*/
/*										\\ 3 -> Victoire en diagonale droite.	*/
/*										\\ 4 -> Victoire en diagonale gauche.	*/
/********************************************************************************/
typedef struct {
	int8_t game_end;
	int8_t winner_player;
	int8_t victory_token_coord[2];
	int8_t victory_line_tokens_coord[4][2];
	int8_t victory_type;
} victory_infos_t;

void gp4_initialize_message_struct(Msg_t * message);

// Initialise la struct de victoire.
void gp4_initialize_victory_struct(victory_infos_t* victory_infos);

// Initialise la struct de  mouvement.
void gp4_initialize_move_struct(move_t* move);

// Initialise le jeu puissance 4.
void gp4_init(void);

//Affiche la matrice dans la console.
void gp4_display(void);

//Pilotage de la partie.
void gp4_pilot(void);

//Bouger le jeton en jeu vers la gauche.
move_t gp4_move_left(void);

//Bouger le jeton en jeu vers la droite.
move_t gp4_move_right(void);

//Selectionne le joueur actif pour le tour suivant.
move_t gp4_next_player(void);

//Valider la position du jeton.
move_t gp4_play_token(void);

//Vérifie si le jeton joué est gagnant et retourne une structure de type victory_infos_struct.
victory_infos_t gp4_check_winner(void);

//Remplie une ligne afin de pouvoir tester la victoire.
void gp4_debug_set_line(int line, char *RawLine);

#endif /* GAME_P4_H_ */
