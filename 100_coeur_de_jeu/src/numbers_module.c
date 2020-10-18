/*
 * numbers_module.c
 *
 *  Created on: Apr 3, 2020
 *      Author: jonathan
 */


#include<unistd.h>
#include"numbers_module.h"
#include"leds_control.h"
#include"display_thread.h"
#include"game_p4.h"


/************************************************************************************************/
/*									Setnumber													*/
/*	But:																						*/
/*		Fonction qui modifie la couleur d'une led unique.										*/
/*																								*/
/* 	Interface:																					*/
/* 		*matrice[] qui est un tableau de tableaux de structures de type led (struct).		*/
/*		number qui est le numéro à afficher par la matrice, de type integer.			 		*/
/*		color qui est la couleur à appliquer au chiffre, de type led (struct).					*/
/*		background_color qui est la couleur à appliquer au fond, de type led (struct).			*/
/*																								*/
/************************************************************************************************/

void Setnumber(led (*matrix)[7], int number, int color) {
	if (number == 1) {
		for (int row = 0; row < 7; row++) {
			for (int col = 0; col < 7; col++) {
				if ((row == 0 && col == 4) || (row == 1 && col == 3) /*Ensemble de conditions pour afficher le chiffre 1*/
				|| (row == 1 && col == 4) || (row == 2 && col == 2)
						|| (row == 2 && col == 4) || (row == 3 && col == 1)
						|| (row == 3 && col == 4) || (row == 4 && col == 4)
						|| (row == 5 && col == 4) || (row == 6 && col == 4)) {
					SetMatrixTokenColor(&matrix[row][col], color);
				} else {
					SetMatrixTokenColor(&matrix[row][col], BACKGROUND_COLOR);
				}
			}
		}
	} else if (number == 2) {
		for (int row = 0; row < 7; row++) {
			for (int col = 0; col < 7; col++) {
				if ((row == 0 && col == 2) || (row == 0 && col == 3) /*Ensemble de conditions pour afficher le chiffre 2*/
				|| (row == 0 && col == 4) || (row == 1 && col == 1)
						|| (row == 1 && col == 5) || (row == 2 && col == 5)
						|| (row == 3 && col == 2) || (row == 3 && col == 3)
						|| (row == 3 && col == 4) || (row == 4 && col == 1)
						|| (row == 5 && col == 1) || (row == 6 && col == 2)
						|| (row == 6 && col == 3) || (row == 6 && col == 4)
						|| (row == 6 && col == 5)) {
					SetMatrixTokenColor(&matrix[row][col], color);
				} else {
					SetMatrixTokenColor(&matrix[row][col], BACKGROUND_COLOR);
				}
			}
		}
	} else if (number == 3) {
		for (int row = 0; row < 7; row++) {
			for (int col = 0; col < 7; col++) {
				if ((row == 0 && col == 2) || (row == 0 && col == 3) /*Ensemble de conditions pour afficher le chiffre 3*/
				|| (row == 0 && col == 4) || (row == 1 && col == 1)
						|| (row == 1 && col == 5) || (row == 2 && col == 5)
						|| (row == 3 && col == 2) || (row == 3 && col == 3)
						|| (row == 3 && col == 4) || (row == 4 && col == 5)
						|| (row == 5 && col == 5) || (row == 6 && col == 2)
						|| (row == 6 && col == 3) || (row == 6 && col == 4)
						|| (row == 5 && col == 1)) {
					SetMatrixTokenColor(&matrix[row][col], color);
				} else {
					SetMatrixTokenColor(&matrix[row][col], BACKGROUND_COLOR);
				}
			}
		}
	} else if (number == 4) {
		for (int row = 0; row < 7; row++) {
			for (int col = 0; col < 7; col++) {
				if ((row == 0 && col == 4) || (row == 1 && col == 3) /*Ensemble de conditions pour afficher le chiffre 4*/
				|| (row == 1 && col == 4) || (row == 2 && col == 2)
						|| (row == 2 && col == 4) || (row == 3 && col == 1)
						|| (row == 3 && col == 2) || (row == 3 && col == 3)
						|| (row == 3 && col == 4) || (row == 3 && col == 5)
						|| (row == 4 && col == 4) || (row == 5 && col == 4)
						|| (row == 6 && col == 4)) {
					SetMatrixTokenColor(&matrix[row][col], color);
				} else {
					SetMatrixTokenColor(&matrix[row][col], BACKGROUND_COLOR);
				}
			}
		}
	} else if (number == 5) {
		for (int row = 0; row < 7; row++) {
			for (int col = 0; col < 7; col++) {
				if ((row == 0 && col == 1) || (row == 0 && col == 2) /*Ensemble de conditions pour afficher le chiffre 5*/
				|| (row == 0 && col == 3) || (row == 0 && col == 4)
						|| (row == 0 && col == 5) || (row == 1 && col == 1)
						|| (row == 2 && col == 1) || (row == 3 && col == 1)
						|| (row == 3 && col == 2) || (row == 3 && col == 3)
						|| (row == 3 && col == 4) || (row == 4 && col == 5)
						|| (row == 5 && col == 5) || (row == 6 && col == 1)
						|| (row == 6 && col == 2) || (row == 6 && col == 3)
						|| (row == 6 && col == 4)) {
					SetMatrixTokenColor(&matrix[row][col], color);
				} else {
					SetMatrixTokenColor(&matrix[row][col], BACKGROUND_COLOR);
				}
			}
		}
	} else if (number == 6) {
		for (int row = 0; row < 7; row++) {
			for (int col = 0; col < 7; col++) {
				if ((row == 0 && col == 2) || (row == 0 && col == 3) /*Ensemble de conditions pour afficher le chiffre 6*/
				|| (row == 0 && col == 4) || (row == 1 && col == 1)
						|| (row == 2 && col == 1) || (row == 3 && col == 1)
						|| (row == 3 && col == 2) || (row == 3 && col == 3)
						|| (row == 3 && col == 4) || (row == 4 && col == 1)
						|| (row == 4 && col == 5) || (row == 5 && col == 1)
						|| (row == 5 && col == 5) || (row == 6 && col == 2)
						|| (row == 6 && col == 3) || (row == 6 && col == 4)
						|| (row == 1 && col == 5)) {
					SetMatrixTokenColor(&matrix[row][col], color);
				} else {
					SetMatrixTokenColor(&matrix[row][col], BACKGROUND_COLOR);
				}
			}
		}
	} else if (number == 7) {
		for (int row = 0; row < 7; row++) {
			for (int col = 0; col < 7; col++) {
				if ((row == 0 && col == 1) || (row == 0 && col == 2) /*Ensemble de conditions pour afficher le chiffre 7*/
				|| (row == 0 && col == 3) || (row == 0 && col == 4)
						|| (row == 0 && col == 5) || (row == 1 && col == 5)
						|| (row == 2 && col == 5) || (row == 3 && col == 3)
						|| (row == 3 && col == 4) || (row == 3 && col == 5)
						|| (row == 4 && col == 5) || (row == 5 && col == 5)
						|| (row == 6 && col == 5) || (row == 3 && col == 2)) {
					SetMatrixTokenColor(&matrix[row][col], color);
				} else {
					SetMatrixTokenColor(&matrix[row][col], BACKGROUND_COLOR);
				}
			}
		}
	} else if (number == 8) {
		for (int row = 0; row < 7; row++) {
			for (int col = 0; col < 7; col++) {
				if ((row == 0 && col == 2) || (row == 0 && col == 3) /*Ensemble de conditions pour afficher le chiffre 8*/
				|| (row == 0 && col == 4) || (row == 1 && col == 1)
						|| (row == 2 && col == 1) || (row == 2 && col == 5)
						|| (row == 3 && col == 2) || (row == 3 && col == 3)
						|| (row == 3 && col == 4) || (row == 4 && col == 1)
						|| (row == 4 && col == 5) || (row == 5 && col == 1)
						|| (row == 5 && col == 5) || (row == 6 && col == 2)
						|| (row == 6 && col == 3) || (row == 6 && col == 4)
						|| (row == 1 && col == 5)) {
					SetMatrixTokenColor(&matrix[row][col], color);
				} else {
					SetMatrixTokenColor(&matrix[row][col], BACKGROUND_COLOR);
				}
			}
		}
	} else if (number == 9) {
		for (int row = 0; row < 7; row++) {
			for (int col = 0; col < 7; col++) {
				if ((row == 0 && col == 2) || (row == 0 && col == 3) /*Ensemble de conditions pour afficher le chiffre 9*/
				|| (row == 0 && col == 4) || (row == 1 && col == 1)
						|| (row == 2 && col == 1) || (row == 2 && col == 5)
						|| (row == 3 && col == 2) || (row == 3 && col == 3)
						|| (row == 3 && col == 4) || (row == 3 && col == 5)
						|| (row == 4 && col == 5) || (row == 5 && col == 1)
						|| (row == 5 && col == 5) || (row == 6 && col == 2)
						|| (row == 6 && col == 3) || (row == 6 && col == 4)
						|| (row == 1 && col == 5)) {
					SetMatrixTokenColor(&matrix[row][col], color);
				} else {
					SetMatrixTokenColor(&matrix[row][col], BACKGROUND_COLOR);
				}
			}
		}
	} else if (number == 0) {
		for (int row = 0; row < 7; row++) {
			for (int col = 0; col < 7; col++) {
				if ((row == 0 && col == 2) || (row == 0 && col == 3) /*Ensemble de conditions pour afficher le chiffre 0*/
				|| (row == 0 && col == 4) || (row == 1 && col == 1)
						|| (row == 2 && col == 1) || (row == 2 && col == 5)
						|| (row == 3 && col == 5) || (row == 4 && col == 1)
						|| (row == 4 && col == 5) || (row == 5 && col == 1)
						|| (row == 5 && col == 5) || (row == 6 && col == 2)
						|| (row == 6 && col == 3) || (row == 6 && col == 4)
						|| (row == 1 && col == 5) || (row == 3 && col == 1)) {
					SetMatrixTokenColor(&matrix[row][col], color);
				} else {
					SetMatrixTokenColor(&matrix[row][col], BACKGROUND_COLOR);
				}
			}
		}
	}
}



