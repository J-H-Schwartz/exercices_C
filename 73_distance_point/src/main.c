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
	double y;
} point;

double distance_calc(point point_1, point point_2){
	double length = point_2.x-point_1.x;
	double width = point_2.y-point_1.y;
	double sides = (length*length)+(width*width);
	double result = sqrt(sides);
	return result;
}

int main(void){
	point point_1;
	point point_2;
	double x1;
	double y1;
	double x2;
	double y2;
	double result;
	printf("Entrez les coordonnées des 2 points suivant le format: x1 y1 x2 y2 : ");
	scanf("%lf %lf %lf %lf", &x1, &y1, &x2, &y2);
	point_1.x = x1;
	point_1.y = y1;
	point_2.x = x2;
	point_2.y = y2;
	result = distance_calc(point_1, point_2);
	printf("%10.2lf", result);
	return 0;
}


