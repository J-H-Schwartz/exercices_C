/*
 * numbers_module.c
 *
 *  Created on: Apr 3, 2020
 *      Author: jonathan
 */


#include<unistd.h>
#include"numbers_module.h"
#include"leds_control.h"
#define COLOR_ON 255
#define COLOR_OFF 0
#define RED 0
#define GREEN 1
#define BLUE 2
#define WHITE 3
#define PURPLE 4
#define YELLOW 5
#define BLACK 6



/************************************************************************************************/
/*								Initialisation de la matrice.									*/
/*	line_x:																						*/
/*		Initialisation en noir de chaque led de la ligne x (x à remplacer par le numéro de ligne*/
/*			par le numéro de ligne.																*/
/*																								*/
/* 	*led_matrice:																				*/
/* 		Initialisation du tableau de tableaux de structure représentant la matrice.				*/
/*																						 		*/
/*	colors:																						*/
/*		Initialisation du tableau de structs comprennant les différentes couleurs possibles.	*/
/*																								*/
/************************************************************************************************/

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

static led line_1_2[7] = { { COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF,
COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF,
COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF } };
static led line_2_2[7] = { { COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF,
COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF,
COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF } };
static led line_3_2[7] = { { COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF,
COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF,
COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF } };
static led line_4_2[7] = { { COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF,
COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF,
COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF } };
static led line_5_2[7] = { { COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF,
COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF,
COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF } };
static led line_6_2[7] = { { COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF,
COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF,
COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF } };
static led line_7_2[7] = { { COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF,
COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF }, {
COLOR_OFF, COLOR_OFF,
COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF } };
static led *black_matrice[7] = { line_1_2, line_2_2, line_3_2, line_4_2, line_5_2, line_6_2,
		line_7_2 };


static led colors[7] = { { COLOR_ON, COLOR_OFF, COLOR_OFF }, { COLOR_OFF,
COLOR_ON,
COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_ON }, { COLOR_ON,
COLOR_ON, COLOR_ON }, { COLOR_ON, COLOR_OFF, COLOR_ON }, { COLOR_ON,
COLOR_ON, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF } };

led *old_matrice [7]= { line_1_1, line_2_1, line_3_1, line_4_1, line_5_1, line_6_1,
		line_7_1 };


void SetBlackMatrice(void){
	for (int row = 0; row < 7; row++) {
		for (int col = 0; col < 7; col++){
			if ((black_matrice[row][col].RValue != old_matrice[row][col].RValue) || (black_matrice[row][col].GValue != old_matrice[row][col].GValue) || (black_matrice[row][col].BValue != old_matrice[row][col].BValue)){
				setLedColor((row + 1), (col + 1), black_matrice[row][col].RValue,
						black_matrice[row][col].GValue, black_matrice[row][col].BValue);
			}
		}
	}
}

void SetMatriceBack(void){
	for (int row = 0; row < 7; row++) {
		for (int col = 0; col < 7; col++){
			if ((black_matrice[row][col].RValue != old_matrice[row][col].RValue) || (black_matrice[row][col].GValue != old_matrice[row][col].GValue) || (black_matrice[row][col].BValue != old_matrice[row][col].BValue)){
				setLedColor((row + 1), (col + 1), old_matrice[row][col].RValue,
						old_matrice[row][col].GValue, old_matrice[row][col].BValue);
			}
		}
	}
}

/************************************************************************************************/
/*									SetLedMatrice												*/
/*	But:																						*/
/*		Fonction qui envoie la couleur des leds une par une sur la totalité de la matrice.		*/
/*																								*/
/* 	Interface:																					*/
/* 		*leds_table[] qui est un tableau de tableaux de structures de type led (struct).		*/
/*		Tabsize qui est la taille du tableau de tableaux et de type integer.					*/
/*																								*/
/************************************************************************************************/

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