/************************************************************************************************/
/*									SetFlashingSmiley											*/
/*	But:																						*/
/*		Fonction qui fait clignoter un smiley avec les couleurs choisies pendant le temps voulu.*/
/*																								*/
/* 	Interface:																					*/
/* 		*matrice[] qui est un tableau de tableaux de structures de type led (struct).			*/
/*		smiley_color1 qui est la couleur à appliquer au smiley, de type led (struct) en cycle 1.*/
/*		smiley_color2 qui est la couleur à appliquer au fond, de type led (struct) en cycle 2.	*/
/*		timer qui est le nombre de flashs avant arrêt de type integer.							*/
/*																								*/
/************************************************************************************************/

void SetFlashingSmiley(led (*matrix)[7], int color,
		int timer) {
	while (timer > 0) {
		for (int row = 0; row < 7; row++) {
			for (int col = 0; col < 7; col++) {
				if ((row == 0 && col == 1) || (row == 0 && col == 2) /*Conditions pour remplissage du smiley cycle 1*/
				|| (row == 0 && col == 3) || (row == 0 && col == 4)
						|| (row == 0 && col == 0) || (row == 0 && col == 6)
						|| (row == 0 && col == 5) || (row == 1 && col == 0)
						|| (row == 1 && col == 2) || (row == 1 && col == 3)
						|| (row == 1 && col == 4) || (row == 1 && col == 6)
						|| (row == 2 && col == 0) || (row == 2 && col == 3)
						|| (row == 2 && col == 6) || (row == 3 && col == 0)
						|| (row == 3 && col == 1) || (row == 3 && col == 2)
						|| (row == 3 && col == 3) || (row == 3 && col == 4)
						|| (row == 3 && col == 5) || (row == 3 && col == 6)
						|| (row == 4 && col == 0) || (row == 4 && col == 6)
						|| (row == 5 && col == 0) || (row == 5 && col == 1)
						|| (row == 5 && col == 5) || (row == 5 && col == 6)
						|| (row == 6 && col == 0) || (row == 6 && col == 1)
						|| (row == 6 && col == 2) || (row == 6 && col == 3)
						|| (row == 6 && col == 4) || (row == 6 && col == 5)
						|| (row == 6 && col == 6)) {
					SetMatrixTokenColor(&matrix[row][col], color);
				} else {
					SetMatrixTokenColor(&matrix[row][col], BACKGROUND_COLOR);
				}
			}
		}
		sleep(4);
		for (int row = 0; row < 7; row++) {
			for (int col = 0; col < 7; col++) {
				if ((row == 1 && col == 0) || (row == 0 && col == 2) /*Conditions pour remplissage du smiley cycle 2*/
				|| (row == 0 && col == 3) || (row == 0 && col == 4)
						|| (row == 0 && col == 0) || (row == 0 && col == 6)
						|| (row == 1 && col == 3) || (row == 1 && col == 6)
						|| (row == 2 && col == 0) || (row == 2 && col == 3)
						|| (row == 2 && col == 6) || (row == 3 && col == 0)
						|| (row == 3 && col == 1) || (row == 3 && col == 2)
						|| (row == 3 && col == 3) || (row == 3 && col == 4)
						|| (row == 3 && col == 5) || (row == 3 && col == 6)
						|| (row == 4 && col == 0) || (row == 4 && col == 6)
						|| (row == 5 && col == 0) || (row == 5 && col == 6)
						|| (row == 6 && col == 0) || (row == 6 && col == 1)
						|| (row == 6 && col == 5) || (row == 6 && col == 6)
						|| (row == 0 && col == 5) || (row == 1 && col == 2)
						|| (row == 1 && col == 4) || (row == 0 && col == 1)) {
					SetMatrixTokenColor(&matrix[row][col], BACKGROUND_COLOR);
				} else if ((row == 5 && col == 2) || (row == 5 && col == 3)
						|| (row == 5 && col == 4)) {
					SetMatrixTokenColor(&matrix[row][col], RED_COLOR);
				} else {
					SetMatrixTokenColor(&matrix[row][col], color);
				}
			}
		}
		timer--;
		sleep(4);
	}
}

