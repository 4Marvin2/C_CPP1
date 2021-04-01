// Copyright 2021 Mikhail Popov

#include <time.h>
#include <sys/time.h>
#include <dlfcn.h>

#include "utils.h"

int main() {
    void *handle = dlopen("./libmost_frequent_seqence_parallel.so", RTLD_LAZY);
    if (!handle) {
        return 1;
    }

    char *result_parallel = NULL;
    const char *filename = "text.txt";
    int (*funcpt)();
    *(void **)(&funcpt) = dlsym(handle, "search_substring_of_the_most_common_length");
    struct timeval start;
    struct timeval end;

    gettimeofday(&start, NULL);
    int processes_number = (*funcpt)(filename, &result_parallel);
    gettimeofday(&end, NULL);

    dlclose(handle);

    if (processes_number != CORRECT) {
        free(result_parallel);
        return 1;
    }
    
    long time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("Время многопроцессорной версии: %ld миллисекунд\n", time);

    char *result_serial = NULL;

    gettimeofday(&start, NULL);
    int condition_of_function = search_substring_of_the_most_common_length(filename, &result_serial);
    gettimeofday(&end, NULL);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        free(result_serial);
        return 1;
    }

    time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("Время однопроцессорной версии: %ld миллисекунд\n", time);

    if (strncmp(result_serial, result_parallel, strlen(result_parallel)) == 0) {
        printf("Результаты последовательной и параллельной реализации совпадают\n");
    } else {
        printf("Результаты последовательной и параллельной реализации не совпадают\n");
    }

    free(result_serial);
    free(result_parallel);

    return 0;
}
