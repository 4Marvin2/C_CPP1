#ifndef INCLUDE_GENERAL_UTILS_H_
#define INCLUDE_GENERAL_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_ARR_SIZE 1
#define CORRECT 0
#define NULL_PTR -1
#define NULL_SIZE_REALLOC -2
#define NULL_PTR_REALLOC -3
#define NULL_STREAM -6
#define NO_SUBSTRING -7

int search_max(int *arr, int size);

int search_substring(char *arr, int first_position_of_substring, int most_frequent_length, char *result);

int output(char *arr, int first_position_of_substring, int most_frequent_length, FILE *stream);

int search_first_occurrence_of_substring(char *arr, int size, int most_frequent_length);

#endif  // INCLUDE_GENERAL_UTILS_H_
