// Copyright 2021 Mikhail Popov

#include <cstdio>
#include <fstream>
#include <iostream>
#include <time.h>
#include <sys/time.h>

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
        EXIT_FAILURE;
    }
    
    char *result_serial = NULL;

    condition_of_function = search_substring_of_the_most_common_length_serial(filename, &result_serial);

    if (condition_of_function != CORRECT) {
        free(result_parallel);
        free(result_serial);
        EXIT_FAILURE;
    }

    free(result_serial);
    free(result_parallel);
}

// TEST(multi_process, compare_algs_file_size_150) {

//     const char *filename = "size_150";

//     char *result_serial = NULL;
//     char *result_parallel = NULL;

//     search_substring_of_the_most_common_length_serial(filename, &result_serial);

//     int processes_number =  search_substring_of_the_most_common_length_parallel(filename, &result_parallel);
//     if (processes_number == PARENT_PID) {
//         ASSERT_STREQ(result_serial, result_parallel);
//         free(result_parallel);
//     }

//     free(result_serial);
// }

// TEST(multi_process, compare_algs_file_size_2500) {
//     const char *filename = "size_2500";
//     char *result_parallel = NULL;
//     struct timeval start;
//     struct timeval end;

//     gettimeofday(&start, NULL);
//     int condition_of_function = search_substring_of_the_most_common_length_parallel(filename, &result_parallel);
//     gettimeofday(&end, NULL);

//     if (condition_of_function != CORRECT) {
//         free(result_parallel);
//         EXIT_FAILURE;
//     }
    
//     long time_parallel = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;

//     char *result_serial = NULL;

//     gettimeofday(&start, NULL);
//     condition_of_function = search_substring_of_the_most_common_length_serial(filename, &result_serial);
//     gettimeofday(&end, NULL);

//     if (condition_of_function != CORRECT) {
//         free(result_parallel);
//         free(result_serial);
//         EXIT_FAILURE;
//     }

//     long time_serial = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;

//     ASSERT_EQ(TRUE, !!(time_serial < time_parallel));

//     free(result_serial);
//     free(result_parallel);
// }