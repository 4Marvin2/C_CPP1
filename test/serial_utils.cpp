// Copyright 2021 Mikhail Popov

#include <cstdio>
#include <fstream>
#include <iostream>

#include "gtest/gtest.h"

extern "C" {
#include "utils.h"
#include "../src/serial/utils.c"
#include "general/utils.h"
}

// тесты input
class TestInput : public ::testing::Test {
 protected:
    void TearDown() {
        if (arr != nullptr) {
            munmap(arr, 9);
        }
        remove(file_name);
        close(fd);
    }
    char *arr = nullptr;
    const char *file_name = "./test.txt";
    int fd = 0;
};

TEST_F(TestInput, correct_createing_shared_file) {
    std::ofstream out(file_name);
    out << "qwerthhh";
    out.close();

    ASSERT_EQ(CORRECT, input(file_name, &arr, fd));
}

TEST_F(TestInput, null_arr) {
    ASSERT_EQ(NULL_PTR, input(file_name, NULL, fd));
}

TEST_F(TestInput, null_file_name) {
    ASSERT_EQ(NULL_PTR, input(NULL, &arr, fd));
}

TEST_F(TestInput, no_valid_file_name) {
    const char *no_valid_file_name = "./aaaa.txt";
    ASSERT_EQ(OPEN_FILE_FAILED, input(no_valid_file_name, &arr, fd));
}

// тесты search_number_of_repeating_length
class TestSearchNumberOfRepeatingLength : public ::testing::Test {
 protected:
    void SetUp() {
        correct_arr = reinterpret_cast<int *>(malloc(sizeof(int) * 13));

        correct_arr[0] = 0;
        correct_arr[1] = 1;
        correct_arr[2] = 2;
        correct_arr[3] = 1;
        correct_arr[4] = 1;
        correct_arr[5] = 0;
        correct_arr[6] = 0;
        correct_arr[7] = 0;
        correct_arr[8] = 0;
        correct_arr[9] = 0;
        correct_arr[10] = 0;
        correct_arr[11] = 0;
        correct_arr[12] = 0;
    }
    void TearDown() {
        // free(arr);
        free(correct_arr);
        free(number_of_repeating_length);
        remove(file_name);
        close(fd);
    }
    char *arr = nullptr;
    int *correct_arr, *number_of_repeating_length = nullptr;
    int fd = 0;
    char file_name[11] = "./test.txt";
};

TEST_F(TestSearchNumberOfRepeatingLength, correct_searching) {
    std::ofstream out(file_name);
    out << "qwwhhhggkkkk";
    out.close();
    input(file_name, &arr, fd);
    size_t size = strlen(arr) + 1;
    number_of_repeating_length = create_arr_counter(size);

    ASSERT_EQ(CORRECT, search_number_of_repeating_length(arr, size, number_of_repeating_length));
    for (int i = 0; i < size; i++) {
        ASSERT_EQ(correct_arr[i], number_of_repeating_length[i]);
    }
}

TEST_F(TestSearchNumberOfRepeatingLength, null_str) {
    int size = MIN_ARR_SIZE;
    ASSERT_EQ(NULL_PTR, search_number_of_repeating_length(NULL, size, number_of_repeating_length));
}

TEST_F(TestSearchNumberOfRepeatingLength, null_arr) {
    int size = MIN_ARR_SIZE;
    ASSERT_EQ(NULL_PTR, search_number_of_repeating_length(arr, size, NULL));
}

// тесты search_most_frequent_value
class TestSearchMostFrequentValue : public ::testing::Test {
 protected:
    void SetUp() {
        arr = reinterpret_cast<int *>(malloc(sizeof(int) * 6));
        arr[0] = 0;
        arr[1] = 1;
        arr[2] = 2;
        arr[3] = 1;
        arr[4] = 1;
        arr[5] = 0;
    }
    void TearDown() {
        free(arr);
    }
    int *arr = nullptr;
};

TEST_F(TestSearchMostFrequentValue, correct_searching) {
    ASSERT_EQ(2, search_most_frequent_value(arr, 6));
}

TEST_F(TestSearchMostFrequentValue, null_arr) {
    ASSERT_EQ(NULL_PTR, search_most_frequent_value(NULL, 6));
}

// тесты search_substring_of_the_most_common_length
class TestSearchSubstringOfTheMostCommonLengthSerial : public ::testing::Test {
 protected:
    void TearDown() {
        free(correct_arr);
        free(result);
        remove(file_name);
    }
    char *correct_arr = nullptr, *result = nullptr;
    char file_name[11] = "./test.txt";
};

TEST_F(TestSearchSubstringOfTheMostCommonLengthSerial, correct_searching_with_two_equal_length_of_substring) {
    correct_arr = reinterpret_cast<char *>(malloc(sizeof(char) * 3));
    strncpy(correct_arr, "ww", 3);
    result = reinterpret_cast<char *>(malloc(sizeof(char) * 3));
    std::ofstream out(file_name);
    out << "qwwhhhggkkkkllll";
    out.close();

    ASSERT_EQ(CORRECT, search_substring_of_the_most_common_length(file_name, &result));
    ASSERT_STREQ(correct_arr, result);
}

TEST_F(TestSearchSubstringOfTheMostCommonLengthSerial, correct_searching_with_equal_element_string) {
    correct_arr = reinterpret_cast<char *>(malloc(sizeof(char) * 6));
    strncpy(correct_arr, "jjjjj", 6);
    result = reinterpret_cast<char *>(malloc(sizeof(char) * 6));
    std::ofstream out(file_name);
    out << "jjjjj";
    out.close();

    ASSERT_EQ(CORRECT, search_substring_of_the_most_common_length(file_name, &result));
    ASSERT_STREQ(correct_arr, result);
}

TEST_F(TestSearchSubstringOfTheMostCommonLengthSerial, correct_searching_with_edifferent_elements) {
    correct_arr = reinterpret_cast<char *>(malloc(sizeof(char) * 5));
    strncpy(correct_arr, "llll", 5);
    result = reinterpret_cast<char *>(malloc(sizeof(char) * 5));
    std::ofstream out(file_name);
    out << "qwertjjkkkllllttooooqwertvvvv";
    out.close();

    ASSERT_EQ(CORRECT, search_substring_of_the_most_common_length(file_name, &result));
    ASSERT_STREQ(correct_arr, result);
}

TEST_F(TestSearchSubstringOfTheMostCommonLengthSerial, open_file_failed) {
    char no_valid_file_name[11] = "./aaaa.txt";

    ASSERT_EQ(OPEN_FILE_FAILED, search_substring_of_the_most_common_length(no_valid_file_name, &result));
}
