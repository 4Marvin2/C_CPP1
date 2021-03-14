// Copyright 2021 Mikhail Popov

#include <cstdio>
#include <fstream>

#include "gtest/gtest.h"

#define MAX_FIELD_LENGTH 101
#define NUMBER_OF_FIELDS 4
// + 4 - знаки разделители и конец строки/перевод строки
#define MAX_STR_LENGTH MAX_FIELD_LENGTH * NUMBER_OF_FIELDS + 4
#define INCREASING_THE_ARRAY 2
#define MIN_ARRAY_SIZE 2

#define ERROR_EMPTY_LINE 1
#define ERROR_WRONG_NUMBER_OF_VARIABLE_IN_LINE 2
#define FILE_OPEN_ERROR -1
#define ERROR_EMPTY_ARRAY -2
#define NULL_PTR -3
#define NULL_PTR_REALLOC -4
#define NULL_SIZE -5
#define NULL_SIZE_REALLOC -6
#define ERROR_PRINT -7

extern "C" {
#include "utils/utils.h"
#include "../src/utils.c"
}

// тесты init_song

class TestInitSong : public ::testing::Test {
 protected:
    void SetUp() {
    strncpy(correct_song.author, "VTSS", MAX_FIELD_LENGTH);
    strncpy(correct_song.singer, "VTSS", MAX_FIELD_LENGTH);
    strncpy(correct_song.name, "Atlantyda", MAX_FIELD_LENGTH);
    strncpy(correct_song.duration, "1:30", MAX_FIELD_LENGTH);
    }
    void TearDown() {
        remove(file_name);
        if (fp != NULL) {
            fclose(fp);
        }
    }
    FILE *fp = NULL;
    Song current_song, correct_song;
    char file_name[11] = "./test.txt";
};

TEST_F(TestInitSong, null_ptr1) {
    ASSERT_EQ(FILE_OPEN_ERROR, init_song(NULL, &current_song));
}

TEST_F(TestInitSong, null_ptr2) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;Atlantyda;1:30";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(NULL_PTR, init_song(fp, NULL));
}

TEST_F(TestInitSong, correct_init1) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;Atlantyda;1:30";
    out.close();
    fp = fopen(file_name, "r");

    init_song(fp, &current_song);

    ASSERT_STREQ(correct_song.author, current_song.author);
    ASSERT_STREQ(correct_song.singer, current_song.singer);
    ASSERT_STREQ(correct_song.name, current_song.name);
    ASSERT_STREQ(correct_song.duration, current_song.duration);
}

TEST_F(TestInitSong, correct_init2) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;Atlantyda;1:30\n";
    out.close();
    fp = fopen(file_name, "r");

    init_song(fp, &current_song);

    ASSERT_STREQ(correct_song.author, current_song.author);
    ASSERT_STREQ(correct_song.singer, current_song.singer);
    ASSERT_STREQ(correct_song.name, current_song.name);
    ASSERT_STREQ(correct_song.duration, current_song.duration);
}

TEST_F(TestInitSong, invalid_str1) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;Atlantyda;";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(ERROR_WRONG_NUMBER_OF_VARIABLE_IN_LINE, init_song(fp, &current_song));
}

TEST_F(TestInitSong, invalid_str2) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;Atlantyda";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(ERROR_WRONG_NUMBER_OF_VARIABLE_IN_LINE, init_song(fp, &current_song));
}

TEST_F(TestInitSong, invalid_str3) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;;1:30";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(ERROR_WRONG_NUMBER_OF_VARIABLE_IN_LINE, init_song(fp, &current_song));
}

TEST_F(TestInitSong, invalid_str4) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;1:30";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(ERROR_WRONG_NUMBER_OF_VARIABLE_IN_LINE, init_song(fp, &current_song));
}

TEST_F(TestInitSong, invalid_str5) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;Atlantyda;\n";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(ERROR_WRONG_NUMBER_OF_VARIABLE_IN_LINE, init_song(fp, &current_song));
}

TEST_F(TestInitSong, invalid_str6) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;Atlantyda\n";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(ERROR_WRONG_NUMBER_OF_VARIABLE_IN_LINE, init_song(fp, &current_song));
}

TEST_F(TestInitSong, invalid_str7) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;;1:30\n";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(ERROR_WRONG_NUMBER_OF_VARIABLE_IN_LINE, init_song(fp, &current_song));
}

TEST_F(TestInitSong, invalid_str8) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;1:30\n";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(ERROR_WRONG_NUMBER_OF_VARIABLE_IN_LINE, init_song(fp, &current_song));
}

