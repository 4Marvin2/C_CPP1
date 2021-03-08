// Copyright 2021 Mikhail Popov

#include "gtest/gtest.h"

#define MAX_FIELD_LENGTH 101
#define NUMBER_OF_FIELDS 4
// + 4 - знаки разделители и конец строки/перевод строки
#define MAX_STR_LENGTH MAX_FIELD_LENGTH * NUMBER_OF_FIELDS + 4

extern "C" {
#include "utils/utils.h"
}

class TestPars : public ::testing::Test {
 protected:
    void SetUp() {
        ptr = new char*[NUMBER_OF_FIELDS];
        for (int i = 0; i < NUMBER_OF_FIELDS; i++) {
            ptr[i] = new char[MAX_FIELD_LENGTH];
        }
    }
    void TearDown() {
        for (int i = 0; i < NUMBER_OF_FIELDS; i++) {
            delete []ptr[i];
        }
        delete []ptr;
    }
    char **ptr;
};

TEST_F(TestPars, correct_pars) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;Atlantyda;1:30";

    ASSERT_EQ(pars_str(str, ';', ptr), 4);
}

TEST_F(TestPars, invalid_string1) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;Atlantyda;\n";

    ASSERT_EQ(pars_str(str, ';', ptr), 3);
}

TEST_F(TestPars, invalid_string2) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;Atlantyda\n";

    ASSERT_EQ(pars_str(str, ';', ptr), 3);
}

TEST_F(TestPars, invalid_string3) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;Atlantyda;";

    ASSERT_EQ(pars_str(str, ';', ptr), 3);
}

TEST_F(TestPars, invalid_string4) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;Atlantyda";

    ASSERT_EQ(pars_str(str, ';', ptr), 3);
}

TEST_F(TestPars, invalid_string5) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;;1:30";

    ASSERT_EQ(pars_str(str, ';', ptr), 3);
}

TEST_F(TestPars, invalid_string6) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;1:30";

    ASSERT_EQ(pars_str(str, ';', ptr), 3);
}

TEST_F(TestPars, invalid_string7) {
    char str[MAX_STR_LENGTH] = "";

    ASSERT_EQ(pars_str(str, ';', ptr), 0);
}

// проверить что правильно распарсил
