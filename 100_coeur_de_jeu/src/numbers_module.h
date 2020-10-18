/*
 * numbers_module.h
 *
 *  Created on: Apr 3, 2020
 *      Author: jonathan
 */

#ifndef NUMBERS_MODULE_H_
#define NUMBERS_MODULE_H_
#include"display_thread.h"

void Setnumber(led (*matrix)[7], int number, int color);
void SetFlashingSmiley(led (*matrix)[7], int color, int timer);


#endif /* NUMBERS_MODULE_H_ */
