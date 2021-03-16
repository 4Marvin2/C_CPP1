// Copyright 2021 Mikhail Popov

#include <cstdio>
#include <fstream>

#include "gtest/gtest.h"

#define RANDOM_NUMBER 42  // для проверки некорректного кода ошибки

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

TEST_F(TestInitSong, correct_str_without_enter) {
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

TEST_F(TestInitSong, correct_str_with_enter) {
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

TEST_F(TestInitSong, three_elements_string_ending_with_separator) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;Atlantyda;";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(ERROR_WRONG_NUMBER_OF_VARIABLE_IN_LINE, init_song(fp, &current_song));
}

TEST_F(TestInitSong, three_elements_string_ending_without_separator) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;Atlantyda";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(ERROR_WRONG_NUMBER_OF_VARIABLE_IN_LINE, init_song(fp, &current_song));
}

TEST_F(TestInitSong, three_elements_string_with_gap_in_the_middle_with_separator) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;;1:30";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(ERROR_WRONG_NUMBER_OF_VARIABLE_IN_LINE, init_song(fp, &current_song));
}

TEST_F(TestInitSong, three_elements_string_with_gap_in_the_middle_without_separator) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;1:30";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(ERROR_WRONG_NUMBER_OF_VARIABLE_IN_LINE, init_song(fp, &current_song));
}

TEST_F(TestInitSong, three_elements_string_ending_separator_and_enter) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;Atlantyda;\n";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(ERROR_WRONG_NUMBER_OF_VARIABLE_IN_LINE, init_song(fp, &current_song));
}

TEST_F(TestInitSong, three_elements_string_ending_enter) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;Atlantyda\n";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(ERROR_WRONG_NUMBER_OF_VARIABLE_IN_LINE, init_song(fp, &current_song));
}

TEST_F(TestInitSong, three_elements_string_with_gap_in_the_middle_with_separator_ending_with_enter) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;;1:30\n";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(ERROR_WRONG_NUMBER_OF_VARIABLE_IN_LINE, init_song(fp, &current_song));
}

TEST_F(TestInitSong, three_elements_string_with_gap_in_the_middle_without_separator_ending_with_enter) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;1:30\n";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(ERROR_WRONG_NUMBER_OF_VARIABLE_IN_LINE, init_song(fp, &current_song));
}

TEST_F(TestInitSong, empy_str) {
    std::ofstream out(file_name);
    out << "";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(EOF, init_song(fp, &current_song));
}

TEST_F(TestInitSong, no_str) {
    std::ofstream out(file_name);
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(EOF, init_song(fp, &current_song));
}

TEST_F(TestInitSong, string_from_enter) {
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

TEST_F(TestPars, correct_str_ending_without_enter) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;Atlantyda;1:30";

    ASSERT_EQ(4, pars_str(str, ';', ptr));
    for (int i = 0; i < NUMBER_OF_FIELDS; i++) {
        ASSERT_STREQ(correct_result1[i], ptr[i]);
    }
}

TEST_F(TestPars, correct_str_ending_with_enter) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;Atlantyda;1:30\n";

    ASSERT_EQ(4, pars_str(str, ';', ptr));
    for (int i = 0; i < NUMBER_OF_FIELDS; i++) {
        ASSERT_STREQ(correct_result1[i], ptr[i]);
    }
}

TEST_F(TestPars, three_elements_string_ending_with_separator_and_enter) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;Atlantyda;\n";

    ASSERT_EQ(3, pars_str(str, ';', ptr));
    for (int i = 0; i < NUMBER_OF_FIELDS - 1; i++) {
        ASSERT_STREQ(correct_result1[i], ptr[i]);
    }
}

TEST_F(TestPars, three_elements_string_ending_with_enter) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;Atlantyda\n";

    ASSERT_EQ(3, pars_str(str, ';', ptr));
    for (int i = 0; i < NUMBER_OF_FIELDS - 1; i++) {
        ASSERT_STREQ(correct_result1[i], ptr[i]);
    }
}

TEST_F(TestPars, three_elements_string_ending_with_separator) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;Atlantyda;";

    ASSERT_EQ(3, pars_str(str, ';', ptr));
    for (int i = 0; i < NUMBER_OF_FIELDS - 1; i++) {
        ASSERT_STREQ(correct_result1[i], ptr[i]);
    }
}

TEST_F(TestPars, three_elements_string_ending_without_separator) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;Atlantyda";

    ASSERT_EQ(3, pars_str(str, ';', ptr));
    for (int i = 0; i < NUMBER_OF_FIELDS - 1; i++) {
        ASSERT_STREQ(correct_result1[i], ptr[i]);
    }
}

TEST_F(TestPars, three_elements_string_with_gap_in_the_middle_with_separator) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;;1:30";

    ASSERT_EQ(3, pars_str(str, ';', ptr));
    for (int i = 0; i < NUMBER_OF_FIELDS - 1; i++) {
        ASSERT_STREQ(correct_result2[i], ptr[i]);
    }
}

TEST_F(TestPars, three_elements_string_with_gap_in_the_middle_with_separator_ending_with_enter) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;;1:30\n";

    ASSERT_EQ(3, pars_str(str, ';', ptr));
    std::cout << ptr[2] << " ";
    for (int i = 0; i < NUMBER_OF_FIELDS - 1; i++) {
        ASSERT_STREQ(correct_result2[i], ptr[i]);
    }
}

TEST_F(TestPars, three_elements_string_with_gap_in_the_middle_without_separator) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;1:30";

    ASSERT_EQ(3, pars_str(str, ';', ptr));
    for (int i = 0; i < NUMBER_OF_FIELDS - 1; i++) {
        ASSERT_STREQ(correct_result3[i], ptr[i]);
    }
}

