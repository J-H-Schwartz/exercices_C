/*
 * compare.c
 *
 *  Created on: Feb 26, 2020
 *      Author: jonathan
 */
#include<stdio.h>
#include "compare.h"

int comparer(const char s1[], const char s2[]){
	char word_end = '\0';
	int i = 0;
	while (s1[i] != word_end || s2[i] != word_end){
		if (s1[i]>s2[i]){
			printf("S2 est placé avant à S1\n");
			return 1;
		}
		else if (s1[i]<s2[i]){
			printf("S1 est placé avant à S2\n");
			return -1;
		}
		else{
			i+=1;
		}
	}
	printf("S1 et S2 sont égaux\n");
	return 0;
}
