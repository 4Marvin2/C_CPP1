// Copyright 2021 Mikhail Popov

#include <cstdio>
#include <fstream>
#include <iostream>
#include <sys/mman.h>

#include "gtest/gtest.h"

extern "C" {
#include "utils.h"
#include "../src/parallel/utils.c"
#include "general/general_functions.h"
}

// тесты create_shared_file
class TestCreateSharedFile : public ::testing::Test {
 protected:
    void TearDown() {
        if (arr != nullptr) {
            munmap(arr, 9);
        }
        remove(file_name);
    }
    char *arr = nullptr;
    char file_name[11] = "./test.txt";
    int fd = 0;
};

TEST_F(TestCreateSharedFile, correct_createing_shared_file) {
    std::ofstream out(file_name);
    out << "qwerthhh";
    out.close();

    ASSERT_EQ(9, create_shared_file(file_name, &arr, fd));
}

TEST_F(TestCreateSharedFile, null_arr) {
    ASSERT_EQ(NULL_PTR, create_shared_file(file_name, NULL, fd));
}

TEST_F(TestCreateSharedFile, null_file_name) {
    ASSERT_EQ(NULL_PTR, create_shared_file(NULL, &arr, fd));
}

TEST_F(TestCreateSharedFile, no_valid_file_name) {
    char no_valid_file_name[11] = "./aaaa.txt";
    ASSERT_EQ(OPEN_FILE_FAILED, create_shared_file(no_valid_file_name, &arr, fd));
}

// тесты create_shared_memory
class TestCreateSharedMemory : public ::testing::Test {
 protected:
    void TearDown() {
        if (arr != nullptr) {
            munmap(arr, 9);
        }
    }
    int *arr = nullptr;
};

TEST_F(TestCreateSharedMemory, correct_creating_shared_memory) {
    ASSERT_EQ(CORRECT, create_shared_memory(&arr, 2));
}

TEST_F(TestCreateSharedMemory, null_arr) {
    ASSERT_EQ(NULL_PTR, create_shared_memory(NULL, 1));
}

// тесты finding_partition_of_arr_char
class TestFindingPartitionOfArrChar : public ::testing::Test {
 protected:
    void SetUp() {
        arr = reinterpret_cast<char *>(malloc(sizeof(char) * 11));
        strncpy(arr, "qwerjjjffq", 11);
        arr_splitting = reinterpret_cast<int *>(malloc(sizeof(int) * number_of_processes));
        correct_arr_splitting = reinterpret_cast<int *>(malloc(sizeof(int) * number_of_processes));
        correct_arr_splitting[0] = 3;
        correct_arr_splitting[1] = 7;
        correct_arr_splitting[2] = 11;
    }
    void TearDown() {
        free(arr);
        free(arr_splitting);
        free(correct_arr_splitting);
    }
    char *arr = nullptr;
    int *arr_splitting = nullptr, *correct_arr_splitting = nullptr;
    int number_of_processes = 3;
};

TEST_F(TestFindingPartitionOfArrChar, correct_finding) {
    ASSERT_EQ(CORRECT, finding_partition_of_arr_char(arr_splitting, number_of_processes, arr, 11));
    for (int i = 0; i < number_of_processes; i++) {
        ASSERT_EQ(correct_arr_splitting[i], arr_splitting[i]);
    }
}

TEST_F(TestFindingPartitionOfArrChar, null_arr) {
    ASSERT_EQ(NULL_PTR, finding_partition_of_arr_char(arr_splitting, number_of_processes, NULL, 11));
}

TEST_F(TestFindingPartitionOfArrChar, null_arr_splitting) {
    ASSERT_EQ(NULL_PTR, finding_partition_of_arr_char(NULL, number_of_processes, arr, 11));
}

