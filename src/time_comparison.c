// Copyright 2021 Mikhail Popov

#include <dlfcn.h>
#include <time.h>
#include <sys/time.h>

#include "utils.h"

int test_comparison_size_50() {
    void *handle = dlopen("./libmost_frequent_seqence_parallel.so", RTLD_LAZY);
    if (!handle) {
        return 1;
    }

    const char *filename = "test/size_50";
    char *result_parallel = NULL;

    int (*funcpt)(const char *, char **);
    *(void **)(&funcpt) = dlsym(handle, "search_substring_of_the_most_common_length");
    struct timeval start;
    struct timeval end;

    gettimeofday(&start, NULL);
    int condition_of_function = (*funcpt)(filename, &result_parallel);
    gettimeofday(&end, NULL);

    dlclose(handle);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        return 1;
    }

    long time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("Время многопроцессорной версии: %ld миллисекунд\n", time);

    char *result_serial = NULL;

    gettimeofday(&start, NULL);
    condition_of_function = search_substring_of_the_most_common_length(filename, &result_serial);
    gettimeofday(&end, NULL);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        free(result_serial);
        return 1;
    }

    time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("Время однопроцессорной версии: %ld миллисекунд\n", time);

    free(result_serial);
    free(result_parallel);

    return CORRECT;
}

int test_comparison_size_150() {
    void *handle = dlopen("./libmost_frequent_seqence_parallel.so", RTLD_LAZY);
    if (!handle) {
        return 1;
    }

    const char *filename = "test/size_150";
    char *result_parallel = NULL;

    int (*funcpt)(const char *, char **);
    *(void **)(&funcpt) = dlsym(handle, "search_substring_of_the_most_common_length");
    struct timeval start;
    struct timeval end;

    gettimeofday(&start, NULL);
    int condition_of_function = (*funcpt)(filename, &result_parallel);
    gettimeofday(&end, NULL);

    dlclose(handle);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        return 1;
    }

    long time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("Время многопроцессорной версии: %ld миллисекунд\n", time);

    char *result_serial = NULL;

    gettimeofday(&start, NULL);
    condition_of_function = search_substring_of_the_most_common_length(filename, &result_serial);
    gettimeofday(&end, NULL);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        free(result_serial);
        return 1;
    }

    time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("Время однопроцессорной версии: %ld миллисекунд\n", time);

    free(result_serial);
    free(result_parallel);

    return CORRECT;
}

int test_comparison_size_2500() {
    void *handle = dlopen("./libmost_frequent_seqence_parallel.so", RTLD_LAZY);
    if (!handle) {
        return 1;
    }

    const char *filename = "test/size_2500";
    char *result_parallel = NULL;

    int (*funcpt)(const char *, char **);
    *(void **)(&funcpt) = dlsym(handle, "search_substring_of_the_most_common_length");
    struct timeval start;
    struct timeval end;

    gettimeofday(&start, NULL);
    int condition_of_function = (*funcpt)(filename, &result_parallel);
    gettimeofday(&end, NULL);

    dlclose(handle);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        return 1;
    }

    long time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("Время многопроцессорной версии: %ld миллисекунд\n", time);

    char *result_serial = NULL;

    gettimeofday(&start, NULL);
    condition_of_function = search_substring_of_the_most_common_length(filename, &result_serial);
    gettimeofday(&end, NULL);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        free(result_serial);
        return 1;
    }

    time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("Время однопроцессорной версии: %ld миллисекунд\n", time);

    free(result_serial);
    free(result_parallel);

    return CORRECT;
}

int test_comparison_size_7500() {
    void *handle = dlopen("./libmost_frequent_seqence_parallel.so", RTLD_LAZY);
    if (!handle) {
        return 1;
    }

    const char *filename = "test/size_7500";
    char *result_parallel = NULL;

    int (*funcpt)(const char *, char **);
    *(void **)(&funcpt) = dlsym(handle, "search_substring_of_the_most_common_length");
    struct timeval start;
    struct timeval end;

    gettimeofday(&start, NULL);
    int condition_of_function = (*funcpt)(filename, &result_parallel);
    gettimeofday(&end, NULL);

    dlclose(handle);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        return 1;
    }

    long time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("Время многопроцессорной версии: %ld миллисекунд\n", time);

    char *result_serial = NULL;

    gettimeofday(&start, NULL);
    condition_of_function = search_substring_of_the_most_common_length(filename, &result_serial);
    gettimeofday(&end, NULL);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        free(result_serial);
        return 1;
    }

    time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("Время однопроцессорной версии: %ld миллисекунд\n", time);

    free(result_serial);
    free(result_parallel);

    return CORRECT;
}

