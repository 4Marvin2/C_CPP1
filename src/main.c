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

#define ERROR_EMPTY_LINE 1
#define ERROR_WRONG_NUMBER_OF_VARIABLE_IN_LINE 2
#define FILE_OPEN_ERROR -1
#define ERROR_EMPTY_ARRAY -2

int main() {
    // считываем данные из файла
    char file_name[] = "songs_data.txt";
    Song *all_songs;
    int songs_count = read_data_from_file(file_name, &all_songs);

    if (songs_count == FILE_OPEN_ERROR) {
        printf("%s\n", "Ошибка чтения файла");
        return 0;
    }
    if (songs_count == ERROR_EMPTY_ARRAY) {
        printf("%s\n", "Ошибка: файл не содержит информации");
        return 0;
    }

    // вводим автора, по которому будем осуществлять поиск
    char my_author[MAX_FIELD_LENGTH];
    printf("%s\n",
    "Введите автора, песни которого, хотите найти (не более 100 символов)");
    scanf("%100s", my_author);

    // находим нужные песни
    Song *right_songs;
    int right_songs_count = search_by_author(all_songs,
                                             songs_count,
                                             my_author,
                                             &right_songs);

    if (songs_count == ERROR_EMPTY_ARRAY) {
        printf("%s\n", "Песен данного автора не найдено");
    }

    // выводим нужные песни
    print_songs(right_songs, right_songs_count);

    free(all_songs);
    free(right_songs);

    return 0;
}
