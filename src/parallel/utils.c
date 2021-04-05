// Copyright 2021 Mikhail Popov

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#include "utils.h"
#include "general/utils.h"

// структура для хранения массива максимальных кол-в вхождений подстроки и максимальных длин подстрок в подмассивах
typedef struct max_length_by_section {
    int *arr;
    int *max_length;
} max_length_by_section;

// принимает имя файла, указатель на массив, в который запишутся данные из файла, файловый дескриптор
// создает расшареный файл
// возвращает размер считанных данных или код ошибки в случае неудачи
int create_shared_file(const char *filename, char **arr, int fd) {
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
    *arr = (char *)mmap(NULL, arr_size, PROT_READ, MAP_SHARED, fd, 0);

    if ((*arr) == MAP_FAILED) {
        close(fd);
        return MMAP_FAILED;
    }

    size_t size = strlen(*arr) + 1;
    return size;
}

// принимает указатель на массив, по которому будет выделятся память, и размер массива
// создает расшаренный массив, инициализированный нулями
// возвращает код ошибки либо CORRECT, если все успешно
int create_shared_memory(int **arr, int size) {
    if (arr == NULL) {
        return NULL_PTR;
    }
    int arr_size;
    if (size % getpagesize() == 0) {
        arr_size = size * (sizeof(int));
    } else {
        arr_size = ((size * sizeof(int)) / getpagesize()) * getpagesize() + getpagesize();
    }
    *arr = (int *)mmap(NULL, arr_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if ((*arr) == MAP_FAILED) {
        return MMAP_FAILED;
    }

    for (int i = 0; i < size; i++) {
        (*arr)[i] = 0;
    }

    return CORRECT;
}

// принимает массив, хранящий индексы разделения символьного массива,
// количество процессов, символьный массив, его размер
// находит разбиение символьного массива (индексы) в зависимости от кол-ва процессов
// возвращает код ошибки либо CORRECT, если все успешно
int finding_partition_of_arr_char(int *arr_splitting, int number_of_processes, const char *my_arr, int size) {
    if ((arr_splitting == NULL) || (my_arr == NULL)) {
        return NULL_PTR;
    }
    int number_of_free_processes = number_of_processes;
    int mid;
    int offset = 0;
    int step = 1;
    for (int i = 0; i < number_of_processes; i++) {
        if (i == number_of_processes - 1) {
            arr_splitting[i] = size;
        } else {
            mid = offset + ((size - 1 - offset) / number_of_free_processes);
            char middle_symbol = my_arr[mid];
            char prev_symbol = my_arr[mid - step];
            char next_symbol = my_arr[mid + step];
            while ((middle_symbol == prev_symbol) &&
                   (middle_symbol == next_symbol) &&
                   (mid - step >= 0) &&
                   (mid + step <= size)) {
                step++;
                prev_symbol = my_arr[mid - step];
                next_symbol = my_arr[mid + step];
            }
            if (middle_symbol != prev_symbol) {
                mid = mid - step + 1;
            } else {
                mid = mid + step;
            }
            step = 1;
            offset = mid;
            arr_splitting[i] = mid;
        }
        number_of_free_processes--;
    }

    return CORRECT;
}

// принимает массив, хранящий индексы разделения числового массива, количество процессов, числовой массив и его размер
// находит разбиение числового массива (индексы) в зависимости от кол-ва процессов
// возвращает код ошибки либо CORRECT, если все успешно
int finding_partition_of_arr_int(int *arr_splitting, int number_of_processes, const int *my_arr, size_t size) {
    if ((arr_splitting == NULL) || (my_arr == NULL)) {
        return NULL_PTR;
    }
    int number_of_free_processes = number_of_processes;
    int mid;
    int offset = 0;
    for (int i = 0; i < number_of_processes; i++) {
        if (i == number_of_processes - 1) {
            arr_splitting[i] = size;
        } else {
            mid = offset + ((size - offset) / number_of_free_processes);
            offset = mid;
            arr_splitting[i] = mid;
        }
        number_of_free_processes--;
    }

    return CORRECT;
}

// принимает кол-во процессов и массив для записи их PID
// создает дочерние процессы, родительский процесс сохраняет PIDы дочерних процессов
// возвращает код ошибки либо CORRECT, если все успешно
int create_forks(int number_of_processes, int *pids) {
    if (pids == NULL) {
        return NULL_PTR;
    }
    pid_t pid;
    for (int i = 0; i < number_of_processes; i++) {
        if ((pid = fork()) == -1) {
            return FORK_FAILED;
        }
        if (pid == 0) {
            return i;
        }
        pids[i] = pid;
    }
    return PARENT_PID;
}

// принимает символьный массив, массив разбиения символьного массива, массив-счетчик, кол-во процессов
// ищет количество повторяющихся символов и записывает в счетчик по индексу, равному длинне подстроки
// каждый процесс ищет в своем секторе основного массива
// возвращает код ошибки либо CORRECT, если все успешно
int search_number_of_repeating_length_parallel(char *my_arr,
                                               int *arr_splitting,
                                               int *arr_counter,
                                               int processes_number) {
    if ((my_arr == NULL) || (arr_splitting == NULL) || (arr_counter == NULL)) {
        return NULL_PTR;
    }

    int start_position;
    int end_position;
    if (processes_number == 0) {
        start_position = 0;
    } else {
        start_position = arr_splitting[processes_number - 1];
    }
    end_position = arr_splitting[processes_number];
    int temp_count = 1;
    for (int i = start_position; i < end_position; i++) {
        char previous = my_arr[i];
        char current = my_arr[i + 1];
        if (current == previous) {
            temp_count++;
        } else {
            arr_counter[temp_count]++;
            temp_count = 1;
        }
    }

    return CORRECT;
}

// принимает массив, хранящий максимальные кол-ва вхождений подстроки и максимальные длины подстрок в подмассивах,
// массив разбиений, массив-счетчик, кол-во процессов
// находит максимальные элементы на каждом участке основного массива и соответствующии им значения длин
// каждый процесс ищет в своем секторе основного массива
// возвращает код ошибки либо CORRECT, если все успешно
int search_max_parallel(max_length_by_section *arr_max,
                        int *arr_splitting_for_max,
                        int *arr_counter,
                        int processes_number) {
    if ((arr_max == NULL) || (arr_splitting_for_max == NULL)) {
        return NULL_PTR;
    }
    int most_frequent_length_count = 0;
    int start_position;
    int end_position;
    if (processes_number == 0) {
        start_position = 2;
    } else {
        start_position = arr_splitting_for_max[processes_number - 1];
    }
    if (start_position < 2) {
        start_position = 2;
    }
    end_position = arr_splitting_for_max[processes_number];
    for (int i = start_position; i < end_position; i++) {
        if (arr_counter[i] > most_frequent_length_count) {
            most_frequent_length_count = arr_counter[i];
            arr_max->arr[processes_number] = arr_counter[i];
            arr_max->max_length[processes_number] = i;
        }
    }

    return CORRECT;
}

// принимает файловый дескриптор, символьный массив, его размер, массив-счетчик, массив разбиения, кол-во процессов
// очищает ресурсы
// возвращает код ошибки либо CORRECT, если все успешно
int free_main_resources(int fd, char *my_arr, int size, int *arr_counter, int *arr_splitting, int number_of_processes) {
    close(fd);
    int err = munmap(my_arr, size - 1);
    err += munmap(arr_splitting, number_of_processes);
    err += munmap(arr_counter, size);
    return err;
}

// принимает массив структур максимумов, массив разбиения, кол-во процессов
// очищает ресурсы, которые используются для поиска максимума
// возвращает код ошибки либо CORRECT, если все успешно
int free_resources_for_max(max_length_by_section *arr_max, int *arr_splitting_for_max, int number_of_processes) {
    int err = munmap(arr_max->arr, number_of_processes);
    err += munmap(arr_max->max_length, number_of_processes);
    err += munmap(arr_splitting_for_max, number_of_processes);
    return err;
}

// принимает файловый дескриптор, символьный массив, его размер, массив-счетчик, массив разбиения, кол-во процессов
// создает дочерние процессы
// и каждый процесс на своем участке основного массива ищет кол-во вхождений повторяющихся символов
// возвращает код ошибки либо CORRECT, если все успешно
__attribute__((__always_inline__)) inline int search_arr_of_repeating_length_parallel(int fd,
                                                                                      char *my_arr,
                                                                                      size_t size,
                                                                                      int *arr_counter,
                                                                                      int *arr_splitting,
                                                                                      int number_of_processes) {
    pid_t *pids = (pid_t *)malloc(sizeof(pid_t) * number_of_processes);
    if (pids == NULL) {
        free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
        return NULL_PTR;
    }

    int processes_number = create_forks(number_of_processes, pids);
    if ((processes_number == NULL_PTR) || (processes_number == FORK_FAILED)) {
        int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
        if (processes_number == PARENT_PID) {
            for (int i = 0; i < number_of_processes; i++) {
                while (waitpid(pids[i], NULL, 0) > 0) {}
            }
        }
        free(pids);
        if (err != CORRECT) {
            return err;
        }
        return processes_number;
    }

    if (processes_number != PARENT_PID) {
        int condition_of_function = search_number_of_repeating_length_parallel(my_arr,
                                                                               arr_splitting,
                                                                               arr_counter,
                                                                               processes_number);
        if (condition_of_function != CORRECT) {
            free(pids);
            int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
            exit(err);
        }
        free(pids);
        exit(CORRECT);
    }

    for (int i = 0; i < number_of_processes; i++) {
        while (waitpid(pids[i], NULL, 0) > 0) {}
    }

    free(pids);

    return CORRECT;
}

// принимает файловый дескриптор, символьный массив, его размер, массив-счетчик, массив разбиения,
// массив структур максимумов, массив разбиения для поиска максимума, кол-во процессов
// создает дочернии процессы и каждый процесс на своем участке массива ищет самую часто повторяющуюся длину подстроки
// возвращает код ошибки либо CORRECT, если все успешно
__attribute__((__always_inline__)) inline int search_arr_max_parallel(int fd,
                                                                      char *my_arr,
                                                                      size_t size,
                                                                      int *arr_counter,
                                                                      int *arr_splitting,
                                                                      max_length_by_section *arr_max,
                                                                      int *arr_splitting_for_max,
                                                                      int number_of_processes) {
    pid_t *pids = (pid_t *)malloc(sizeof(pid_t) * number_of_processes);
    if (pids == NULL) {
        int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
        if (err != CORRECT) {
            return err;
        }
        return NULL_PTR;
    }

    int processes_number = create_forks(number_of_processes, pids);
    if ((processes_number == NULL_PTR) || (processes_number == FORK_FAILED)) {
        free(pids);
        if (processes_number == PARENT_PID) {
            for (int i = 0; i < number_of_processes; i++) {
                while (waitpid(pids[i], NULL, 0) > 0) {}
            }
        }
        int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
        err += free_resources_for_max(arr_max, arr_splitting_for_max, number_of_processes);
        if (err != CORRECT) {
            return err;
        }
        return processes_number;
    }

    if (processes_number != PARENT_PID) {
        int condition_of_function = search_max_parallel(arr_max, arr_splitting_for_max, arr_counter, processes_number);
        if (condition_of_function != CORRECT) {
            free(pids);
            int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
            err += free_resources_for_max(arr_max, arr_splitting_for_max, number_of_processes);
            if (err != CORRECT) {
                return err;
            }
        }
        exit(CORRECT);
    }

    for (int i = 0; i < number_of_processes; i++) {
        while (waitpid(pids[i], NULL, 0) > 0) {}
    }

    free(pids);

    return CORRECT;
}

// принимает имя файла и указатель на массив, куда будет записана результирующая подстрока
// находит представителя серии символов самой часто встречаемой длины
// возвращает код ошибки либо CORRECT, если все успешно
int search_substring_of_the_most_common_length(const char *filename, char **result) {
    char *my_arr = NULL;
    int fd = 0;
    int size = create_shared_file(filename, &my_arr, fd);
    if (size < 0) {
        return size;  // возвращаю код ошибки
    }

    int *arr_counter = NULL;
    int condition_of_function = create_shared_memory(&arr_counter, size);
    if (condition_of_function != CORRECT) {
        close(fd);
        int err = munmap(my_arr, size - 1);
        err += munmap(arr_counter, size);
        if (err != CORRECT) {
            return err;
        }
        return condition_of_function;
    }

    int number_of_processes = sysconf(_SC_NPROCESSORS_ONLN);

    int *arr_splitting = NULL;
    condition_of_function = create_shared_memory(&arr_splitting, number_of_processes);
    if (condition_of_function != CORRECT) {
        close(fd);
        int err = munmap(my_arr, size - 1);
        err += munmap(arr_counter, size);
        if (err != CORRECT) {
            return err;
        }
        return condition_of_function;
    }

    condition_of_function = finding_partition_of_arr_char(arr_splitting, number_of_processes, my_arr, size);
    if (condition_of_function != CORRECT) {
        int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
        if (err != CORRECT) {
            return err;
        }
        return condition_of_function;
    }

    condition_of_function = search_arr_of_repeating_length_parallel(fd,
                                                                    my_arr,
                                                                    size,
                                                                    arr_counter,
                                                                    arr_splitting,
                                                                    number_of_processes);
    if (condition_of_function != CORRECT) {
        int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
        if (err != CORRECT) {
            return err;
        }
        return condition_of_function;
    }

    max_length_by_section arr_max;
    condition_of_function = create_shared_memory(&(arr_max.arr), number_of_processes);
    if (condition_of_function != CORRECT) {
        int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
        if (err != CORRECT) {
            return err;
        }
        return condition_of_function;
    }
    condition_of_function = create_shared_memory(&(arr_max.max_length), number_of_processes);
    if (condition_of_function != CORRECT) {
        int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
        err += munmap(arr_max.arr, number_of_processes);
        if (err != CORRECT) {
            return err;
        }
        return condition_of_function;
    }

    int *arr_splitting_for_max = NULL;
    condition_of_function = create_shared_memory(&arr_splitting_for_max, number_of_processes);
    if (condition_of_function != CORRECT) {
        int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
        err += munmap(arr_max.arr, number_of_processes);
        err += munmap(arr_max.max_length, number_of_processes);
        if (err != CORRECT) {
            return err;
        }
        return condition_of_function;
    }

    condition_of_function = finding_partition_of_arr_int(arr_splitting_for_max,
                                                         number_of_processes,
                                                         arr_counter,
                                                         size);

    if (condition_of_function != CORRECT) {
        int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
        err += munmap(&arr_max, number_of_processes);
        if (err != CORRECT) {
            return err;
        }
        return condition_of_function;
    }

    condition_of_function = search_arr_max_parallel(fd,
                                                    my_arr,
                                                    size,
                                                    arr_counter,
                                                    arr_splitting,
                                                    &arr_max,
                                                    arr_splitting_for_max,
                                                    number_of_processes);

    if (condition_of_function != CORRECT) {
        int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
        err += free_resources_for_max(&arr_max, arr_splitting_for_max, number_of_processes);
        if (err != CORRECT) {
            return err;
        }
        return condition_of_function;
    }

    int most_frequent_value = search_max(arr_max.arr, number_of_processes);
    int most_frequent_length = arr_max.max_length[most_frequent_value];
    if (most_frequent_length < 0) {
        int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
        err += free_resources_for_max(&arr_max, arr_splitting_for_max, number_of_processes);
        if (err != CORRECT) {
            return err;
        }
        return most_frequent_length;
    }

    int first_position_of_substring = search_first_occurrence_of_substring(my_arr, size, most_frequent_length);
    if (first_position_of_substring < 0) {
        int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
        err += free_resources_for_max(&arr_max, arr_splitting_for_max, number_of_processes);
        if (err != CORRECT) {
            return err;
        }
        return first_position_of_substring;
    }

    if (*result == NULL) {
        (*result) = (char *)malloc(sizeof(char) * (most_frequent_length + 1));
        if (*result == NULL) {
            int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
            err += free_resources_for_max(&arr_max, arr_splitting_for_max, number_of_processes);
            if (err != CORRECT) {
                return err;
            }
            return NULL_PTR;
        }
    }

    condition_of_function = search_substring(my_arr,
                                             first_position_of_substring,
                                             most_frequent_length,
                                             *result);

    if (condition_of_function != CORRECT) {
        free(*result);
        int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
        err += free_resources_for_max(&arr_max, arr_splitting_for_max, number_of_processes);
        if (err != CORRECT) {
            return err;
        }
        return condition_of_function;
    }

    condition_of_function = output(my_arr, first_position_of_substring, most_frequent_length, stdout);
    if (condition_of_function != CORRECT) {
        free(*result);
        int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
        err += free_resources_for_max(&arr_max, arr_splitting_for_max, number_of_processes);
        if (err != CORRECT) {
            return err;
        }
        return condition_of_function;
    }

    int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
    err += free_resources_for_max(&arr_max, arr_splitting_for_max, number_of_processes);
    if (err != CORRECT) {
        return err;
    }

    return CORRECT;
}
