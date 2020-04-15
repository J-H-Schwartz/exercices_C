/*
 * pile.h
 *
 *  Created on: Feb 26, 2020
 *      Author: jonathan
 */

#ifndef CGI_PILE_H
#define CGI_PILE_H

/*  Structure représentant un élément de la pile. */

typedef struct pile {
	int valeur;
	struct pile *prec;
} pile;

typedef struct pileMessage {
	struct pileMessage *prec;
	char valeur[];
//	char *valeur;
} pileMessage;


#ifdef __cplusplus
extern "C" {
#endif

/*  Push empile une valeur sur la pile. */

void Push(pile **, int);

/*  Pop retire la dernière valeur empilée sur la pile. */

int Pop(pile **);

/*  Clear vide la pile. */

void Clear(pile **);

/*  Length retourne le nombre d'éléments de la pile. */

int Length(pile *p);

/*  Affiche la totalité de la pile en commençant par le sommet. */

void View(pile *);

void ViewM(pileMessage *);

void PushM(pileMessage **, char[]);

char PopM(pileMessage **);

void ClearM(pileMessage **);

int LengthM(pileMessage *p);

#ifdef __cplusplus
}
#endif

#endif
