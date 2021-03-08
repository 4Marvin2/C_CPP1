#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FIELD_LENGTH 101
#define NUMBER_OF_FIELDS 4
#define MAX_STR_LENGTH MAX_FIELD_LENGTH * NUMBER_OF_FIELDS + 4 // + 4 - знаки разделители и конец строки/перевод строки
#define INCREASING_THE_ARRAY 2
#define MIN_ARRAY_SIZE 2

#define ERROR_EMPTY_LINE 1
#define ERROR_WRONG_NUMBER_OF_VARIABLE_IN_LINE 2
#define FILE_OPEN_ERROR -1
#define ERROR_EMPTY_ARRAY -2

typedef struct Song {
    char author[MAX_FIELD_LENGTH];
    char singer[MAX_FIELD_LENGTH];
    char name[MAX_FIELD_LENGTH];
    char duration[MAX_FIELD_LENGTH];
} Song;

// парсер строк
int pars_str(char *str, char sep, char **result);

// инициализация песни
int init_song(FILE *fp, Song *current_song);

// пересоздание массива большего размера
int realloc_array(Song **arr, int size);

// считывание песен из файла
int read_data_from_file(char name[], Song **my_songs_list);

// поиск всех нужных песен по автору
int search_by_author(Song *all_songs, int songs_count, char *my_author, Song **right_songs);

// печать песен
void print_songs(Song *my_song_list, int songs_count);
