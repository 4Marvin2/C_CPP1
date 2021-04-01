// Copyright 2021 Mikhail Popov

#include <cstdio>
#include <fstream>
#include <iostream>
#include <sys/mman.h>

#include "gtest/gtest.h"

extern "C" {
#include "general/utils.h"
}

// тесты realloc_array
class TestReallocArr : public ::testing::Test {
 protected:
    void SetUp() {
        arr = reinterpret_cast<char *>(malloc(sizeof(char) * MIN_ARR_SIZE));
        correct_arr = reinterpret_cast<char *>(malloc(sizeof(char) * MIN_ARR_SIZE));

        arr[0] = '\0';
        correct_arr[0] = '\0';
    }
    void TearDown() {
        free(arr);
        free(correct_arr);
    }
    char *arr, *correct_arr;
};

TEST_F(TestReallocArr, correct_realloc) {
    ASSERT_EQ(2, realloc_array(&arr, MIN_ARR_SIZE * 2));
    ASSERT_EQ(correct_arr[0], arr[0]);
}

TEST_F(TestReallocArr, invalid_size) {
    ASSERT_EQ(NULL_SIZE_REALLOC, realloc_array(&arr, 0));
}

TEST_F(TestReallocArr, null_ptr) {
    ASSERT_EQ(NULL_PTR, realloc_array(NULL, MIN_ARR_SIZE));
}

// тесты search_max
class TestSearchMax: public ::testing::Test {
 protected:
    void SetUp() {
        arr = reinterpret_cast<int *>(malloc(sizeof(int) * size));
        arr[0] = 10;
        arr[1] = 5;
        arr[2] = 1;
        arr[3] = 2;
        arr[4] = 42;
        arr[5] = 6;
    }
    void TearDown() {
        free(arr);
    }
    int *arr = nullptr;
    int max;
    const int size = 6;
    const int correct_max = 4;
};

TEST_F(TestSearchMax, correct_searching) {
    ASSERT_EQ(correct_max, search_max(arr, size));
}

TEST_F(TestSearchMax, null_arr) {
    ASSERT_EQ(NULL_PTR, search_max(NULL, size));
}

// тесты search_substring
class TestSearchSubstring : public ::testing::Test {
 protected:
    void SetUp() {
        arr = reinterpret_cast<char *>(malloc(sizeof(char) * 7));
        strncpy(arr, "qwhhhu", 7);
        correct_arr = reinterpret_cast<char *>(malloc(sizeof(char) * 4));
        strncpy(correct_arr, "hhh", 4);
        result = reinterpret_cast<char *>(malloc(sizeof(char) * 4));
    }
    void TearDown() {
        free(arr);
        free(correct_arr);
        free(result);
    }
    char *arr, *correct_arr, *result = nullptr;
};

TEST_F(TestSearchSubstring , correct_searching) {
    ASSERT_EQ(CORRECT, search_substring(arr, 2, 3, result));
    ASSERT_STREQ(correct_arr, result);
}

TEST_F(TestSearchSubstring , null_arr) {
    ASSERT_EQ(NULL_PTR, search_substring(NULL, 2, 3, result));
}

TEST_F(TestSearchSubstring , null_result_arr) {
    ASSERT_EQ(NULL_PTR, search_substring(arr, 2, 3, NULL));
}

// тесты output
class TestOutput : public ::testing::Test {
 protected:
    void SetUp() {
        arr = reinterpret_cast<char *>(malloc(sizeof(char) * 7));
        strncpy(arr, "qwhhhu", 7);
        correct_arr = reinterpret_cast<char *>(malloc(sizeof(char) * 13));
        strncpy(correct_arr, "Result: hhh\n", 13);
        result = reinterpret_cast<char *>(malloc(sizeof(char) * 13));
    }
    void TearDown() {
        free(arr);
        free(correct_arr);
        free(result);
        fclose(fp);
        remove(file_name);
    }
    FILE *fp;
    char *arr, *correct_arr, *result = nullptr;
    char file_name[11] = "./test.txt";
};

TEST_F(TestOutput , correct_output) {
    fp = fopen(file_name, "w+");

    ASSERT_EQ(CORRECT, output(arr, 2, 3, fp));

    fseek(fp, 0, SEEK_SET);
    fgets(result, 13, fp);
    ASSERT_STREQ(correct_arr, result);
}

TEST_F(TestOutput , null_arr) {
    fp = fopen(file_name, "w+");

    ASSERT_EQ(NULL_PTR, output(NULL, 2, 3, fp));
}

TEST_F(TestOutput , null_stream) {
    fp = fopen(file_name, "w+");

    ASSERT_EQ(NULL_STREAM, output(arr, 2, 3, NULL));
}

// тесты search_first_occurrence_of_substring
class TestSearchFirstOccurenceOfSubstring : public ::testing::Test {
 protected:
    void SetUp() {
        arr = reinterpret_cast<char *>(malloc(sizeof(char) * 7));
        strncpy(arr, "qwhhhu", 7);
    }
    void TearDown() {
        free(arr);
    }
    char *arr = nullptr;
};

TEST_F(TestSearchFirstOccurenceOfSubstring, correct_searching) {
    ASSERT_EQ(2, search_first_occurrence_of_substring(arr, 7, 3));
}

TEST_F(TestSearchFirstOccurenceOfSubstring, null_arr) {
    ASSERT_EQ(NULL_PTR, search_first_occurrence_of_substring(NULL, 7, 3));
}

TEST_F(TestSearchFirstOccurenceOfSubstring, no_substring_this_length) {
    ASSERT_EQ(NO_SUBSTRING, search_first_occurrence_of_substring(arr, 7, 4));
}
