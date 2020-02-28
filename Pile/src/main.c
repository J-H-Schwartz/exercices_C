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

#include <stdlib.h>   /* Pour la fonction system. */
#include <stdio.h>
#include "pile.h"

int main() {
	pile *MaPile = NULL; /* Impératif de l'initialiser à NULL */

	Push(&MaPile, 10);
	Push(&MaPile, 25);
	Push(&MaPile, 33);
	Push(&MaPile, 12); /* Empile 4 valeurs. */

	puts("Affichage de la pile :");
	View(MaPile); /* Affiche la totalité de la pile. */
	puts("------");

	printf("Nb d'elements : %d\n", Length(MaPile));
	puts("------");

	puts("Deux valeurs soutirees de la pile :");
	printf("%d\n", Pop(&MaPile)); /* Affiche deux valeurs */
	printf("%d\n", Pop(&MaPile)); /* soutirées de la pile. */
	puts("------");

	puts("Affichage de la pile :");
	View(MaPile); /* Affiche la totalité de la pile. */
	puts("------");

	Clear(&MaPile); /* Vide la pile. */

	Push(&MaPile, 18); /* Empile une valeur. */

	puts("Affichage de la pile apres vidage et ajout d'une valeur :");
	View(MaPile); /* Affiche la totalité de la pile. */
	puts("------\n");

	Clear(&MaPile); /* Vider la pile avant de quitter. */

#ifdef _WIN32
	system("PAUSE"); /* Pour la console Windows. */
#endif
	return 0;
}

