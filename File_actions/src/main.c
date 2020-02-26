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
#include<string.h>

int main(void){

	FILE *example_file = fopen("/home/jonathan/Desktop/Repositories/exercices_C/File_actions/Debug/example_file.txt", "r");
	if (example_file == NULL)
		{
			printf("Can't open file.");
			return 1;
		}
	char vowels []= "AEIOUYaeiouy¬‚¿‡…È»Ë ÍŒÓ‘Ù÷ˆ€˚‹¸Ÿ˘";
	char consonants []= "ZzRrTtPpQqSsDdFfGgHhJjKkLlMmWwXxCcVvBbNn";
	int text_char_count = 0;
	int consonants_length = strlen(consonants);
	int text_vowels = 0;
	int text_consonants = 0;
	int is_consonant;
	int is_vowel;
	char ch;
	for (int i = 0; (ch=fgetc(example_file)) != EOF; i++)
		{
			is_vowel = 0;
			is_consonant = 0;
			for (int i = 0; (vowels[i] != '\0') && !is_vowel; i++)
				{
					if (ch == vowels[i])
					{
						text_vowels += 1;
						is_vowel = 1;
					}
				}
			for (int j = 0; j < consonants_length && !is_vowel && !is_consonant; j++)
				{
					if (ch == consonants[j])
					{
						text_consonants += 1;
						is_consonant = 1;
					}
				}
			text_char_count++;
		}
	int text_other_chars = text_char_count - text_consonants - text_vowels;
	printf("%d caracteres, %d consonnes, %d voyelles, et %d autres caractËres.\n", text_char_count, text_consonants, text_vowels, text_other_chars);
	return 0;
}