TEST_F(TestInitSong, invalid_str9) {
    std::ofstream out(file_name);
    out << "";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(EOF, init_song(fp, &current_song));
}

TEST_F(TestInitSong, invalid_str10) {
    std::ofstream out(file_name);
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(EOF, init_song(fp, &current_song));
}

TEST_F(TestInitSong, invalid_str11) {
    std::ofstream out(file_name);
    out << "\n";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(ERROR_EMPTY_LINE, init_song(fp, &current_song));
}

// тесты pars_str

class TestPars : public ::testing::Test {
 protected:
    void SetUp() {
        ptr = new char*[NUMBER_OF_FIELDS];
        for (int i = 0; i < NUMBER_OF_FIELDS; i++) {
            ptr[i] = new char[MAX_FIELD_LENGTH];
        }

        correct_result1 = new char*[NUMBER_OF_FIELDS];
        for (int i = 0; i < NUMBER_OF_FIELDS; i++) {
            correct_result1[i] = new char[MAX_FIELD_LENGTH];
        }
        strncpy(correct_result1[0], "VTSS", MAX_FIELD_LENGTH);
        strncpy(correct_result1[1], "VTSS", MAX_FIELD_LENGTH);
        strncpy(correct_result1[2], "Atlantyda", MAX_FIELD_LENGTH);
        strncpy(correct_result1[3], "1:30", MAX_FIELD_LENGTH);

        correct_result2 = new char*[NUMBER_OF_FIELDS - 1];
        for (int i = 0; i < NUMBER_OF_FIELDS - 1; i++) {
            correct_result2[i] = new char[MAX_FIELD_LENGTH];
        }
        strncpy(correct_result2[0], "VTSS", MAX_FIELD_LENGTH);
        strncpy(correct_result2[1], "VTSS", MAX_FIELD_LENGTH);
        strncpy(correct_result2[2], "", MAX_FIELD_LENGTH);

        correct_result3 = new char*[NUMBER_OF_FIELDS - 1];
        for (int i = 0; i < NUMBER_OF_FIELDS - 1; i++) {
            correct_result3[i] = new char[MAX_FIELD_LENGTH];
        }
        strncpy(correct_result3[0], "VTSS", MAX_FIELD_LENGTH);
        strncpy(correct_result3[1], "VTSS", MAX_FIELD_LENGTH);
        strncpy(correct_result3[2], "1:30", MAX_FIELD_LENGTH);
    }
    void TearDown() {
        for (int i = 0; i < NUMBER_OF_FIELDS; i++) {
            delete []ptr[i];
        }
        delete []ptr;

        for (int i = 0; i < NUMBER_OF_FIELDS; i++) {
            delete []correct_result1[i];
        }
        delete []correct_result1;

        for (int i = 0; i < NUMBER_OF_FIELDS - 1; i++) {
            delete []correct_result2[i];
        }
        delete []correct_result2;

        for (int i = 0; i < NUMBER_OF_FIELDS - 1; i++) {
            delete []correct_result3[i];
        }
        delete []correct_result3;
    }
    char **correct_result1;
    char **correct_result2;
    char **correct_result3;
    char **ptr;
};

TEST_F(TestPars, null_ptr1) {
    ASSERT_EQ(NULL_PTR, pars_str(NULL, ';', ptr));
}

TEST_F(TestPars, null_ptr2) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;Atlantyda;1:30";

    ASSERT_EQ(NULL_PTR, pars_str(str, ';', NULL));
}

TEST_F(TestPars, null_ptr3) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;Atlantyda;1:30";
    delete []ptr[0];
    ptr[0] = NULL;

    ASSERT_EQ(NULL_PTR, pars_str(str, ';', ptr));
}

TEST_F(TestPars, correct_pars1) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;Atlantyda;1:30";

    ASSERT_EQ(4, pars_str(str, ';', ptr));
    for (int i = 0; i < NUMBER_OF_FIELDS; i++) {
        ASSERT_STREQ(correct_result1[i], ptr[i]);
    }
}

TEST_F(TestPars, correct_pars2) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;Atlantyda;1:30\n";

    ASSERT_EQ(4, pars_str(str, ';', ptr));
    for (int i = 0; i < NUMBER_OF_FIELDS; i++) {
        ASSERT_STREQ(correct_result1[i], ptr[i]);
    }
}

