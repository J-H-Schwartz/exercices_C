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
#include<ctype.h>
#include<string.h>
#include<wchar.h>
#include<locale.h>
#include<stdlib.h>

int analyze_results[6] = {};

int vowels_ratio_count(int text_char_count, int text_vowels)
{
	double result;
	double vowels_ratio = (double)text_vowels * 100.0 / (double)text_char_count;
	result = (int)vowels_ratio;
	return result;
}

int consonants_ratio_count(int text_char_count, int text_consonants)
{
	int result;
	double consonants_ratio = (double)text_consonants * 100.0 / (double)text_char_count;
	result = (int)consonants_ratio;
	return result;
}


int* analyze_text(wchar_t text[])
{
	wchar_t vowels []= L"AEIOUYaeiouyÂâÀàÉéÈèÊêÎîÔôÖöÛûÜüÙù";
	wchar_t consonants []= L"ZzRrTtPpQqSsDdFfGgHhJjKkLlMmWwXxCcVvBbNn";
	int text_char_count = 0;
	int text_consonants = 0;
	int text_vowels = 0;
	int text_other_chars = 0;
	int vowels_length = wcslen(vowels);
	int consonants_length = wcslen(consonants);
	int vowel;
	int consonant;
	setlocale(LC_ALL, "");
	while (text[text_char_count] != '\0')
		{
				vowel = 0;
				consonant = 0;
				for (int i = 0; (i < vowels_length) && !vowel; i++)
				{
					if (text[text_char_count] == vowels[i])
					{
						text_vowels += 1;
						vowel = 1;
					}
				}
				for (int j = 0; j < consonants_length && !vowel && !consonant; j++)
				{
					if (text[text_char_count] == consonants[j])
					{
						text_consonants += 1;
						consonant = 1;
					}
				}
				text_char_count++;
		}
		text_other_chars = text_char_count - text_vowels - text_consonants;
		int vowels_ratio = (int)vowels_ratio_count(text_char_count, text_vowels);
		int consonants_ratio = (int)consonants_ratio_count(text_char_count, text_consonants);
		analyze_results [0] = text_char_count;
		analyze_results [1] = text_vowels;
		analyze_results [2] = text_consonants;
		analyze_results [3] = text_other_chars;
		analyze_results [4] = vowels_ratio;
		analyze_results [5] = consonants_ratio;
		return analyze_results;
}


int main(void)
{
	setlocale(LC_ALL, "");
	FILE *example_file = fopen("/home/jonathan/Desktop/Repositories/exercices_C/61_count_char_file/Debug/example.txt", "rt+, ccs=UTF-8");
	FILE *out = fopen("/home/jonathan/Desktop/Repositories/exercices_C/61_count_char_file/Debug/out.txt", "w");
	wchar_t ch;
	if (example_file == NULL)
	{
		printf("Can't open file.");
		return 1;
	}
	wchar_t file_array[255]= L"";
	for (int i = 0; (ch=fgetwc(example_file)) != EOF; i++)
	{
		file_array[i] = ch;
		fwprintf(out, L"%lc", toupper(ch));
	}
	fclose(example_file);
	fclose(out);
	wchar_t text_1 []= L"“Bonjour, je suis heureux d’être au campus”";
	wchar_t text_2 []= L"Qu'est-ce qu'un processeur ARM ? C'est un type de processeur particulier dont l'agencement des composants électroniques permet une taille réduite. ARM est l'acronyme de Advanced Risk Machine. A l'origine, le A de ARM signifiait Acorn, le nom de la société qui a équipé pour la première fois un ordinateur avec unprocesseur ARM, en 1987. Depuis, l'architecture ARM a rencontré un succès qui ne se démentpas, notamment ces dernières années. Des processeurs ARM, on en trouve dans tous les smartphones récents : iPhone 4, Samsung Galaxy S, tout comme dans l'iPad, le Nokia N97 ou le Nexus One de Google. Mais la société ARM ltd ne fabrique pas, à proprement parler, les processeurs. Elle conçoit les architectures des processeurs, leurs plans en quelque sorte. Ce sont les clients qui, après achat des licences, produisent les processeurs. Par exemple, Samsung fabrique un processeur ARM pour ses téléphones haut de gamme (Samsung Galaxy S et Wave), reposant sur l'architecture ARM Cortex-A8, cadencée à 1 GHz. Le processeur Apple A4, qui équipe les iPhone 4 et les iPad, utilise exactement la même architecture Cortex-A8. Ces “ petits ” processeurs connaissent un tel succès qu'ils évoluent très rapidement : Samsung a déjà annoncé le remplaçant de Cortex-A8 ; celui-ci répond au nom de Cortex-A9 et intègre deux cœurs, comme dans les PC. Quant au Cortex-A15, qui devrait être disponible dans une vingtaine de mois, on sait déjà qu'il s'agira d'un processeur quatre cœurs cadencé à 2,5 GHz et basse consommation ! Un tel processeur devrait avoir des performances similaires à celles d'un ordinateur portable très haut de gamme, alors qu'il est prévu pour être embarqué dans un téléphone de seulement quelques millimètres d'épaisseur.";
    analyze_text((wchar_t*)file_array);
	printf("%d caracteres, %d voyelles, %d consonnes, et %d autres caractères. Il y a %d %% de voyelles et %d %% de consonnes.\n", analyze_results[0], analyze_results[1], analyze_results[2], analyze_results[3], analyze_results[4], analyze_results[5]);
	analyze_text(text_1);
	printf("%d caracteres, %d voyelles, %d consonnes, et %d autres caractères. Il y a %d %% de voyelles et %d %% de consonnes.\n", analyze_results[0], analyze_results[1], analyze_results[2], analyze_results[3], analyze_results[4], analyze_results[5]);
	analyze_text(text_2);
	printf("%d caracteres, %d voyelles, %d consonnes, et %d autres caractères. Il y a %d %% de voyelles et %d %% de consonnes.\n", analyze_results[0], analyze_results[1], analyze_results[2], analyze_results[3], analyze_results[4], analyze_results[5]);
	return 0;
}
