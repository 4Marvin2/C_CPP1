// Copyright 2021 Mikhail Popov

#include "utils/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
#define NULL_PTR -3
#define NULL_PTR_REALLOC -4
#define NULL_SIZE -5
#define NULL_SIZE_REALLOC -6

int pars_str(char *str, char sep, char **result) {
    int i = 0;
    int j = 0;
    int k = 0;
    int recorded_values_number = 0;
    while ((str[i] != '\0') && (str[i] != '\n')) {
        if (str[i] == sep) {
            result[j][k] = '\0';
            if (strncmp(result[j], "", MAX_FIELD_LENGTH) == 0) {
                recorded_values_number--;
            }
            j++;
            k = 0;
            recorded_values_number++;
        } else {
            result[j][k] = str[i];
            k++;
            }
        i++;
    }
    result[j][k] = '\0';
    if (strncmp(result[j], "", MAX_FIELD_LENGTH) == 0) {
        recorded_values_number--;
    }

    recorded_values_number++;

    return recorded_values_number;
}

int read_str_from_file(FILE *fp, char *str) {
    if (fgets(str, MAX_STR_LENGTH, fp) == NULL) {
        return EOF;
    }

    if (str[0] == '\n') {
        return ERROR_EMPTY_LINE;
    }

    return 0;
}

int init_song(FILE *fp, Song *current_song) {
    char current_str[MAX_STR_LENGTH];
    int read_data_from_file_condition = read_str_from_file(fp, current_str);

    if (read_data_from_file_condition == EOF) {
        return EOF;
    }

    if (read_data_from_file_condition == ERROR_EMPTY_LINE) {
        return ERROR_EMPTY_LINE;
    }

    char **parsed_str = malloc(sizeof(char *) * NUMBER_OF_FIELDS);
    for (int i = 0; i < NUMBER_OF_FIELDS; i++) {
        parsed_str[i] = malloc(sizeof(char) * MAX_FIELD_LENGTH);
    }

    int recorded_values_number = pars_str(current_str, ';', parsed_str);
    if (recorded_values_number != NUMBER_OF_FIELDS) {
        for (int i = 0; i < NUMBER_OF_FIELDS; i++) {
            free(parsed_str[i]);
        }
        free(parsed_str);
        return ERROR_WRONG_NUMBER_OF_VARIABLE_IN_LINE;
    }

    strncpy(current_song->author, parsed_str[0], MAX_FIELD_LENGTH);
    strncpy(current_song->singer, parsed_str[1], MAX_FIELD_LENGTH);
    strncpy(current_song->name, parsed_str[2], MAX_FIELD_LENGTH);
    strncpy(current_song->duration, parsed_str[3], MAX_FIELD_LENGTH);

    for (int i = 0; i < NUMBER_OF_FIELDS; i++) {
        free(parsed_str[i]);
    }
    free(parsed_str);

    return 0;
}

int realloc_array(Song **arr, int size) {
    if (size == 0) {
        return NULL_SIZE;
    }
    Song *temp_arr = (Song *)malloc(sizeof(Song) * size * INCREASING_THE_ARRAY);
    if (temp_arr ==  NULL) {
        return NULL_PTR_REALLOC;
    }
    // перекопируем
    memcpy(temp_arr, (*arr), sizeof(Song) * size);
    free(*arr);
    // переназначаем изначальный массив
    (*arr) = temp_arr;
    size *= INCREASING_THE_ARRAY;

    return size;
}

int read_data_from_file(char name[], Song **my_songs_list) {
    FILE *fp;
    if ((fp = fopen(name, "r")) == NULL) {
        return FILE_OPEN_ERROR;
    }

    Song current_song;
    int all_songs_count = MIN_ARRAY_SIZE;
    Song *all_songs = malloc(sizeof(Song) * all_songs_count);
    if (all_songs == NULL) {
        fclose(fp);
        return NULL_PTR;
    }
    int added_songs_count = 0;
    int init_song_condition = init_song(fp, &current_song);
    while (init_song_condition != EOF) {
        if (init_song_condition == 0) {
            all_songs[added_songs_count] = current_song;
            added_songs_count++;
            // если достигли конца массива, то расширяем
            if (added_songs_count == all_songs_count) {
                all_songs_count = realloc_array(&all_songs, all_songs_count);
                if (all_songs_count == NULL_PTR) {
                    free(all_songs);
                    fclose(fp);
                    return NULL_PTR_REALLOC;
                }
                if (all_songs_count == NULL_SIZE) {
                    free(all_songs);
                    fclose(fp);
                    return NULL_SIZE_REALLOC;
                }
            }
        }

        init_song_condition = init_song(fp, &current_song);
    }

    fclose(fp);

    if (added_songs_count == 0) {
        free(all_songs);
        return ERROR_EMPTY_ARRAY;
    }

    *my_songs_list = all_songs;



    return added_songs_count;
}

int search_by_author(Song *all_songs, int songs_count, char *my_author, Song **right_songs) {
    Song *result = malloc(sizeof(Song) * MIN_ARRAY_SIZE);
    if (result == NULL) {
        return NULL_PTR;
    }
    int added_songs_count = 0;
    int all_songs_count = MIN_ARRAY_SIZE;
    for (int i = 0; i < songs_count; i++) {
        if (strncmp(all_songs[i].author, my_author, MAX_FIELD_LENGTH) == 0) {
            result[added_songs_count] = all_songs[i];
            added_songs_count++;
            // если достигли конца массива, то расширяем
            if (all_songs_count == added_songs_count) {
                all_songs_count = realloc_array(&result, all_songs_count);
                if (all_songs_count == NULL_PTR) {
                    free(result);
                    return NULL_PTR_REALLOC;
                }
                if (all_songs_count == NULL_SIZE) {
                    free(result);
                    return NULL_SIZE_REALLOC;
                }
            }
        }
    }

    if (added_songs_count == 0) {
        free(result);
        return ERROR_EMPTY_ARRAY;
    }

    *right_songs = result;



    return added_songs_count;
}

void print_songs(Song *my_song_list, int songs_count, FILE *stream) {
    for (int i = 0; i < songs_count; i++) {
        fprintf(stream, "%s: %s | %s: %s | %s: %s | %s: %s\n",
        "Автор", my_song_list[i].author,
        "Исполнитель", my_song_list[i].singer,
        "Название", my_song_list[i].name,
        "Длительность", my_song_list[i].duration);
    }
}