TEST_F(TestPars, invalid_string1) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;Atlantyda;\n";

    ASSERT_EQ(3, pars_str(str, ';', ptr));
    for (int i = 0; i < NUMBER_OF_FIELDS - 1; i++) {
        ASSERT_STREQ(correct_result1[i], ptr[i]);
    }
}

TEST_F(TestPars, invalid_string2) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;Atlantyda\n";

    ASSERT_EQ(3, pars_str(str, ';', ptr));
    for (int i = 0; i < NUMBER_OF_FIELDS - 1; i++) {
        ASSERT_STREQ(correct_result1[i], ptr[i]);
    }
}

TEST_F(TestPars, invalid_string3) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;Atlantyda;";

    ASSERT_EQ(3, pars_str(str, ';', ptr));
    for (int i = 0; i < NUMBER_OF_FIELDS - 1; i++) {
        ASSERT_STREQ(correct_result1[i], ptr[i]);
    }
}

TEST_F(TestPars, invalid_string4) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;Atlantyda";

    ASSERT_EQ(3, pars_str(str, ';', ptr));
    for (int i = 0; i < NUMBER_OF_FIELDS - 1; i++) {
        ASSERT_STREQ(correct_result1[i], ptr[i]);
    }
}

TEST_F(TestPars, invalid_string5) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;;1:30";

    ASSERT_EQ(3, pars_str(str, ';', ptr));
    for (int i = 0; i < NUMBER_OF_FIELDS - 1; i++) {
        ASSERT_STREQ(correct_result2[i], ptr[i]);
    }
}

TEST_F(TestPars, invalid_string6) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;;1:30\n";

    ASSERT_EQ(3, pars_str(str, ';', ptr));
    std::cout << ptr[2] << " ";
    for (int i = 0; i < NUMBER_OF_FIELDS - 1; i++) {
        ASSERT_STREQ(correct_result2[i], ptr[i]);
    }
}

TEST_F(TestPars, invalid_string7) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;1:30";

    ASSERT_EQ(3, pars_str(str, ';', ptr));
    for (int i = 0; i < NUMBER_OF_FIELDS - 1; i++) {
        ASSERT_STREQ(correct_result3[i], ptr[i]);
    }
}

TEST_F(TestPars, invalid_string8) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;1:30\n";

    ASSERT_EQ(3, pars_str(str, ';', ptr));
    for (int i = 0; i < NUMBER_OF_FIELDS - 1; i++) {
        ASSERT_STREQ(correct_result3[i], ptr[i]);
    }
}

TEST_F(TestPars, invalid_string9) {
    char str[MAX_STR_LENGTH] = "";

    ASSERT_EQ(0, pars_str(str, ';', ptr));
}

// тесты print_songs

class TestPrintSongs : public ::testing::Test {
 protected:
    void SetUp() {
        all_songs = reinterpret_cast<Song *>(malloc(sizeof(Song) * MIN_ARRAY_SIZE));

        strncpy(song1.author, "VTSS", MAX_FIELD_LENGTH);
        strncpy(song1.singer, "VTSS", MAX_FIELD_LENGTH);
        strncpy(song1.name, "Atlantyda", MAX_FIELD_LENGTH);
        strncpy(song1.duration, "1:30", MAX_FIELD_LENGTH);

        strncpy(song2.author, "Yaeji", MAX_FIELD_LENGTH);
        strncpy(song2.singer, "Yaeji", MAX_FIELD_LENGTH);
        strncpy(song2.name, "Raingirl", MAX_FIELD_LENGTH);
        strncpy(song2.duration, "2:00", MAX_FIELD_LENGTH);

        all_songs[0] = song1;
        all_songs[1] = song2;

        fp = fopen(file_name, "w+");
    }
    void TearDown() {
        fclose(fp);
        remove(file_name);
        free(all_songs);
    }
    FILE *fp;
    Song song1, song2;
    Song *all_songs;
    char file_name[11] = "./test.txt";
    char current_str1[112], current_str2[113];
    char correct_str1[112] = "Автор: VTSS | Исполнитель: VTSS | Название: Atlantyda | Длительность: 1:30\n";
    char correct_str2[113] = "Автор: Yaeji | Исполнитель: Yaeji | Название: Raingirl | Длительность: 2:00\n";
};

TEST_F(TestPrintSongs, correct_ouput) {
    print_songs(all_songs, MIN_ARRAY_SIZE, fp);
    fseek(fp, 0, SEEK_SET);
    fgets(current_str1, 112, fp);
    ASSERT_STREQ(correct_str1, current_str1);
    fgets(current_str2, 113, fp);
    ASSERT_STREQ(correct_str2, current_str2);
}

