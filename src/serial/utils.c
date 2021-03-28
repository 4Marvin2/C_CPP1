// Copyright 2021 Mikhail Popov

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "serial/utils.h"

#define BUFFERSIZE 10

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

int input(char **arr, FILE *stream) {
    if (arr == NULL) {
        return NULL_PTR;
    }
    if (stream == NULL) {
        return NULL_STREAM;
    }
    char *buffer = (char *)malloc(sizeof(char) * BUFFERSIZE);
    if (buffer == NULL) {
        return NULL_PTR;
    }
    while (fgets(buffer, BUFFERSIZE, stream)) {
        int size = realloc_array(arr, strlen(*arr) + 1 + strlen(buffer));
        if ((size == NULL_PTR) || (size == NULL_PTR_REALLOC)) {
            free(buffer);
            return NULL_PTR;
        }
        strncat(*arr, buffer, BUFFERSIZE);
    }
    free(buffer);

    return CORRECT;
}

int* create_arr_counter(size_t size) {
    int *number_of_repeating_length = (int *)malloc(sizeof(int) * size);
    if (number_of_repeating_length == NULL) {
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        number_of_repeating_length[i] = 0;
    }

    return number_of_repeating_length;
}

int search_number_of_repeating_length(char *arr, size_t size, int *arr_counter) {
    if ((arr == NULL) || (arr_counter == NULL)) {
        return NULL_PTR;
    }
    int temp_count = 1;
    for (int i = 0;  i < size - 1; i++) {
        char previous = arr[i];
        char current = arr[i + 1];
        if (current == previous) {
            temp_count++;
        } else {
            arr_counter[temp_count]++;
            temp_count = 1;
        }
    }

    return CORRECT;
}

int search_most_frequent_value(int *arr_counter, size_t size) {
    if (arr_counter == NULL) {
        return NULL_PTR;
    }
    int most_frequent_length = 0;
    int most_frequent_length_count = 0;
    for (int i = 2; i < size; i++) {
        if (arr_counter[i] > most_frequent_length_count) {
            most_frequent_length_count = arr_counter[i];
            most_frequent_length = i;
        }
    }

    return most_frequent_length;
}

int search_first_occurrence_of_substring(char *arr, size_t size, int most_frequent_length) {
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

void free_resources(FILE *fp, char *first_arr, int *second_arr) {
    fclose(fp);
    free(first_arr);
    free(second_arr);
}

int search_substring_of_the_most_common_length_serial(const char *filename, char **result) {
    char *my_arr = (char *)malloc(sizeof(char) * MIN_ARR_SIZE);
    if (my_arr == NULL) {
        return NULL_PTR;
    }
    my_arr[0] = '\0';
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        free(my_arr);
        return OPEN_FILE_FAILED;
    }

    int condition_of_function = input(&my_arr, fp);
    if (condition_of_function != CORRECT) {
        fclose(fp);
        free(my_arr);
        return condition_of_function;
    }

    size_t length = strlen(my_arr) + 1;  // +1 это \0

    int *number_of_repeating_length = create_arr_counter(length);
    if (number_of_repeating_length == NULL) {
        return NULL_PTR;
    }

    condition_of_function = search_number_of_repeating_length(my_arr, length, number_of_repeating_length);
    if (condition_of_function != CORRECT) {
        free_resources(fp, my_arr, number_of_repeating_length);
        return condition_of_function;
    }

    int most_frequent_length = search_most_frequent_value(number_of_repeating_length, length);
    if (most_frequent_length == NULL_PTR) {
        free_resources(fp, my_arr, number_of_repeating_length);
        return most_frequent_length;
    }

    int first_position_of_substring = search_first_occurrence_of_substring(my_arr, length, most_frequent_length);
    if (first_position_of_substring < 0) {
        free_resources(fp, my_arr, number_of_repeating_length);
        return first_position_of_substring;
    }

    if (*result == NULL) {
        (*result) = (char *)malloc(sizeof(char) * (most_frequent_length + 1));
    }

    condition_of_function = search_substring(my_arr, first_position_of_substring, most_frequent_length, *result);
    if (condition_of_function != CORRECT) {
        free_resources(fp, my_arr, number_of_repeating_length);
        return condition_of_function;
    }

    condition_of_function = output(my_arr, first_position_of_substring, most_frequent_length, stdout);
    if (condition_of_function != CORRECT) {
        free_resources(fp, my_arr, number_of_repeating_length);
        return condition_of_function;
    }

    free_resources(fp, my_arr, number_of_repeating_length);

    return CORRECT;
}
