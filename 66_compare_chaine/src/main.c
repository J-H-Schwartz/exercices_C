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

int main(void){
	int result;
	const char s1[] = "azerty";
	const char s2[] = "azerty";
	result = comparer(s1, s2);
	printf("Retour de fonction : %d\n", result);
	return 0;
}


