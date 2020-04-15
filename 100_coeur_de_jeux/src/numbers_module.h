/*
 * numbers_module.h
 *
 *  Created on: Apr 3, 2020
 *      Author: jonathan
 */

#ifndef NUMBERS_MODULE_H_
#define NUMBERS_MODULE_H_

typedef struct { /*Définition du nouveau type led qui contient les valeurs des couleurs*/
	int RValue; /*Rouge, Verte, et Bleue et qui est une struct.						  */
	int GValue;
	int BValue;
} led;

/************************************************************************************************/
/*									Setnumber													*/
/*	But:																						*/
/*		Fonction qui modifie la couleur d'une led unique.										*/
/*																								*/
/* 	Interface:																					*/
/* 		*matrice[] qui est un tableau de tableaux de structures de type led (struct).			*/
/*		number qui est le numéro à afficher par la matrice, de type integer.			 		*/
/*		color qui est la couleur à appliquer au chiffre, de type led (struct).					*/
/*		background_color qui est la couleur à appliquer au fond, de type led (struct).			*/
/*																								*/
/************************************************************************************************/

void Setnumber(int number);

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

void SetLedMatrice(void);
void SetBlackMatrice(void);
void SetMatriceBack(void);

#endif /* NUMBERS_MODULE_H_ */
