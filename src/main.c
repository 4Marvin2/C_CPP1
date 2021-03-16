// Copyright 2021 Mikhail Popov

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/utils.h"

int main() {
    // считываем данные из файла
    char file_name[] = "songs_data.txt";
    Song *all_songs = NULL;
    int songs_count = read_data_from_file(file_name, &all_songs);

    if (songs_count <= 0) {
        char *error_description;
        error_description = error_decoding(songs_count);
        fprintf(stderr, "%s\n", error_description);
        free(error_description);
        return 1;
    }

    // вводим автора, по которому будем осуществлять поиск
    char my_author[MAX_FIELD_LENGTH] = {0};
    printf("%s\n", "Введите автора, песни которого хотите найти (не более 100 символов)");
    scanf("%100s", my_author);

    // находим нужные песни
    Song *right_songs = NULL;
    int right_songs_count = search_by_author(all_songs, songs_count, my_author, &right_songs);

    if (right_songs_count <= 0) {
        char *error_description;
        error_description = error_decoding(right_songs_count);
        fprintf(stderr, "%s\n", error_description);
        free(error_description);
        return 1;
    }

    // выводим нужные песни
    if (print_songs(right_songs, right_songs_count, stdout) == ERROR_PRINT) {
        fprintf(stderr, "%s\n", "Ошибка: не удалось напечатать песни");
        return ERROR_PRINT;
    }

    free(all_songs);
    free(right_songs);

    return 0;
}
