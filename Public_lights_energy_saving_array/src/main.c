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

const int NIGHT_POWER_RATIO=70;
const int EVENING_POWER_RATIO = 100;
const int TRAFFIC_POWER_RATIO = 100;
const int DAWN_DUSK_POWER_RATIO = 50;
const int TOTAL_DAY_LENGTH = 24;
const int MAX_NIGHT_POWER_LENGTH = 5;
const float DAWN=0.5;
const float DUSK=0.5;
int main(){
	while (1)
	{
		double sunrise_time;
		double day_length;
		int energy_ratio_spent_without_savings;
		int energy_ratio_spent_with_savings;

		scanf("%lf %lf", &sunrise_time, &day_length);

		double total_night_length = TOTAL_DAY_LENGTH - day_length;
		double evening_length = total_night_length - sunrise_time - DUSK;
		double dawn_electric_use = DAWN * DAWN_DUSK_POWER_RATIO;
		double dusk_electric_use = DUSK * DAWN_DUSK_POWER_RATIO;
		double evening_electric_use = ((evening_length*EVENING_POWER_RATIO) + dusk_electric_use);
		energy_ratio_spent_without_savings = total_night_length * EVENING_POWER_RATIO / TOTAL_DAY_LENGTH;


		if (sunrise_time <= MAX_NIGHT_POWER_LENGTH)
		{
			double night_electric_use = (sunrise_time - DAWN) * NIGHT_POWER_RATIO;

			energy_ratio_spent_with_savings = (night_electric_use + dawn_electric_use + evening_electric_use)/ TOTAL_DAY_LENGTH;
		}
		else
		{
			double night_electric_use = MAX_NIGHT_POWER_LENGTH * NIGHT_POWER_RATIO;
			double traffic_time_electric_use = (sunrise_time - MAX_NIGHT_POWER_LENGTH) * TRAFFIC_POWER_RATIO;

			energy_ratio_spent_with_savings = (night_electric_use + traffic_time_electric_use + evening_electric_use)/TOTAL_DAY_LENGTH;
		}
		printf("%2.2f %d %% %d %% \n", day_length, energy_ratio_spent_without_savings, energy_ratio_spent_with_savings);
	}
	return 0;
}