TEST_F(TestPrintSongs, invalid_ouput) {
    ASSERT_EQ(ERROR_PRINT, print_songs(all_songs, MIN_ARRAY_SIZE, stdin));
}

TEST_F(TestPrintSongs, null_ptr1) {
    ASSERT_EQ(NULL_PTR, print_songs(NULL, MIN_ARRAY_SIZE, stdin));
}

TEST_F(TestPrintSongs, null_ptr2) {
    ASSERT_EQ(NULL_PTR, print_songs(all_songs, MIN_ARRAY_SIZE, NULL));
}

// тесты read_data_from_file

class TestReadDataFile : public ::testing::Test {
 protected:
    void SetUp() {
        correct_all_songs = reinterpret_cast<Song *>(malloc(sizeof(Song) * MIN_ARRAY_SIZE));

        strncpy(song1.author, "VTSS", MAX_FIELD_LENGTH);
        strncpy(song1.singer, "VTSS", MAX_FIELD_LENGTH);
        strncpy(song1.name, "Atlantyda", MAX_FIELD_LENGTH);
        strncpy(song1.duration, "1:30", MAX_FIELD_LENGTH);

        strncpy(song2.author, "VTSS", MAX_FIELD_LENGTH);
        strncpy(song2.singer, "VTSS", MAX_FIELD_LENGTH);
        strncpy(song2.name, "Atlantyda", MAX_FIELD_LENGTH);
        strncpy(song2.duration, "1:30", MAX_FIELD_LENGTH);

        correct_all_songs[0] = song1;
        correct_all_songs[1] = song2;
    }
    void TearDown() {
        remove(file_name);
        if (all_songs != NULL) {
            free(all_songs);
        }
        free(correct_all_songs);
    }
    Song song1, song2;
    Song *all_songs = NULL, *correct_all_songs;
    char file_name[11] = "./test.txt";
};

TEST_F(TestReadDataFile, null_ptr1) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;Atlantyda;1:30";
    out.close();

    ASSERT_EQ(NULL_PTR, read_data_from_file(file_name, NULL));
}

TEST_F(TestReadDataFile, correct_reading1) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;Atlantyda;1:30";
    out.close();

    ASSERT_EQ(1, read_data_from_file(file_name, &all_songs));
    ASSERT_STREQ(correct_all_songs[0].author, all_songs[0].author);
    ASSERT_STREQ(correct_all_songs[0].singer, all_songs[0].singer);
    ASSERT_STREQ(correct_all_songs[0].name, all_songs[0].name);
    ASSERT_STREQ(correct_all_songs[0].duration, all_songs[0].duration);
}

TEST_F(TestReadDataFile, correct_reading2) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;Atlantyda;1:30\n";
    out.close();

    ASSERT_EQ(1, read_data_from_file(file_name, &all_songs));
    ASSERT_STREQ(correct_all_songs[0].author, all_songs[0].author);
    ASSERT_STREQ(correct_all_songs[0].singer, all_songs[0].singer);
    ASSERT_STREQ(correct_all_songs[0].name, all_songs[0].name);
    ASSERT_STREQ(correct_all_songs[0].duration, all_songs[0].duration);
}

TEST_F(TestReadDataFile, correct_reading3) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;Atlantyda;1:30\n";
    out << "VTSS;VTSS;Atlantyda;1:30";
    out.close();

    ASSERT_EQ(2, read_data_from_file(file_name, &all_songs));
    ASSERT_STREQ(correct_all_songs[0].author, all_songs[0].author);
    ASSERT_STREQ(correct_all_songs[0].singer, all_songs[0].singer);
    ASSERT_STREQ(correct_all_songs[0].name, all_songs[0].name);
    ASSERT_STREQ(correct_all_songs[0].duration, all_songs[0].duration);
    ASSERT_STREQ(correct_all_songs[1].author, all_songs[1].author);
    ASSERT_STREQ(correct_all_songs[1].singer, all_songs[1].singer);
    ASSERT_STREQ(correct_all_songs[1].name, all_songs[1].name);
    ASSERT_STREQ(correct_all_songs[1].duration, all_songs[1].duration);
}

