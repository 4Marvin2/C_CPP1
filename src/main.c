// Copyright 2021 Mikhail Popov

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/utils.h"

#define MAX_FIELD_LENGTH 101
#define NUMBER_OF_FIELDS 4
// + 4 - знаки разделители и конец строки/перевод строки
#define MAX_STR_LENGTH MAX_FIELD_LENGTH * NUMBER_OF_FIELDS + 4
#define INCREASING_THE_ARRAY 2
#define MIN_ARRAY_SIZE 2
#define MAX_ERROR_STR_LENGTH 200

#define ERROR_EMPTY_LINE 1
#define ERROR_WRONG_NUMBER_OF_VARIABLE_IN_LINE 2
#define FILE_OPEN_ERROR -1
#define ERROR_EMPTY_FILE -2
#define NULL_PTR -3
#define NULL_PTR_REALLOC -4
#define NULL_SIZE_REALLOC -6
#define ERROR_PRINT -7
#define ERROR_EMPTY_ARRAY -8

int main() {
    // считываем данные из файла
    char file_name[] = "songs_data.txt";
    Song *all_songs = NULL;
    int songs_count = read_data_from_file(file_name, &all_songs);

    if (songs_count <= 0) {
        char *error_description;
        error_description = error_decoding(songs_count);
        printf("%s\n", error_description);
        free(error_description);
        return 1;
    }

    // вводим автора, по которому будем осуществлять поиск
    char my_author[MAX_FIELD_LENGTH] = {0};
    printf("%s\n", "Введите автора, песни которого, хотите найти (не более 100 символов)");
    scanf("%100s", my_author);

    // находим нужные песни
    Song *right_songs = NULL;
    int right_songs_count = search_by_author(all_songs, songs_count, my_author, &right_songs);

    if (right_songs_count <= 0) {
        char *error_description;
        error_description = error_decoding(right_songs_count);
        printf("%s\n", error_description);
        free(error_description);
        return 1;
    }

    // выводим нужные песни
    if (print_songs(right_songs, right_songs_count, stdout) == ERROR_PRINT) {
        printf("%s\n", "Ошибка: не удалось напечатать песни");
        return ERROR_PRINT;
    }

    free(all_songs);
    free(right_songs);

    return 0;
}
