// Copyright 2021 Mikhail Popov

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "general/utils.h"

#define MIN_ARR_SIZE 1
#define BUFFERSIZE 10

int input(const char *filename, char **arr, int fd) {
    if ((arr == NULL) || (filename == NULL)) {
        return NULL_PTR;
    }

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        return OPEN_FILE_FAILED;
    }

    struct stat st;
    stat(filename, &st);
    size_t file_size = st.st_size;

    size_t arr_size;
    if (file_size % getpagesize() == 0) {
        arr_size = file_size * sizeof(char);
    } else {
        arr_size = ((file_size * sizeof(char)) / getpagesize()) * getpagesize() + getpagesize();
    }
    *arr = (char *)mmap(NULL, arr_size, PROT_READ, MAP_PRIVATE, fd, 0);

    if ((*arr) == MAP_FAILED) {
        close(fd);
        return MMAP_FAILED;
    }

    return CORRECT;
}

int* create_arr_counter(int size) {
    int *number_of_repeating_length = (int *)malloc(sizeof(int) * size);
    if (number_of_repeating_length == NULL) {
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        number_of_repeating_length[i] = 0;
    }

    return number_of_repeating_length;
}

int search_number_of_repeating_length(char *arr, int size, int *arr_counter) {
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

int search_most_frequent_value(int *arr_counter, int size) {
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

int free_resources(int fd, char *first_arr, int size, int *second_arr) {
    close(fd);
    int err = munmap(first_arr, size);
    free(second_arr);
    if (err != CORRECT) {
        return MUNMAP_FAILED;
    }

    return CORRECT;
}

int search_substring_of_the_most_common_length(const char *filename, char **result) {
    char *my_arr = NULL;
    int fd = 0;
    int condition_of_function = input(filename, &my_arr, fd);
    if (condition_of_function != CORRECT) {
        close(fd);
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
        int err = free_resources(fd, my_arr, length, number_of_repeating_length);
        if (err != CORRECT) {
            return err;
        }
        return condition_of_function;
    }

    int most_frequent_length = search_most_frequent_value(number_of_repeating_length, length);
    if (most_frequent_length == NULL_PTR) {
        int err = free_resources(fd, my_arr, length, number_of_repeating_length);
        if (err != CORRECT) {
            return err;
        }
        return most_frequent_length;
    }

    int first_position_of_substring = search_first_occurrence_of_substring(my_arr, length, most_frequent_length);
    if (first_position_of_substring < 0) {
        int err = free_resources(fd, my_arr, length, number_of_repeating_length);
        if (err != CORRECT) {
            return err;
        }
        return first_position_of_substring;
    }

    if (*result == NULL) {
        (*result) = (char *)malloc(sizeof(char) * (most_frequent_length + 1));
    }

    condition_of_function = search_substring(my_arr, first_position_of_substring, most_frequent_length, *result);
    if (condition_of_function != CORRECT) {
        int err = free_resources(fd, my_arr, length, number_of_repeating_length);
        if (err != CORRECT) {
            return err;
        }
        return condition_of_function;
    }

    condition_of_function = output(my_arr, first_position_of_substring, most_frequent_length, stdout);
    if (condition_of_function != CORRECT) {
        int err = free_resources(fd, my_arr, length, number_of_repeating_length);
        if (err != CORRECT) {
            return err;
        }
        return condition_of_function;
    }

    int err = free_resources(fd, my_arr, length, number_of_repeating_length);
    if (err != CORRECT) {
        return err;
    }

    return CORRECT;
}