TEST_F(TestReadDataFile, correct_reading4) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;Atlantyda;1:30\n";
    out << "\n";
    out << "VTSS;VTSS;Atlantyda;1:30\n";
    out.close();

    ASSERT_EQ(2, read_data_from_file(file_name, &all_songs));
    ASSERT_STREQ(correct_all_songs[0].author, all_songs[0].author);
    ASSERT_STREQ(correct_all_songs[0].singer, all_songs[0].singer);
    ASSERT_STREQ(correct_all_songs[0].name, all_songs[0].name);
    ASSERT_STREQ(correct_all_songs[0].duration, all_songs[0].duration);
    ASSERT_STREQ(correct_all_songs[1].author, all_songs[1].author);
    ASSERT_STREQ(correct_all_songs[1].singer, all_songs[1].singer);
    ASSERT_STREQ(correct_all_songs[1].name, all_songs[1].name);
    ASSERT_STREQ(correct_all_songs[1].duration, all_songs[1].duration);
}

TEST_F(TestReadDataFile, correct_reading5) {
    std::ofstream out(file_name);
    out << "\n";
    out << "VTSS;VTSS;Atlantyda;1:30\n";
    out << "VTSS;VTSS;Atlantyda;1:30\n";
    out.close();

    ASSERT_EQ(2, read_data_from_file(file_name, &all_songs));
    ASSERT_STREQ(correct_all_songs[0].author, all_songs[0].author);
    ASSERT_STREQ(correct_all_songs[0].singer, all_songs[0].singer);
    ASSERT_STREQ(correct_all_songs[0].name, all_songs[0].name);
    ASSERT_STREQ(correct_all_songs[0].duration, all_songs[0].duration);
    ASSERT_STREQ(correct_all_songs[1].author, all_songs[1].author);
    ASSERT_STREQ(correct_all_songs[1].singer, all_songs[1].singer);
    ASSERT_STREQ(correct_all_songs[1].name, all_songs[1].name);
    ASSERT_STREQ(correct_all_songs[1].duration, all_songs[1].duration);
}

TEST_F(TestReadDataFile, failed_no_file) {
    ASSERT_EQ(FILE_OPEN_ERROR, read_data_from_file(file_name, &all_songs));
}

TEST_F(TestReadDataFile, failed_empty_array) {
    std::ofstream out(file_name);
    out.close();

    ASSERT_EQ(ERROR_EMPTY_ARRAY, read_data_from_file(file_name, &all_songs));
}

// тесты read_str_from_file

class TestReadStr : public ::testing::Test {
 protected:
    void TearDown() {
        remove(file_name);
        if (fp != NULL) {
            fclose(fp);
        }
    }
    FILE *fp = NULL;
    char file_name[11] = "./test.txt";
};

TEST_F(TestReadStr, null_ptr1) {
    char current_str[MAX_STR_LENGTH];

    ASSERT_EQ(FILE_OPEN_ERROR, read_str_from_file(NULL, current_str));
}

TEST_F(TestReadStr, null_ptr2) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;Atlantyda;1:30";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(NULL_PTR, read_str_from_file(fp, NULL));
}

TEST_F(TestReadStr, correct_reading) {
    char current_str[MAX_STR_LENGTH];
    std::ofstream out(file_name);
    out << "VTSS;VTSS;Atlantyda;1:30";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(0, read_str_from_file(fp, current_str));
    ASSERT_STREQ("VTSS;VTSS;Atlantyda;1:30", current_str);
}

TEST_F(TestReadStr, empty_line1) {
    char current_str[MAX_STR_LENGTH];
    std::ofstream out(file_name);
    out << "\n";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(ERROR_EMPTY_LINE, read_str_from_file(fp, current_str));
}

TEST_F(TestReadStr, empty_line2) {
    char current_str[MAX_STR_LENGTH];
    std::ofstream out(file_name);
    out << "";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(EOF, read_str_from_file(fp, current_str));
}

TEST_F(TestReadStr, eof) {
    char current_str[MAX_STR_LENGTH];
    std::ofstream out(file_name);
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(EOF, read_str_from_file(fp, current_str));
}

// тесты realloc_array

