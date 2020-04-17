/*
 * game_p4.h
 *
 *  Created on: Mar 30, 2020
 *      Author: jonathan
 */

#ifndef GAME_P4_H_
#define GAME_P4_H_


#include<stdint.h>

typedef struct { /*Définition du nouveau type led qui contient les valeurs des couleurs*/
	int RValue; /*Rouge, Verte, et Bleue et qui est une struct.						  */
	int GValue;
	int BValue;
} led;

typedef enum {
	live, //!< Need to continue game
	stop_winner, //!< game is finish and winner
	stop_equal //<! game is finish and equal
} status_t;

//Victory type enum.
typedef enum {
	NO_VICTORY,
	HORIZONTAL,
	VERTICAL,
	RIGHT_DIAGONAL,
	LEFT_DIAGONAL,
} victory_t;

typedef enum {
	NO_PLAYER,
	PLAYER_1,
	PLAYER_2
} player_t;

typedef struct {
	int8_t l;
	int8_t c;
} point_t;

typedef struct {
	status_t status; //!< status of winner
	victory_t win_type; //<!
	player_t win_player; //<! the winner player
	point_t win_position[4]; //!< position of 4 winner token
} winner_t;


//Structure de renvoi de mouvement.
typedef struct {
	int player;
	int player_color;
	point_t beg, end;
}move_t;

//Dimensions de la matrice.
#define ROWS 6
#define COLS 7

//Joueurs.
#define PLAYER_ONE 1
#define PLAYER_TWO 2
#define NO_ACTIVE_PLAYER 0

//Couleurs des joueurs.
#define COLOR_P_1 GREEN
#define COLOR_P_2 RED

//Couleur du fond.
#define BACKGROUND_COLOR BLACK

//Couleurs
#define COLOR_ON 255
#define COLOR_OFF 0
#define RED_COLOR 0xFF0000
#define GREEN_COLOR 0x00FF00
#define BLUE_COLOR 0x0000FF
#define WHITE_COLOR 0xFFFFFF
#define PURPLE_COLOR 0xFF00FF
#define YELLOW_COLOR 0xFFFF00
#define BLACK_COLOR 0x000000

typedef enum colors{
	RED, GREEN, BLUE, WHITE, PURPLE, YELLOW, BLACK
}colors_enum;

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
/*										\\ 3 -> Victoire en diagonale Sud-Est.	*/
/*										\\ 4 -> Victoire en diagonale Sud-ouest.*/
/********************************************************************************/
typedef struct {
	int8_t game_end;
	int8_t winner_player;
	int8_t victory_token_coord[2];
	int8_t victory_line_tokens_coord[4][2];
	int8_t victory_type;
} victory_infos_struct;

#define VICTORY 1
#define GAMEOVER 2

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
victory_infos_struct gp4_check_winner(void);

//Remplie une ligne afin de pouvoir tester la victoire.
void gp4_debug_set_line(int line, char *RawLine);

#endif /* GAME_P4_H_ */