// тесты finding_partition_of_arr_int
class TestFindingPartitionOfArrInt : public ::testing::Test {
 protected:
    void SetUp() {
        arr = reinterpret_cast<int *>(malloc(sizeof(int) * 11));
        for (int i = 0 ; i < 11; i++) {
            arr[i] = i;
        }
        arr_splitting = reinterpret_cast<int *>(malloc(sizeof(int) * number_of_processes));
        correct_arr_splitting = reinterpret_cast<int *>(malloc(sizeof(int) * number_of_processes));
        correct_arr_splitting[0] = 3;
        correct_arr_splitting[1] = 7;
        correct_arr_splitting[2] = 11;
    }
    void TearDown() {
        free(arr);
        free(arr_splitting);
        free(correct_arr_splitting);
    }
    int *arr = nullptr, *arr_splitting = nullptr, *correct_arr_splitting = nullptr;
    int number_of_processes = 3;
};

TEST_F(TestFindingPartitionOfArrInt, correct_finding) {
    ASSERT_EQ(CORRECT, finding_partition_of_arr_int(arr_splitting, number_of_processes, arr, 11));
    for (int i = 0; i < number_of_processes; i++) {
        ASSERT_EQ(correct_arr_splitting[i], arr_splitting[i]);
    }
}

TEST_F(TestFindingPartitionOfArrInt, null_arr) {
    ASSERT_EQ(NULL_PTR, finding_partition_of_arr_int(arr_splitting, number_of_processes, NULL, 11));
}

TEST_F(TestFindingPartitionOfArrInt, null_arr_splitting) {
    ASSERT_EQ(NULL_PTR, finding_partition_of_arr_int(NULL, number_of_processes, arr, 11));
}

// тесты search_number_of_repeating_length_parallel
class TestSearchNumberOfRepeatingLengthParallel: public ::testing::Test {
 protected:
    void SetUp() {
        std::ofstream out(file_name);
        out << "qwerthhhggyyy";
        out.close();
        pids = reinterpret_cast<int *>(malloc(sizeof(int) * number_of_processes));
        size = create_shared_file(file_name, &arr, fd);
        create_shared_memory(&arr_splitting, number_of_processes);
        finding_partition_of_arr_char(arr_splitting, number_of_processes, arr, size);
        create_shared_memory(&arr_counter, size);

        correct_arr_counter = reinterpret_cast<int *>(malloc(sizeof(int) * size));
        correct_arr_counter[0] = 0;
        correct_arr_counter[1] = 5;
        correct_arr_counter[2] = 1;
        correct_arr_counter[3] = 2;
        correct_arr_counter[4] = 0;
        correct_arr_counter[5] = 0;
        correct_arr_counter[6] = 0;
        correct_arr_counter[7] = 0;
        correct_arr_counter[8] = 0;
        correct_arr_counter[9] = 0;
        correct_arr_counter[10] = 0;
        correct_arr_counter[11] = 0;
        correct_arr_counter[12] = 0;
        correct_arr_counter[13] = 0;
    }
    void TearDown() {
        free(pids);
        free(correct_arr_counter);
        if (pid == PARENT_PID) {
            free_main_resources(fd, arr, size, arr_counter, arr_splitting, number_of_processes);
            remove(file_name);
        }
    }
    int *pids = nullptr, *arr_splitting = nullptr, *arr_counter = nullptr, *correct_arr_counter = nullptr;
    char *arr = nullptr;
    const int number_of_processes = 3;
    int fd = 0;
    int size;
    int pid;
    char file_name[11] = "./test.txt";
};

TEST_F(TestSearchNumberOfRepeatingLengthParallel, correct_searching) {
    pid = create_forks(number_of_processes, pids);
    if (pid != PARENT_PID) {
        ASSERT_EQ(CORRECT, search_number_of_repeating_length_parallel(arr,
                                                                      arr_splitting,
                                                                      arr_counter,
                                                                      pid));
        close(fd);
        free(correct_arr_counter);
        free(pids);
        exit(EXIT_SUCCESS);
    }

    for (int i = 0; i < number_of_processes; i++) {
        while (int wait_status = waitpid(pids[i], NULL, 0) > 0) {}
    }

    for (int i = 0; i < size; i++) {
        ASSERT_EQ(correct_arr_counter[i], arr_counter[i]);
    }
}

