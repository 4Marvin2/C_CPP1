// Copyright 2021 Mikhail Popov

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "general/utils.h"

int realloc_array(char **arr, size_t size) {
    if (arr == NULL) {
        return NULL_PTR;
    }
    if (size == 0) {
        return NULL_SIZE_REALLOC;
    }
    char *temp_arr = (char *)malloc(sizeof(char) * size);
    if (temp_arr ==  NULL) {
        return NULL_PTR_REALLOC;
    }
    // перекопируем
    memcpy(temp_arr, (*arr), strlen(*arr) + 1);
    free(*arr);
    // переназначаем изначальный массив
    (*arr) = temp_arr;

    return size;
}

int search_max(int *arr, int size) {
    if (arr == NULL) {
        return NULL_PTR;
    }
    int max = 0;
    int max_index;
    for (int i = 0; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
            max_index = i;
        }
    }

    return max_index;
}

int search_substring(char *arr, int first_position_of_substring, int most_frequent_length, char *result) {
    if ((arr == NULL) || (result == NULL)) {
        return NULL_PTR;
    }
    int j = 0;
    for (int i = first_position_of_substring; i < first_position_of_substring + most_frequent_length; i++) {
        result[j] = arr[i];
        j++;
    }
    result[j] = '\0';

    return CORRECT;
}

int output(char *arr, int first_position_of_substring, int most_frequent_length, FILE *stream) {
    if (arr == NULL) {
        return NULL_PTR;
    }
    if (stream == NULL) {
        return NULL_STREAM;
    }
    fprintf(stream, "Result: ");
    for (int i = first_position_of_substring; i < first_position_of_substring + most_frequent_length; i++) {
        putc(arr[i], stream);
    }
    fprintf(stream, "\n");

    return CORRECT;
}

int search_first_occurrence_of_substring(char *arr, int size, int most_frequent_length) {
    if (arr == NULL) {
        return NULL_PTR;
    }
    int temp_count = 1;
    char previous;
    previous = arr[0];
    for (int i = 1; i < size; i++) {
        char current = arr[i];
        if (current == previous) {
            temp_count++;
        } else {
            if (temp_count == most_frequent_length) {
                return i - temp_count;
            }
            temp_count = 1;
        }
        previous = current;
    }

    return NO_SUBSTRING;
}