TEST_F(TestPars, three_elements_string_with_gap_in_the_middle_without_separator_ending_with_enter) {
    char str[MAX_STR_LENGTH] = "VTSS;VTSS;1:30\n";

    ASSERT_EQ(3, pars_str(str, ';', ptr));
    for (int i = 0; i < NUMBER_OF_FIELDS - 1; i++) {
        ASSERT_STREQ(correct_result3[i], ptr[i]);
    }
}

TEST_F(TestPars, empty_str) {
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

TEST_F(TestPrintSongs, correct_output) {
    print_songs(all_songs, MIN_ARRAY_SIZE, fp);
    fseek(fp, 0, SEEK_SET);
    fgets(current_str1, 112, fp);
    ASSERT_STREQ(correct_str1, current_str1);
    fgets(current_str2, 113, fp);
    ASSERT_STREQ(correct_str2, current_str2);
}

TEST_F(TestPrintSongs, invalid_output) {
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
        free(all_songs);
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

TEST_F(TestReadDataFile, correct_str) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;Atlantyda;1:30";
    out.close();

    ASSERT_EQ(1, read_data_from_file(file_name, &all_songs));
    ASSERT_STREQ(correct_all_songs[0].author, all_songs[0].author);
    ASSERT_STREQ(correct_all_songs[0].singer, all_songs[0].singer);
    ASSERT_STREQ(correct_all_songs[0].name, all_songs[0].name);
    ASSERT_STREQ(correct_all_songs[0].duration, all_songs[0].duration);
}

TEST_F(TestReadDataFile, correct_str_ending_with_enter) {
    std::ofstream out(file_name);
    out << "VTSS;VTSS;Atlantyda;1:30\n";
    out.close();

    ASSERT_EQ(1, read_data_from_file(file_name, &all_songs));
    ASSERT_STREQ(correct_all_songs[0].author, all_songs[0].author);
    ASSERT_STREQ(correct_all_songs[0].singer, all_songs[0].singer);
    ASSERT_STREQ(correct_all_songs[0].name, all_songs[0].name);
    ASSERT_STREQ(correct_all_songs[0].duration, all_songs[0].duration);
}

TEST_F(TestReadDataFile, correct_two_str) {
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

TEST_F(TestReadDataFile, correct_two_str_with_a_gap_enter) {
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

TEST_F(TestReadDataFile, correct_two_str_with_a_gap_enter_in_begin) {
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

TEST_F(TestReadDataFile, failed_empty_file) {
    std::ofstream out(file_name);
    out.close();

    ASSERT_EQ(ERROR_EMPTY_FILE, read_data_from_file(file_name, &all_songs));
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

TEST_F(TestReadStr, enter_line) {
    char current_str[MAX_STR_LENGTH];
    std::ofstream out(file_name);
    out << "\n";
    out.close();
    fp = fopen(file_name, "r");

    ASSERT_EQ(ERROR_EMPTY_LINE, read_str_from_file(fp, current_str));
}

TEST_F(TestReadStr, empty_line) {
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
    ASSERT_EQ(NULL_SIZE_REALLOC, realloc_array(&arr, 0));
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
        free(right_songs);
        free(correct_right_songs);
        free(all_songs);
    }
    Song song1, song2;
    Song *right_songs = NULL, *all_songs, *correct_right_songs;
    char my_author[10] = "Yaeji";
    char another_author[10] = "VCL";
};

TEST_F(TestSearchAuthor, correct_searching) {
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

// тесты error_decoding

class TestErrorDecoding : public ::testing::Test {
 protected:
    void TearDown() {
        free(decoding_str);
    }
    char *decoding_str;
};

TEST_F(TestErrorDecoding, file_open_error) {
    int songs_count = FILE_OPEN_ERROR;
    decoding_str = error_decoding(songs_count);
    ASSERT_STREQ("Ошибка чтения файла", decoding_str);
}

TEST_F(TestErrorDecoding, null_ptr) {
    int songs_count = NULL_PTR;
    decoding_str = error_decoding(songs_count);
    ASSERT_STREQ("Ошибка: не удалось выделить память под массив песен", decoding_str);
}

TEST_F(TestErrorDecoding, null_ptr_realloc) {
    int songs_count = NULL_PTR_REALLOC;
    decoding_str = error_decoding(songs_count);
    ASSERT_STREQ("Ошибка: не удалось выделить память под расширенный массив", decoding_str);
}

TEST_F(TestErrorDecoding, null_size_realloc) {
    int songs_count = NULL_SIZE_REALLOC;
    decoding_str = error_decoding(songs_count);
    ASSERT_STREQ("Ошибка: не верный размер массива под расширенный массив", decoding_str);
}

TEST_F(TestErrorDecoding, error_empty_line) {
    int songs_count = ERROR_EMPTY_FILE;
    decoding_str = error_decoding(songs_count);
    ASSERT_STREQ("Ошибка: файл не содержит информации", decoding_str);
}

TEST_F(TestErrorDecoding, error_empty_array) {
    int songs_count = ERROR_EMPTY_ARRAY;
    decoding_str = error_decoding(songs_count);
    ASSERT_STREQ("Песен данного автора не найдено", decoding_str);
}

TEST_F(TestErrorDecoding, no_valid_error) {
    int songs_count = RANDOM_NUMBER;
    decoding_str = error_decoding(songs_count);
    ASSERT_STREQ("Не верный код ошибки", decoding_str);
}