class TestReallocArr : public ::testing::Test {
 protected:
    void SetUp() {
        arr = reinterpret_cast<Song *>(malloc(sizeof(Song) * MIN_ARRAY_SIZE));
        correct_arr = reinterpret_cast<Song *>(malloc(sizeof(Song) * MIN_ARRAY_SIZE));

        strncpy(song1.author, "VTSS", MAX_FIELD_LENGTH);
        strncpy(song1.singer, "VTSS", MAX_FIELD_LENGTH);
        strncpy(song1.name, "Atlantyda", MAX_FIELD_LENGTH);
        strncpy(song1.duration, "1:30", MAX_FIELD_LENGTH);

        strncpy(song2.author, "VTSS", MAX_FIELD_LENGTH);
        strncpy(song2.singer, "VTSS", MAX_FIELD_LENGTH);
        strncpy(song2.name, "Atlantyda", MAX_FIELD_LENGTH);
        strncpy(song2.duration, "1:30", MAX_FIELD_LENGTH);

        arr[0] = song1;
        arr[1] = song2;
        correct_arr[0] = song1;
        correct_arr[1] = song2;
    }
    void TearDown() {
        free(arr);
        free(correct_arr);
    }
    Song song1, song2;
    Song *arr, *correct_arr;
};

TEST_F(TestReallocArr, correct_realloc) {
    ASSERT_EQ(4, realloc_array(&arr, MIN_ARRAY_SIZE));
    ASSERT_STREQ(correct_arr[0].author, arr[0].author);
    ASSERT_STREQ(correct_arr[0].singer, arr[0].singer);
    ASSERT_STREQ(correct_arr[0].name, arr[0].name);
    ASSERT_STREQ(correct_arr[0].duration, arr[0].duration);
    ASSERT_STREQ(correct_arr[1].author, arr[1].author);
    ASSERT_STREQ(correct_arr[1].singer, arr[1].singer);
    ASSERT_STREQ(correct_arr[1].name, arr[1].name);
    ASSERT_STREQ(correct_arr[1].duration, arr[1].duration);
}

TEST_F(TestReallocArr, invalid_size) {
    ASSERT_EQ(NULL_SIZE, realloc_array(&arr, 0));
}

TEST_F(TestReallocArr, null_ptr1) {
    ASSERT_EQ(NULL_PTR, realloc_array(NULL, MIN_ARRAY_SIZE));
}

// тесты search_by_author

class TestSearchAuthor : public ::testing::Test {
 protected:
    void SetUp() {
        all_songs = reinterpret_cast<Song *>(malloc(sizeof(Song) * MIN_ARRAY_SIZE));
        correct_right_songs = reinterpret_cast<Song *>(malloc(sizeof(Song) * MIN_ARRAY_SIZE));

        strncpy(song1.author, "VTSS", MAX_FIELD_LENGTH);
        strncpy(song1.singer, "VTSS", MAX_FIELD_LENGTH);
        strncpy(song1.name, "Atlantyda", MAX_FIELD_LENGTH);
        strncpy(song1.duration, "1:30", MAX_FIELD_LENGTH);

        strncpy(song2.author, "Yaeji", MAX_FIELD_LENGTH);
        strncpy(song2.singer, "Yaeji", MAX_FIELD_LENGTH);
        strncpy(song2.name, "Raingirl", MAX_FIELD_LENGTH);
        strncpy(song2.duration, "2:00", MAX_FIELD_LENGTH);

        all_songs[0] = song1;
        all_songs[1] = song2;
        correct_right_songs[0] = song2;
    }
    void TearDown() {
        if (right_songs != NULL) {
            free(right_songs);
        }
        free(correct_right_songs);
        free(all_songs);
    }
    Song song1, song2;
    Song *right_songs = NULL, *all_songs, *correct_right_songs;
    char my_author[10] = "Yaeji";
    char another_author[10] = "VCL";
};

TEST_F(TestSearchAuthor, correct_searching1) {
    ASSERT_EQ(1, search_by_author(all_songs, MIN_ARRAY_SIZE, my_author, &right_songs));
    ASSERT_STREQ(correct_right_songs[0].author, right_songs[0].author);
    ASSERT_STREQ(correct_right_songs[0].singer, right_songs[0].singer);
    ASSERT_STREQ(correct_right_songs[0].name, right_songs[0].name);
    ASSERT_STREQ(correct_right_songs[0].duration, right_songs[0].duration);
}

TEST_F(TestSearchAuthor, invalid_searching) {
    ASSERT_EQ(ERROR_EMPTY_ARRAY, search_by_author(all_songs, MIN_ARRAY_SIZE, another_author, &right_songs));
}

TEST_F(TestSearchAuthor, null_ptr1) {
    ASSERT_EQ(NULL_PTR, search_by_author(NULL, MIN_ARRAY_SIZE, another_author, &right_songs));
}

TEST_F(TestSearchAuthor, null_ptr2) {
    ASSERT_EQ(NULL_PTR, search_by_author(all_songs, MIN_ARRAY_SIZE, NULL, &right_songs));
}
