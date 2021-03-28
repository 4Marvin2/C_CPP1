// Copyright 2021 Mikhail Popov

#include <cstdio>
#include <fstream>
#include <iostream>

#include "gtest/gtest.h"

extern "C" {
#include "serial/utils.h"
#include "../src/serial/utils.c"
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

// тесты input
class TestInput : public ::testing::Test {
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
        remove(file_name);
        if (fp != NULL) {
            fclose(fp);
        }
    }
    char *arr, *correct_arr;
    FILE *fp = NULL;
    char file_name[11] = "./test.txt";
};

TEST_F(TestInput, correct_reading) {
    std::ofstream out(file_name);
    out << "qwert";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(CORRECT, input(&arr, fp));
    ASSERT_STREQ("qwert", arr);
}

TEST_F(TestInput, null_arr) {
    fp = fopen(file_name, "r");

    ASSERT_EQ(NULL_PTR, input(NULL, fp));
}

TEST_F(TestInput, null_file_pointer) {
    ASSERT_EQ(NULL_STREAM, input(&arr, NULL));
}

// тесты search_number_of_repeating_length
class TestSearchNumberOfRepeatingLength : public ::testing::Test {
 protected:
    void SetUp() {
        arr = reinterpret_cast<char *>(malloc(sizeof(char) * MIN_ARR_SIZE));
        correct_arr = reinterpret_cast<int *>(malloc(sizeof(int) * 13));

        arr[0] = '\0';
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
        free(arr);
        free(correct_arr);
        free(number_of_repeating_length);
        remove(file_name);
        if (fp != NULL) {
            fclose(fp);
        }
    }
    char *arr;
    int *correct_arr, *number_of_repeating_length = nullptr;
    FILE *fp = NULL;
    char file_name[11] = "./test.txt";
};

TEST_F(TestSearchNumberOfRepeatingLength, correct_searching) {
    std::ofstream out(file_name);
    out << "qwwhhhggkkkk";
    out.close();
    fp = fopen(file_name, "r");
    input(&arr, fp);
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

// тесты search_substring_of_the_most_common_length_serial
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

    ASSERT_EQ(CORRECT, search_substring_of_the_most_common_length_serial(file_name, &result));
    ASSERT_STREQ(correct_arr, result);
}

TEST_F(TestSearchSubstringOfTheMostCommonLengthSerial, correct_searching_with_equal_element_string) {
    correct_arr = reinterpret_cast<char *>(malloc(sizeof(char) * 6));
    strncpy(correct_arr, "jjjjj", 6);
    result = reinterpret_cast<char *>(malloc(sizeof(char) * 6));
    std::ofstream out(file_name);
    out << "jjjjj";
    out.close();

    ASSERT_EQ(CORRECT, search_substring_of_the_most_common_length_serial(file_name, &result));
    ASSERT_STREQ(correct_arr, result);
}

TEST_F(TestSearchSubstringOfTheMostCommonLengthSerial, correct_searching_with_edifferent_elements) {
    correct_arr = reinterpret_cast<char *>(malloc(sizeof(char) * 5));
    strncpy(correct_arr, "llll", 5);
    result = reinterpret_cast<char *>(malloc(sizeof(char) * 5));
    std::ofstream out(file_name);
    out << "qwertjjkkkllllttooooqwertvvvv";
    out.close();

    ASSERT_EQ(CORRECT, search_substring_of_the_most_common_length_serial(file_name, &result));
    ASSERT_STREQ(correct_arr, result);
}

TEST_F(TestSearchSubstringOfTheMostCommonLengthSerial, open_file_failed) {
    char no_valid_file_name[11] = "./aaaa.txt";

    ASSERT_EQ(OPEN_FILE_FAILED, search_substring_of_the_most_common_length_serial(no_valid_file_name, &result));
}
