// Copyright 2021 Mikhail Popov

#include <cstdio>
#include <fstream>
#include <iostream>

#include "gtest/gtest.h"

extern "C" {
#include "parallel/utils.h"
#include "serial/utils.h"
}

#define TRUE 1

TEST(multi_process, compare_algs_file_size_50) {
    const char *filename = "size_50";
    char *result_parallel = NULL;

    int condition_of_function = search_substring_of_the_most_common_length_parallel(filename, &result_parallel);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        exit(EXIT_FAILURE);
    }

    char *result_serial = NULL;

    condition_of_function = search_substring_of_the_most_common_length_serial(filename, &result_serial);

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
    const char *filename = "size_150";
    char *result_parallel = NULL;

    int condition_of_function = search_substring_of_the_most_common_length_parallel(filename, &result_parallel);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        exit(EXIT_FAILURE);
    }

    char *result_serial = NULL;

    condition_of_function = search_substring_of_the_most_common_length_serial(filename, &result_serial);

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
    const char *filename = "size_2500";
    char *result_parallel = NULL;

    int condition_of_function = search_substring_of_the_most_common_length_parallel(filename, &result_parallel);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        EXIT_FAILURE;
    }

    char *result_serial = NULL;

    condition_of_function = search_substring_of_the_most_common_length_serial(filename, &result_serial);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        free(result_serial);
        EXIT_FAILURE;
    }

    EXPECT_STREQ(result_serial, result_parallel);

    free(result_serial);
    free(result_parallel);
}

TEST(multi_process, compare_algs_file_size_7500) {
    const char *filename = "size_7500";
    char *result_parallel = NULL;

    int condition_of_function = search_substring_of_the_most_common_length_parallel(filename, &result_parallel);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        EXIT_FAILURE;
    }

    char *result_serial = NULL;

    condition_of_function = search_substring_of_the_most_common_length_serial(filename, &result_serial);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        free(result_serial);
        EXIT_FAILURE;
    }

    EXPECT_STREQ(result_serial, result_parallel);

    free(result_serial);
    free(result_parallel);
}

TEST(multi_process, compare_algs_file_size_15000) {
    const char *filename = "size_15000";
    char *result_parallel = NULL;

    int condition_of_function = search_substring_of_the_most_common_length_parallel(filename, &result_parallel);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        EXIT_FAILURE;
    }

    char *result_serial = NULL;

    condition_of_function = search_substring_of_the_most_common_length_serial(filename, &result_serial);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        free(result_serial);
        EXIT_FAILURE;
    }

    EXPECT_STREQ(result_serial, result_parallel);

    free(result_serial);
    free(result_parallel);
}

TEST(multi_process, compare_algs_file_size_50000) {
    const char *filename = "size_50000";
    char *result_parallel = NULL;

    int condition_of_function = search_substring_of_the_most_common_length_parallel(filename, &result_parallel);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        EXIT_FAILURE;
    }

    char *result_serial = NULL;

    condition_of_function = search_substring_of_the_most_common_length_serial(filename, &result_serial);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        free(result_serial);
        EXIT_FAILURE;
    }

    EXPECT_STREQ(result_serial, result_parallel);

    free(result_serial);
    free(result_parallel);
}

TEST(multi_process, compare_algs_file_size_150000) {
    const char *filename = "size_150000";
    char *result_parallel = NULL;

    int condition_of_function = search_substring_of_the_most_common_length_parallel(filename, &result_parallel);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        EXIT_FAILURE;
    }

    char *result_serial = NULL;

    condition_of_function = search_substring_of_the_most_common_length_serial(filename, &result_serial);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        free(result_serial);
        EXIT_FAILURE;
    }

    EXPECT_STREQ(result_serial, result_parallel);

    free(result_serial);
    free(result_parallel);
}