TEST_F(TestSearchNumberOfRepeatingLengthParallel, null_char_arr) {
    pid = create_forks(number_of_processes, pids);
    if (pid != PARENT_PID) {
        ASSERT_EQ(NULL_PTR, search_number_of_repeating_length_parallel(NULL,
                                                                       arr_splitting,
                                                                       arr_counter,
                                                                       pid));
        close(fd);
        free(correct_arr_counter);
        free(pids);
        exit(EXIT_SUCCESS);
    }

    for (int i = 0; i < number_of_processes; i++) {
        while (waitpid(pids[i], NULL, 0) > 0) {}
    }
}

TEST_F(TestSearchNumberOfRepeatingLengthParallel, null_arr_splitting) {
    pid = create_forks(number_of_processes, pids);
    if (pid != PARENT_PID) {
        ASSERT_EQ(NULL_PTR, search_number_of_repeating_length_parallel(arr,
                                                                       NULL,
                                                                       arr_counter,
                                                                       pid));
        close(fd);
        free(correct_arr_counter);
        free(pids);
        exit(EXIT_SUCCESS);
    }

    for (int i = 0; i < number_of_processes; i++) {
        while (waitpid(pids[i], NULL, 0) > 0) {}
    }
}

TEST_F(TestSearchNumberOfRepeatingLengthParallel, null_arr_counter) {
    pid = create_forks(number_of_processes, pids);
    if (pid != PARENT_PID) {
        ASSERT_EQ(NULL_PTR, search_number_of_repeating_length_parallel(arr,
                                                                       arr_splitting,
                                                                       NULL,
                                                                       pid));
        close(fd);
        free(correct_arr_counter);
        free(pids);
        exit(EXIT_SUCCESS);
    }

    for (int i = 0; i < number_of_processes; i++) {
        while (waitpid(pids[i], NULL, 0) > 0) {}
    }
}

// тесты search_max_parallel
class TestSearchMaxParallel: public ::testing::Test {
 protected:
    void SetUp() {
        pids = reinterpret_cast<int *>(malloc(sizeof(int) * number_of_processes));
        create_shared_memory(&arr_splitting_for_max, number_of_processes);
        create_shared_memory(&arr_max.arr, number_of_processes);
        create_shared_memory(&arr_max.max_length, number_of_processes);
        create_shared_memory(&arr_counter, size);
        arr_counter[0] = 0;
        arr_counter[1] = 5;
        arr_counter[2] = 1;
        arr_counter[3] = 2;
        arr_counter[4] = 0;
        arr_counter[5] = 6;
        arr_counter[6] = 7;
        arr_counter[7] = 1;
        arr_counter[8] = 0;
        arr_counter[9] = 0;
        arr_counter[10] = 5;
        arr_counter[11] = 3;
        arr_counter[12] = 8;
        arr_counter[13] = 0;
        finding_partition_of_arr_int(arr_splitting_for_max, number_of_processes, arr_counter, size);
        correct_arr_max = reinterpret_cast<int *>(malloc(sizeof(int) * number_of_processes));
        correct_arr_max[0] = 2;
        correct_arr_max[1] = 7;
        correct_arr_max[2] = 8;
    }
    void TearDown() {
        free(pids);
        free(correct_arr_max);
        if (pid == PARENT_PID) {
            munmap(arr_max.arr, number_of_processes);
            munmap(arr_max.max_length, number_of_processes);
            munmap(arr_splitting_for_max, number_of_processes);
            munmap(arr_counter, size);
        }
    }
    int *pids = nullptr;
    max_length_by_section arr_max;
    int *arr_splitting_for_max = nullptr;
    int *arr_counter = nullptr;
    int *correct_arr_max = nullptr;
    const int number_of_processes = 3;
    const int size = 13;
    int pid;
};

TEST_F(TestSearchMaxParallel, correct_searching) {
    pid = create_forks(number_of_processes, pids);
    if (pid != PARENT_PID) {
        ASSERT_EQ(CORRECT, search_max_parallel(&arr_max, arr_splitting_for_max, arr_counter, pid));
        free(correct_arr_max);
        free(pids);
        exit(EXIT_SUCCESS);
    }

    for (int i = 0; i < number_of_processes; i++) {
        while (waitpid(pids[i], NULL, 0) > 0) {}
    }

    for (int i = 0; i < number_of_processes; i++) {
        ASSERT_EQ(correct_arr_max[i], arr_max.arr[i]);
    }
}

