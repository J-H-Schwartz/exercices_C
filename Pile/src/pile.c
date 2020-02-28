/*
 * pile.c
 *
 *  Created on: Feb 26, 2020
 *      Author: jonathan
 */

#include <stdlib.h>
#include <stdio.h>

#include "pile.h"

/*************************************************************************/

void Push(pile **p, int Val) {
	pile *element = malloc(sizeof(pile));
	if (!element)
		exit(EXIT_FAILURE); /* Si l'allocation a échoué. */
	element->valeur = Val;
	element->prec = *p;
	*p = element; /* Le pointeur pointe sur le dernier élément. */
}
/*************************************************************************/

int Pop(pile **p) {
	int Val;
	pile *tmp;
	if (!*p)
		return -1; /* Retourne -1 si la pile est vide. */
	tmp = (*p)->prec;
	Val = (*p)->valeur;
	free(*p);
	*p = tmp; /* Le pointeur pointe sur le dernier élément. */
	return Val; /* Retourne la valeur soutirée de la pile. */
}

/*************************************************************************/

void Clear(pile **p) {
	pile *tmp;
	while (*p) {
		tmp = (*p)->prec;
		free(*p);
		*p = tmp;
	}
}
/*************************************************************************/

int Length(pile *p) {
	int n = 0;
	while (p) {
		n++;
		p = p->prec;
	}
	return n;
}

/*************************************************************************/

void View(pile *p) {
	while (p) {
		printf("%d\n", p->valeur);
		p = p->prec;
	}
}

