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
#include"leds_control.h"
#include<unistd.h>
#define COLOR_ON 255
#define COLOR_OFF 0
#define RED 0
#define GREEN 1
#define BLUE 2
#define WHITE 3
#define PURPLE 4
#define YELLOW 5
#define BLACK 6


typedef struct {			/*Définition du nouveau type led qui contient les valeurs des couleurs*/
	int RValue;				/*Rouge, Verte, et Bleue et qui est une struct.						  */
	int GValue;
	int BValue;
} led;



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
static led *matrice[7] = { line_1, line_2, line_3, line_4, line_5, line_6, line_7 };

static led colors[7] = { { COLOR_ON, COLOR_OFF, COLOR_OFF }, { COLOR_OFF, COLOR_ON,
COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_ON }, { COLOR_ON,
COLOR_ON, COLOR_ON }, { COLOR_ON, COLOR_OFF, COLOR_ON }, { COLOR_ON,
COLOR_ON, COLOR_OFF }, { COLOR_OFF, COLOR_OFF, COLOR_OFF } };



/************************************************************************************************/
/*									SetLedNewColor												*/
/*	But:																						*/
/*		Fonction qui modifie la couleur d'une led unique.										*/
/*																								*/
/* 	Interface:																					*/
/* 		*leds_table[] qui est un tableau de tableaux de structures de type led (struct).		*/
/*		led_num qui est le numéro de la led (sa position dans la ligne) de type integer. 		*/
/*		new_led_color qui est la nouvelle couleur à appliquer, de type led (struct).			*/
/*																								*/
/************************************************************************************************/

