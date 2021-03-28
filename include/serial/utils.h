#ifndef INCLUDE_SERIAL_UTILS_H_
#define INCLUDE_SERIAL_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_ARR_SIZE 1
#define CORRECT 0
#define NULL_PTR -1
#define NULL_SIZE_REALLOC -2
#define NULL_PTR_REALLOC -3
#define NULL_STREAM -6
#define OPEN_FILE_FAILED -5
#define NO_SUBSTRING -7

// нахождение подстроки самой часто встречаемой длины (последовательно)
int search_substring_of_the_most_common_length_serial(const char *filename, char **result);

#endif  // INCLUDE_SERIAL_UTILS_H_
