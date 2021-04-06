// Copyright 2021 Mikhail Popov

#include <cstdio>
#include <fstream>
#include <iostream>
#include <dlfcn.h>

#include "gtest/gtest.h"

extern "C" {
#include "utils.h"
}

#define TRUE 1

TEST(multi_process, compare_algs_file_size_50) {
    void *handle = dlopen("../libmost_frequent_seqence_parallel.so", RTLD_LAZY);
    if (!handle) {
        exit(EXIT_FAILURE);
    }

    const char *filename = "size_50";
    char *result_parallel = NULL;

    int (*funcpt)(const char *, char **);
    *(void **)(&funcpt) = dlsym(handle, "search_substring_of_the_most_common_length");

    int condition_of_function = (*funcpt)(filename, &result_parallel);

    dlclose(handle);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        exit(EXIT_FAILURE);
    }

    char *result_serial = NULL;

    condition_of_function = search_substring_of_the_most_common_length(filename, &result_serial);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        free(result_serial);
        exit(EXIT_FAILURE);
    }

    EXPECT_STREQ(result_serial, result_parallel);

    free(result_serial);
    free(result_parallel);
}

TEST(multi_process, compare_algs_file_size_150) {
    void *handle = dlopen("../libmost_frequent_seqence_parallel.so", RTLD_LAZY);
    if (!handle) {
        exit(EXIT_FAILURE);
    }

    const char *filename = "size_150";
    char *result_parallel = NULL;

    int (*funcpt)(const char *, char **);
    *(void **)(&funcpt) = dlsym(handle, "search_substring_of_the_most_common_length");

    int condition_of_function = (*funcpt)(filename, &result_parallel);

    dlclose(handle);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        exit(EXIT_FAILURE);
    }

    char *result_serial = NULL;

    condition_of_function = search_substring_of_the_most_common_length(filename, &result_serial);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        free(result_serial);
        exit(EXIT_FAILURE);
    }

    EXPECT_STREQ(result_serial, result_parallel);

    free(result_serial);
    free(result_parallel);
}

TEST(multi_process, compare_algs_file_size_2500) {
    void *handle = dlopen("../libmost_frequent_seqence_parallel.so", RTLD_LAZY);
    if (!handle) {
        exit(EXIT_FAILURE);
    }

    const char *filename = "size_2500";
    char *result_parallel = NULL;

    int (*funcpt)(const char *, char **);
    *(void **)(&funcpt) = dlsym(handle, "search_substring_of_the_most_common_length");

    int condition_of_function = (*funcpt)(filename, &result_parallel);

    dlclose(handle);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        exit(EXIT_FAILURE);
    }

    char *result_serial = NULL;

    condition_of_function = search_substring_of_the_most_common_length(filename, &result_serial);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        free(result_serial);
        exit(EXIT_FAILURE);
    }

    EXPECT_STREQ(result_serial, result_parallel);

    free(result_serial);
    free(result_parallel);
}

TEST(multi_process, compare_algs_file_size_7500) {
    void *handle = dlopen("../libmost_frequent_seqence_parallel.so", RTLD_LAZY);
    if (!handle) {
        exit(EXIT_FAILURE);
    }

    const char *filename = "size_7500";
    char *result_parallel = NULL;

    int (*funcpt)(const char *, char **);
    *(void **)(&funcpt) = dlsym(handle, "search_substring_of_the_most_common_length");

    int condition_of_function = (*funcpt)(filename, &result_parallel);

    dlclose(handle);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        exit(EXIT_FAILURE);
    }

    char *result_serial = NULL;

    condition_of_function = search_substring_of_the_most_common_length(filename, &result_serial);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        free(result_serial);
        exit(EXIT_FAILURE);
    }

    EXPECT_STREQ(result_serial, result_parallel);

    free(result_serial);
    free(result_parallel);
}

TEST(multi_process, compare_algs_file_size_15000) {
    void *handle = dlopen("../libmost_frequent_seqence_parallel.so", RTLD_LAZY);
    if (!handle) {
        exit(EXIT_FAILURE);
    }

    const char *filename = "size_15000";
    char *result_parallel = NULL;

    int (*funcpt)(const char *, char **);
    *(void **)(&funcpt) = dlsym(handle, "search_substring_of_the_most_common_length");

    int condition_of_function = (*funcpt)(filename, &result_parallel);

    dlclose(handle);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        exit(EXIT_FAILURE);
    }

    char *result_serial = NULL;

    condition_of_function = search_substring_of_the_most_common_length(filename, &result_serial);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        free(result_serial);
        exit(EXIT_FAILURE);
    }

    EXPECT_STREQ(result_serial, result_parallel);

    free(result_serial);
    free(result_parallel);
}

TEST(multi_process, compare_algs_file_size_50000) {
    void *handle = dlopen("../libmost_frequent_seqence_parallel.so", RTLD_LAZY);
    if (!handle) {
        exit(EXIT_FAILURE);
    }

    const char *filename = "size_50000";
    char *result_parallel = NULL;

    int (*funcpt)(const char *, char **);
    *(void **)(&funcpt) = dlsym(handle, "search_substring_of_the_most_common_length");

    int condition_of_function = (*funcpt)(filename, &result_parallel);

    dlclose(handle);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        exit(EXIT_FAILURE);
    }

    char *result_serial = NULL;

    condition_of_function = search_substring_of_the_most_common_length(filename, &result_serial);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        free(result_serial);
        exit(EXIT_FAILURE);
    }

    EXPECT_STREQ(result_serial, result_parallel);

    free(result_serial);
    free(result_parallel);
}

TEST(multi_process, compare_algs_file_size_150000) {
    void *handle = dlopen("../libmost_frequent_seqence_parallel.so", RTLD_LAZY);
    if (!handle) {
        exit(EXIT_FAILURE);
    }

    const char *filename = "size_150000";
    char *result_parallel = NULL;

    int (*funcpt)(const char *, char **);
    *(void **)(&funcpt) = dlsym(handle, "search_substring_of_the_most_common_length");

    int condition_of_function = (*funcpt)(filename, &result_parallel);

    dlclose(handle);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        exit(EXIT_FAILURE);
    }

    char *result_serial = NULL;

    condition_of_function = search_substring_of_the_most_common_length(filename, &result_serial);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        free(result_serial);
        exit(EXIT_FAILURE);
    }

    EXPECT_STREQ(result_serial, result_parallel);

    free(result_serial);
    free(result_parallel);
}