int test_comparison_size_15000() {
    void *handle = dlopen("./libmost_frequent_seqence_parallel.so", RTLD_LAZY);
    if (!handle) {
        return 1;
    }

    const char *filename = "test/size_15000";
    char *result_parallel = NULL;

    int (*funcpt)(const char *, char **);
    *(void **)(&funcpt) = dlsym(handle, "search_substring_of_the_most_common_length");
    struct timeval start;
    struct timeval end;

    gettimeofday(&start, NULL);
    int condition_of_function = (*funcpt)(filename, &result_parallel);
    gettimeofday(&end, NULL);

    dlclose(handle);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        return 1;
    }

    long time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("Время многопроцессорной версии: %ld миллисекунд\n", time);

    char *result_serial = NULL;

    gettimeofday(&start, NULL);
    condition_of_function = search_substring_of_the_most_common_length(filename, &result_serial);
    gettimeofday(&end, NULL);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        free(result_serial);
        return 1;
    }

    time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("Время однопроцессорной версии: %ld миллисекунд\n", time);

    free(result_serial);
    free(result_parallel);

    return CORRECT;
}

int test_comparison_size_50000() {
    void *handle = dlopen("./libmost_frequent_seqence_parallel.so", RTLD_LAZY);
    if (!handle) {
        return 1;
    }

    const char *filename = "test/size_50000";
    char *result_parallel = NULL;

    int (*funcpt)(const char *, char **);
    *(void **)(&funcpt) = dlsym(handle, "search_substring_of_the_most_common_length");
    struct timeval start;
    struct timeval end;

    gettimeofday(&start, NULL);
    int condition_of_function = (*funcpt)(filename, &result_parallel);
    gettimeofday(&end, NULL);

    dlclose(handle);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        return 1;
    }

    long time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("Время многопроцессорной версии: %ld миллисекунд\n", time);

    char *result_serial = NULL;

    gettimeofday(&start, NULL);
    condition_of_function = search_substring_of_the_most_common_length(filename, &result_serial);
    gettimeofday(&end, NULL);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        free(result_serial);
        return 1;
    }

    time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("Время однопроцессорной версии: %ld миллисекунд\n", time);

    free(result_serial);
    free(result_parallel);

    return CORRECT;
}

int test_comparison_size_150000() {
    void *handle = dlopen("./libmost_frequent_seqence_parallel.so", RTLD_LAZY);
    if (!handle) {
        return 1;
    }

    const char *filename = "test/size_150000";
    char *result_parallel = NULL;

    int (*funcpt)(const char *, char **);
    *(void **)(&funcpt) = dlsym(handle, "search_substring_of_the_most_common_length");
    struct timeval start;
    struct timeval end;

    gettimeofday(&start, NULL);
    int condition_of_function = (*funcpt)(filename, &result_parallel);
    gettimeofday(&end, NULL);

    dlclose(handle);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        return 1;
    }

    long time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("Время многопроцессорной версии: %ld миллисекунд\n", time);

    char *result_serial = NULL;

    gettimeofday(&start, NULL);
    condition_of_function = search_substring_of_the_most_common_length(filename, &result_serial);
    gettimeofday(&end, NULL);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        free(result_serial);
        return 1;
    }

    time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("Время однопроцессорной версии: %ld миллисекунд\n", time);

    free(result_serial);
    free(result_parallel);

    return CORRECT;
}

int test_comparison_size_100000000() {
    void *handle = dlopen("./libmost_frequent_seqence_parallel.so", RTLD_LAZY);
    if (!handle) {
        return 1;
    }

    const char *filename = "test/size_100000000";
    char *result_parallel = NULL;

    int (*funcpt)(const char *, char **);
    *(void **)(&funcpt) = dlsym(handle, "search_substring_of_the_most_common_length");
    struct timeval start;
    struct timeval end;

    gettimeofday(&start, NULL);
    int condition_of_function = (*funcpt)(filename, &result_parallel);
    gettimeofday(&end, NULL);

    dlclose(handle);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        return 1;
    }

    long time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("Время многопроцессорной версии: %ld миллисекунд\n", time);

    char *result_serial = NULL;

    gettimeofday(&start, NULL);
    condition_of_function = search_substring_of_the_most_common_length(filename, &result_serial);
    gettimeofday(&end, NULL);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        free(result_serial);
        return 1;
    }

    time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("Время однопроцессорной версии: %ld миллисекунд\n", time);

    free(result_serial);
    free(result_parallel);

    return CORRECT;
}

int main() {
    printf("Сравнение времени на файле размеров 50 байт\n");
    test_comparison_size_50();
    printf("Сравнение времени на файле размеров 150 байт\n");
    test_comparison_size_150();
    printf("Сравнение времени на файле размеров 2500 байт\n");
    test_comparison_size_2500();
    printf("Сравнение времени на файле размеров 7500 байт\n");
    test_comparison_size_7500();
    printf("Сравнение времени на файле размеров 15 Кбайт\n");
    test_comparison_size_15000();
    printf("Сравнение времени на файле размеров 50 Кбайт\n");
    test_comparison_size_50000();
    printf("Сравнение времени на файле размеров 150 Кбайт\n");
    test_comparison_size_150000();
    printf("Сравнение времени на файле размеров 100 Мбайт\n");
    test_comparison_size_100000000();
}