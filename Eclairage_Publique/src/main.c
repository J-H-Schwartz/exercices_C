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

int main(){
	int nighttime = 0;
	int time = 0;
	int partial_night = 0;
	int power_rate = 0;
	float power = 0.0;
	while (1)
	{
		scanf("%d %d %d", &nighttime, &partial_night, &time);
		bool past_midnight = (time >= 0 && time < 5);
		bool dawn_traffic_time = (time >= 5 && time < 12 && nighttime);
		if (past_midnight)
		{
			power = 8.4;
			power_rate = 70;
		}
		else if (dawn_traffic_time)
		{
			power = 12;
			power_rate = 100;
		}
		else if (partial_night)
		{
			power = 6;
			power_rate = 50;
		}
		else if (nighttime)
		{
			power = 12;
			power_rate = 100;
		}
		else
		{
			power = 0;
			power_rate = 0;
		}
		printf("Lights at %d %% power. Electric power set to %2.2f Volts.", power_rate, power);
	}
	return 0;
}
