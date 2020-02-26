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
#include<math.h>

typedef struct {
	double x;
	char blabla[1024];
	double y;
} point;

point translate_point(point point,double delta_x, double delta_y){
	printf("translate point= %p\n", &point);
	point.x += delta_x;
	point.y += delta_y;
	return point;
}

void translate(point *point_ptr, double delta_x, double delta_y){
	printf("translate point= %p\n", point_ptr);
	(*point_ptr).x += delta_x;
	(*point_ptr).y += delta_y;
}

int main(void){
	point point_1;
	point point_2;
	point_1.x = 1;
	point_1.y = 1;
	point_2.x = 3;
	point_2.y = 3;
	double delta_x;
	double delta_y;
	point result;
	printf("Entrez les déplacements (delta_x et delta_y) : ");
	scanf("%lf %lf", &delta_x, &delta_y);
	printf("Avant translate point= %p\n", &point_1);
	result = translate_point(point_1, delta_x, delta_y);
	printf("After translate point= %p\n", &result);
	printf("Point 1 : %10.2lf %10.2lf\n", point_1.x, point_1.y);
	printf("Nouveau point créé à partir du premier : %10.2lf %10.2lf\n", result.x, result.y);
	printf("Point 2 avant translation : %10.2lf %10.2lf\n", point_2.x, point_2.y);
	translate(&point_2, delta_x, delta_y);
	printf("Point 2 après translation :%10.2lf %10.2lf\n", point_2.x, point_2.y);
	return 0;
}


