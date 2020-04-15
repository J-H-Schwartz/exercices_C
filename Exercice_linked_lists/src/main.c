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
#include<stdlib.h>

typedef struct linked_list_element{
	int valeur;
	struct linked_list_element* next;
}linked_list_element;

int main(void){
	linked_list_element* root = (linked_list_element*)malloc(sizeof(linked_list_element));
	root->valeur = 0;
	root->next = NULL;
	linked_list_element* tmp = root;
	for (int i = 0; i < 10; i++){
		linked_list_element* next_element = (linked_list_element*)malloc(sizeof(root));
		next_element->valeur = i + 1;
		linked_list_element* next_element_tmp = next_element;
		next_element->next = tmp;
		tmp = next_element_tmp;
	}


	linked_list_element* cursor = tmp;
	while (cursor != NULL){
		printf("%d\n",cursor->valeur);
		cursor = cursor->next;
	}


	cursor = tmp;
	linked_list_element* tmp2;
	while (cursor != NULL){
		tmp2 = cursor->next;
		free(cursor);
		cursor = tmp2;
	}

	return 0;
}