TEST_F(TestSearchMaxParallel, null_arr_max) {
    pid = create_forks(number_of_processes, pids);
    if (pid != PARENT_PID) {
        ASSERT_EQ(NULL_PTR, search_max_parallel(NULL, arr_splitting_for_max, arr_counter, pid));
        free(correct_arr_max);
        free(pids);
        exit(EXIT_SUCCESS);
    }

    for (int i = 0; i < number_of_processes; i++) {
        while (waitpid(pids[i], NULL, 0) > 0) {}
    }
}

TEST_F(TestSearchMaxParallel, null_arr_splitting_for_max) {
    pid = create_forks(number_of_processes, pids);
    if (pid != PARENT_PID) {
        ASSERT_EQ(NULL_PTR, search_max_parallel(&arr_max, NULL, arr_counter, pid));
        free(correct_arr_max);
        free(pids);
        exit(EXIT_SUCCESS);
    }

    for (int i = 0; i < number_of_processes; i++) {
        while (waitpid(pids[i], NULL, 0) > 0) {}
    }
}

TEST_F(TestSearchMaxParallel, null_arr_counter) {
    pid = create_forks(number_of_processes, pids);
    if (pid != PARENT_PID) {
        ASSERT_EQ(NULL_PTR, search_max_parallel(&arr_max, arr_splitting_for_max, NULL, pid));
        free(correct_arr_max);
        free(pids);
        exit(EXIT_SUCCESS);
    }

    for (int i = 0; i < number_of_processes; i++) {
        while (waitpid(pids[i], NULL, 0) > 0) {}
    }
}

// тесты search_arr_of_repeating_length
class TestSearchSubstringOfTheMostCommonLengthParallel : public ::testing::Test {
 protected:
    void TearDown() {
        free(correct_arr);
        free(result);
        remove(file_name);
    }
    char *correct_arr = nullptr, *result = nullptr;
    char file_name[11] = "./test.txt";
};
TEST_F(TestSearchSubstringOfTheMostCommonLengthParallel, correct_searching_with_two_equal_length_of_substring) {
    correct_arr = reinterpret_cast<char *>(malloc(sizeof(char) * 3));
    strncpy(correct_arr, "ww", 3);
    result = reinterpret_cast<char *>(malloc(sizeof(char) * 3));
    std::ofstream out(file_name);
    out << "qwwhhhggkkkkllll";
    out.close();

    ASSERT_EQ(CORRECT, search_substring_of_the_most_common_length(file_name, &result));
    ASSERT_STREQ(correct_arr, result);
}

TEST_F(TestSearchSubstringOfTheMostCommonLengthParallel, correct_searching_with_equal_element_string) {
    correct_arr = reinterpret_cast<char *>(malloc(sizeof(char) * 6));
    strncpy(correct_arr, "jjjjj", 6);
    result = reinterpret_cast<char *>(malloc(sizeof(char) * 6));
    std::ofstream out(file_name);
    out << "jjjjj";
    out.close();

    ASSERT_EQ(CORRECT, search_substring_of_the_most_common_length(file_name, &result));
    ASSERT_STREQ(correct_arr, result);
}

TEST_F(TestSearchSubstringOfTheMostCommonLengthParallel, correct_searching_with_edifferent_elements) {
    correct_arr = reinterpret_cast<char *>(malloc(sizeof(char) * 5));
    strncpy(correct_arr, "llll", 5);
    result = reinterpret_cast<char *>(malloc(sizeof(char) * 5));
    std::ofstream out(file_name);
    out << "qwertjjkkkllllttooooqwertvvvv";
    out.close();

    ASSERT_EQ(CORRECT, search_substring_of_the_most_common_length(file_name, &result));
    ASSERT_STREQ(correct_arr, result);
}

TEST_F(TestSearchSubstringOfTheMostCommonLengthParallel, open_file_failed) {
    char no_valid_file_name[11] = "./aaaa.txt";

    ASSERT_EQ(OPEN_FILE_FAILED, search_substring_of_the_most_common_length(no_valid_file_name, &result));
}