void SetLedNewColor(led leds_table[], int led_num, led new_led_color) {
	leds_table[led_num].RValue = new_led_color.RValue;
	leds_table[led_num].GValue = new_led_color.GValue;
	leds_table[led_num].BValue = new_led_color.BValue;
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

void SetLedMatrice(led *leds_table[], int Tabsize) {
	for (int row = 0; row < Tabsize; row++) {
		for (int col = 0; col < Tabsize; col++) {
			setLedColor((row + 1), (col + 1), leds_table[row][col].RValue,
					leds_table[row][col].GValue, leds_table[row][col].BValue);

		}
	}
}



/************************************************************************************************/
/*									print_colored_square										*/
/*	But:																						*/
/*		Fonction qui affiche un des carrés de couleurs imbriqués sur la matrice.				*/
/*																								*/
/* 	Interface:																					*/
/* 		*matrice[] qui est un tableau de tableaux de structures de type led (struct).			*/
/*																								*/
/************************************************************************************************/

void print_colored_square(led *matrice[]){
	for (int row = 0; row < 7; row++) {
		for (int col = 0; col < 7; col++) {
			if (row == 3 && col == 3)SetLedNewColor(matrice[row], col, colors[YELLOW]);
			else if ( (row >= 2 && row <= 4) && (col >= 2 && col <= 4)) SetLedNewColor(matrice[row], col, colors[GREEN]);
			else if ( (row >= 1 && col >= 1) && (row <= 5 && col <= 5)) SetLedNewColor(matrice[row], col, colors[BLACK]);
			else SetLedNewColor(matrice[row], col, colors[RED]);
		}
	}
}



/************************************************************************************************/
/*									Setnumber													*/
/*	But:																						*/
/*		Fonction qui modifie la couleur d'une led unique.										*/
/*																								*/
/* 	Interface:																					*/
/* 		*leds_table[] qui est un tableau de tableaux de structures de type led (struct).		*/
/*		number qui est le numéro à afficher par la matrice, de type integer.			 		*/
/*		color qui est la couleur à appliquer au chiffre, de type led (struct).					*/
/*		background_color qui est la couleur à appliquer au fond, de type led (struct).			*/
/*																								*/
/************************************************************************************************/

void Setnumber(led *matrice[], int number, led color, led background_color) {
	if (number == 1) {
		for (int row = 0; row < 7; row++) {
			for (int col = 0; col < 7; col++) {
				if ((row == 0 && col == 4) || (row == 1 && col == 3)		/*Ensemble de conditions pour afficher le chiffre 1*/
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
				if ((row == 0 && col == 2) || (row == 0 && col == 3)		/*Ensemble de conditions pour afficher le chiffre 2*/
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
				if ((row == 0 && col == 2) || (row == 0 && col == 3)		/*Ensemble de conditions pour afficher le chiffre 3*/
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
				if ((row == 0 && col == 4) || (row == 1 && col == 3)		/*Ensemble de conditions pour afficher le chiffre 4*/
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
				if ((row == 0 && col == 1) || (row == 0 && col == 2)		/*Ensemble de conditions pour afficher le chiffre 5*/
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
				if ((row == 0 && col == 2) || (row == 0 && col == 3)		/*Ensemble de conditions pour afficher le chiffre 6*/
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
				if ((row == 0 && col == 1) || (row == 0 && col == 2)		/*Ensemble de conditions pour afficher le chiffre 7*/
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
				if ((row == 0 && col == 2) || (row == 0 && col == 3)		/*Ensemble de conditions pour afficher le chiffre 8*/
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
				if ((row == 0 && col == 2) || (row == 0 && col == 3)		/*Ensemble de conditions pour afficher le chiffre 9*/
						|| (row == 0 && col == 4) || (row== 1 && col == 1)
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
				if ((row == 0 && col == 2) || (row == 0 && col == 3)		/*Ensemble de conditions pour afficher le chiffre 0*/
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

int main(void) {
	int up = 0;
	int down = 3;
	for (int number = 9; number >= 0; number--){			/*Boucle de décompte de 9 à 0*/
		Setnumber(matrice, number, colors[up], colors[down]);
		SetLedMatrice(matrice, 7);
		up++;
		down++;
		if (up == 7) up = 0;
		if (down == 7) down = 0;
		sleep(4);
	}

	return 0;
}


/*void SetLedColor(int led_num, led leds[]) {
 if (leds[led_num].RValue != COLOR_OFF && leds[led_num].GValue == COLOR_OFF && leds[led_num].BValue == COLOR_OFF) {
 printf("Red %d %d %d\n", leds[led_num].RValue, leds[led_num].GValue, leds[led_num].BValue);
 }
 else if (leds[led_num].RValue == COLOR_OFF && leds[led_num].GValue != COLOR_OFF && leds[led_num].BValue == COLOR_OFF) {
 printf("Green %d %d %d\n", leds[led_num].RValue, leds[led_num].GValue, leds[led_num].BValue);
 }
 else if (leds[led_num].RValue == COLOR_OFF && leds[led_num].GValue == COLOR_OFF && leds[led_num].BValue != COLOR_OFF) {
 printf("Blue %d %d %d\n", leds[led_num].RValue, leds[led_num].GValue, leds[led_num].BValue);
 }
 else if (leds[led_num].RValue == COLOR_ON && leds[led_num].GValue == COLOR_ON && leds[led_num].BValue == COLOR_ON){
 printf("White %d %d %d\n", leds[led_num].RValue, leds[led_num].GValue, leds[led_num].BValue);
 }
 else if (leds[led_num].RValue != COLOR_OFF && leds[led_num].GValue == COLOR_OFF && leds[led_num].BValue != COLOR_OFF) {
 printf("Purple %d %d %d\n", leds[led_num].RValue, leds[led_num].GValue, leds[led_num].BValue);
 }
 else if (leds[led_num].BValue == COLOR_OFF && leds[led_num].RValue == COLOR_ON && leds[led_num].GValue == COLOR_ON ) {
 printf("yellow %d %d %d\n", leds[led_num].RValue, leds[led_num].GValue, leds[led_num].BValue);
 }
 else if (leds[led_num].RValue == COLOR_OFF && leds[led_num].GValue == COLOR_OFF && leds[led_num].BValue == COLOR_OFF){
 printf("Black %d %d %d\n", leds[led_num].RValue, leds[led_num].GValue, leds[led_num].BValue);
 }
 }
 */

