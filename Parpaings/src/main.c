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
#include<stdbool.h>

char cinderblock_category;
int weight;
int length;
int width;
int height;
int max_weight = 19380;		/*weight with +2% tolerance (Kilograms)*/
int min_weight = 18620;		/*weight with -2% tolerance (Kilograms)*/
int max_length = 510;		/*length with +2% tolerance (Millimeters)*/
int min_length = 490;		/*length with -2% tolerance (Millimeters)*/
int max_width = 204;		/*width with +2% tolerance (Millimeters)*/
int min_width = 196;		/*width with -2% tolerance (Millimeters)*/
int max_height = 204;		/*height with +2% tolerance (Millimeters)*/
int min_height = 196;		/*height with -2% tolerance (Millimeters)*/

int isbetween(int value, int min_value, int max_value)		/*Checks if dimensions value are correct*/
{
	if ((value >= min_value) && (value <= max_value))
	{
		return true;
	}
	else
	{
		return false;
	}
}

int correct_dimensions_counter(int height, int width, int length)		/*Counts how much dimensions are correct*/
{
	int correct_dimensions = 0;
	if (isbetween(height, min_height, max_height))
	{
		correct_dimensions += 1;
	}
	if (isbetween(width, min_width, max_width))
	{
		correct_dimensions += 1;
	}
	if (isbetween(length, min_length, max_length))
	{
		correct_dimensions += 1;
	}
	return correct_dimensions;
}

char cinderblock_check(int weight, int length, int width, int height)		/*Checks in which category goes each cinderblock*/
{
	int correct_dimensions = correct_dimensions_counter(height, width, length);
	bool correct_weight = isbetween(weight, min_weight, max_weight);
	if (correct_weight)
	{
		if (correct_dimensions >= 2)
		{
			cinderblock_category = 'A';
		}
		else
		{
			cinderblock_category = 'B';
		}
	}
	else
	{
		if (correct_dimensions == 3)
		{
			cinderblock_category = 'B';
		}
		else
		{
			cinderblock_category = 'C';
		}
	}
	return cinderblock_category;
}

int main()
{
	while (1)
	{
		char category;
		scanf("%d %d %d %d", &weight, &length, &width, &height);
		category = cinderblock_check(weight, length, width, height);
		printf("%c\n", category);
	}
	return 0;
}

