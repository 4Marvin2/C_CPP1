#include "gtest/gtest.h"
#include <fstream>

#define MAX_FIELD_LENGTH 101
#define NUMBER_OF_FIELDS 4
// + 4 - знаки разделители и конец строки/перевод строки
#define MAX_STR_LENGTH MAX_FIELD_LENGTH * NUMBER_OF_FIELDS + 4

#define ERROR_EMPTY_LINE 1

extern "C" {
#include "utils/utils.h"
}

class TestReadStr : public ::testing::Test
{
protected:
	void SetUp()
	{
		
	}
	void TearDown()
	{
		if (fp != NULL) {
			fclose(fp);
		}
	}
	FILE *fp;
};

TEST_F(TestReadStr, correct_reading) {
	char current_str[MAX_STR_LENGTH];
	std::ofstream out("test.txt");
    out << "VTSS;VTSS;Atlantyda;1:30";
    out.close();
	fp = fopen("test.txt", "r");

	ASSERT_EQ(read_str_from_file(fp, current_str), 0);
}

TEST_F(TestReadStr, empty_line1) {
	char current_str[MAX_STR_LENGTH];
	std::ofstream out("test.txt");
    out << "\n";
    out.close();
	fp = fopen("test.txt", "r");

	ASSERT_EQ(read_str_from_file(fp, current_str), ERROR_EMPTY_LINE);
}

TEST_F(TestReadStr, empty_line2) {
	char current_str[MAX_STR_LENGTH];
	std::ofstream out("test.txt");
    out << "";
    out.close();
	fp = fopen("test.txt", "r");

	ASSERT_EQ(read_str_from_file(fp, current_str), EOF);
}

TEST_F(TestReadStr, eof) {
	char current_str[MAX_STR_LENGTH];
	std::ofstream out("test.txt");
    out.close();
	fp = fopen("test.txt", "r");

	ASSERT_EQ(read_str_from_file(fp, current_str), EOF);
}
