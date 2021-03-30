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

#include "parallel/utils.h"

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

int finding_number_of_processes(size_t size) {
    if (size < 100) {
        return 1;
    }
    if (size < 1000) {
        return 2;
    }
    if (size < 5000) {
        return 3;
    }
    if (size < 10000) {
        return 4;
    }
    if (size < 20000) {
        return 6;
    }
    if (size < 100000) {
        return 7;
    }
    return 10;
}

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

int search_number_of_repeating_length_parallel(char *my_arr,
                                               int *arr_splitting,
                                               int *arr_counter,
                                               int processes_number,
                                               pthread_mutex_t mutex) {
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
            pthread_mutex_lock(&mutex);
            arr_counter[temp_count]++;
            pthread_mutex_unlock(&mutex);
            temp_count = 1;
        }
    }

    return CORRECT;
}

int search_max_parallel(int *arr_max, int *arr_splitting_for_max, int *arr_counter, int processes_number) {
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
    end_position = arr_splitting_for_max[processes_number];
    for (int i = start_position; i < end_position; i++) {
        if (arr_counter[i] > most_frequent_length_count) {
            most_frequent_length_count = arr_counter[i];
            arr_max[processes_number] = i;
        }
    }

    return CORRECT;
}

int search_max(int *arr, int size) {
    if (arr == NULL) {
        return NULL_PTR;
    }
    int max = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }

    return max;
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

int free_main_resources(int fd, char *my_arr, int size, int *arr_counter, int *arr_splitting, int number_of_processes) {
    close(fd);
    int err = munmap(my_arr, size - 1);
    err += munmap(arr_splitting, number_of_processes);
    err += munmap(arr_counter, size);
    return err;
}

int free_resources_for_max(int *arr_max, int *arr_splitting_for_max, int number_of_processes) {
    int err = munmap(arr_max, number_of_processes);
    err += munmap(arr_splitting_for_max, number_of_processes);
    return err;
}

int search_arr_of_repeating_length_parallel(int fd,
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

    pthread_mutexattr_t attr;
    pthread_mutex_t mutex;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&mutex, &attr);

    if (processes_number != PARENT_PID) {
        int condition_of_function = search_number_of_repeating_length_parallel(my_arr,
                                                                               arr_splitting,
                                                                               arr_counter,
                                                                               processes_number,
                                                                               mutex);
        if (condition_of_function != CORRECT) {
            free(pids);
            int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
            exit(err);
        }
        free(pids);
        pthread_mutexattr_destroy(&attr);
        pthread_mutex_destroy(&mutex);
        exit(CORRECT);
    }

    for (int i = 0; i < number_of_processes; i++) {
        while (waitpid(pids[i], NULL, 0) > 0) {}
    }

    pthread_mutexattr_destroy(&attr);
    pthread_mutex_destroy(&mutex);

    free(pids);

    return CORRECT;
}

int search_arr_max_parallel(int fd,
                            char *my_arr,
                            size_t size,
                            int *arr_counter,
                            int *arr_splitting,
                            int *arr_max,
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

int search_substring_of_the_most_common_length_parallel(const char *filename, char **result) {
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

    int number_of_processes = finding_number_of_processes(size);

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

    int *arr_max = NULL;
    condition_of_function = create_shared_memory(&arr_max, number_of_processes);
    if (condition_of_function != CORRECT) {
        int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
        if (err != CORRECT) {
            return err;
        }
        return condition_of_function;
    }

    int *arr_splitting_for_max = NULL;
    condition_of_function = create_shared_memory(&arr_splitting_for_max, number_of_processes);
    if (condition_of_function != CORRECT) {
        int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
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
        err += munmap(arr_max, number_of_processes);
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
                                                    arr_max,
                                                    arr_splitting_for_max,
                                                    number_of_processes);
    if (condition_of_function != CORRECT) {
        int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
        err += free_resources_for_max(arr_max, arr_splitting_for_max, number_of_processes);
        if (err != CORRECT) {
            return err;
        }
        return condition_of_function;
    }

    int most_frequent_length = search_max(arr_max, number_of_processes);
    if (most_frequent_length < 0) {
        int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
        err += free_resources_for_max(arr_max, arr_splitting_for_max, number_of_processes);
        if (err != CORRECT) {
            return err;
        }
        return most_frequent_length;
    }

    int first_position_of_substring = search_first_occurrence_of_substring(my_arr, size, most_frequent_length);
    if (first_position_of_substring < 0) {
        int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
        err += free_resources_for_max(arr_max, arr_splitting_for_max, number_of_processes);
        if (err != CORRECT) {
            return err;
        }
        return first_position_of_substring;
    }

    if (*result == NULL) {
        (*result) = (char *)malloc(sizeof(char) * (most_frequent_length + 1));
        if (*result == NULL) {
            int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
            err += free_resources_for_max(arr_max, arr_splitting_for_max, number_of_processes);
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
        err += free_resources_for_max(arr_max, arr_splitting_for_max, number_of_processes);
        if (err != CORRECT) {
            return err;
        }
        return condition_of_function;
    }

    condition_of_function = output(my_arr, first_position_of_substring, most_frequent_length, stdout);
    if (condition_of_function != CORRECT) {
        free(*result);
        int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
        err += free_resources_for_max(arr_max, arr_splitting_for_max, number_of_processes);
        if (err != CORRECT) {
            return err;
        }
        return condition_of_function;
    }

    int err = free_main_resources(fd, my_arr, size, arr_counter, arr_splitting, number_of_processes);
    err += free_resources_for_max(arr_max, arr_splitting_for_max, number_of_processes);
    if (err != CORRECT) {
        return err;
    }

    return CORRECT;
}