void Setnumber(int number) {
	led color = colors[RED];
	led background_color = colors[BLACK];
	if (number == 1) {
		for (int row = 0; row < 7; row++) {
			for (int col = 0; col < 7; col++) {
				if ((row == 0 && col == 4) || (row == 1 && col == 3) /*Ensemble de conditions pour afficher le chiffre 1*/
				|| (row == 1 && col == 4) || (row == 2 && col == 2)
						|| (row == 2 && col == 4) || (row == 3 && col == 1)
						|| (row == 3 && col == 4) || (row == 4 && col == 4)
						|| (row == 5 && col == 4) || (row == 6 && col == 4)) {
					matrice[row][col].RValue = color.RValue;
					matrice[row][col].GValue = color.GValue;
					matrice[row][col].BValue = color.BValue;
				} else {
					matrice[row][col].RValue = background_color.RValue;
					matrice[row][col].GValue = background_color.GValue;
					matrice[row][col].BValue = background_color.BValue;
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
					matrice[row][col].RValue = color.RValue;
					matrice[row][col].GValue = color.GValue;
					matrice[row][col].BValue = color.BValue;
				} else {
					matrice[row][col].RValue = background_color.RValue;
					matrice[row][col].GValue = background_color.GValue;
					matrice[row][col].BValue = background_color.BValue;
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
					matrice[row][col].RValue = color.RValue;
					matrice[row][col].GValue = color.GValue;
					matrice[row][col].BValue = color.BValue;
				} else {
					matrice[row][col].RValue = background_color.RValue;
					matrice[row][col].GValue = background_color.GValue;
					matrice[row][col].BValue = background_color.BValue;
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
					matrice[row][col].RValue = color.RValue;
					matrice[row][col].GValue = color.GValue;
					matrice[row][col].BValue = color.BValue;
				} else {
					matrice[row][col].RValue = background_color.RValue;
					matrice[row][col].GValue = background_color.GValue;
					matrice[row][col].BValue = background_color.BValue;
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
					matrice[row][col].RValue = color.RValue;
					matrice[row][col].GValue = color.GValue;
					matrice[row][col].BValue = color.BValue;
				} else {
					matrice[row][col].RValue = background_color.RValue;
					matrice[row][col].GValue = background_color.GValue;
					matrice[row][col].BValue = background_color.BValue;
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
					matrice[row][col].RValue = color.RValue;
					matrice[row][col].GValue = color.GValue;
					matrice[row][col].BValue = color.BValue;
				} else {
					matrice[row][col].RValue = background_color.RValue;
					matrice[row][col].GValue = background_color.GValue;
					matrice[row][col].BValue = background_color.BValue;
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
					matrice[row][col].RValue = color.RValue;
					matrice[row][col].GValue = color.GValue;
					matrice[row][col].BValue = color.BValue;
				} else {
					matrice[row][col].RValue = background_color.RValue;
					matrice[row][col].GValue = background_color.GValue;
					matrice[row][col].BValue = background_color.BValue;
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
					matrice[row][col].RValue = color.RValue;
					matrice[row][col].GValue = color.GValue;
					matrice[row][col].BValue = color.BValue;
				} else {
					matrice[row][col].RValue = background_color.RValue;
					matrice[row][col].GValue = background_color.GValue;
					matrice[row][col].BValue = background_color.BValue;
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
					matrice[row][col].RValue = color.RValue;
					matrice[row][col].GValue = color.GValue;
					matrice[row][col].BValue = color.BValue;
				} else {
					matrice[row][col].RValue = background_color.RValue;
					matrice[row][col].GValue = background_color.GValue;
					matrice[row][col].BValue = background_color.BValue;
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
					matrice[row][col].RValue = color.RValue;
					matrice[row][col].GValue = color.GValue;
					matrice[row][col].BValue = color.BValue;
				} else {
					matrice[row][col].RValue = background_color.RValue;
					matrice[row][col].GValue = background_color.GValue;
					matrice[row][col].BValue = background_color.BValue;
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

void SetFlashingSmiley(led smiley_color1, led smiley_color2,
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
					matrice[row][col].RValue = smiley_color1.RValue;
					matrice[row][col].GValue = smiley_color1.GValue;
					matrice[row][col].BValue = smiley_color1.BValue;
				} else {
					matrice[row][col].RValue = smiley_color2.RValue;
					matrice[row][col].GValue = smiley_color2.GValue;
					matrice[row][col].BValue = smiley_color2.BValue;
				}
			}
		}
		SetLedMatrice();
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
					matrice[row][col].RValue = smiley_color2.RValue;
					matrice[row][col].GValue = smiley_color2.GValue;
					matrice[row][col].BValue = smiley_color2.BValue;
				} else if ((row == 5 && col == 2) || (row == 5 && col == 3)
						|| (row == 5 && col == 4)) {
					matrice[row][col].RValue = colors[RED].RValue;
					matrice[row][col].GValue = colors[RED].GValue;
					matrice[row][col].BValue = colors[RED].BValue;
				} else {
					matrice[row][col].RValue = smiley_color1.RValue;
					matrice[row][col].GValue = smiley_color1.GValue;
					matrice[row][col].BValue = smiley_color1.BValue;
				}
			}
		}
		SetLedMatrice();
		timer--;
		sleep(4);
	}
}


/************************************************************************************************/
/*									countdown													*/
/*	But:																						*/
/*		Fonction qui affiche un décompte de 9 à 0 avec une couleur variable.					*/
/*																								*/
/* 	Interface:																					*/
/* 		*matrice[] qui est un tableau de tableaux de structures de type led (struct).			*/
/*																								*/
/************************************************************************************************/

void countdown(led *matrice[]) {
	for (int number = 9; number >= 0; number--) { /*Boucle de décompte de 9 à 0*/
		Setnumber(number);
		SetLedMatrice();
		sleep(1);
	}
}
