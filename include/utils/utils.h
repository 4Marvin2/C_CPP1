#ifndef INCLUDE_UTILS_UTILS_H_
#define INCLUDE_UTILS_UTILS_H_

#include <stdio.h>

#define MAX_FIELD_LENGTH 101

typedef struct Song {
    char author[MAX_FIELD_LENGTH];
    char singer[MAX_FIELD_LENGTH];
    char name[MAX_FIELD_LENGTH];
    char duration[MAX_FIELD_LENGTH];
} Song;

// парсер строк
int pars_str(char *str, char sep, char **result);

// чтение строки из файла
int read_str_from_file(FILE *fp, char *str);

// инициализация песни
int init_song(FILE *fp, Song *current_song);

// пересоздание массива большего размера
int realloc_array(Song **arr, int size);

// считывание песен из файла
int read_data_from_file(char name[], Song **my_songs_list);

// поиск всех нужных песен по автору
int search_by_author(Song *all_songs, int songs_count, char *my_author, Song **right_songs);

// печать песен
int print_songs(Song *my_song_list, int songs_count, FILE *stream);

#endif  // INCLUDE_UTILS_UTILS_H_
