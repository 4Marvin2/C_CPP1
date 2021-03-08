#include "gtest/gtest.h"

extern "C" {
#include "utils/utils.h"
}

TEST(utils, ok) {
    char **ptr = new char*[4];
    for(int i = 0; i < 4; i++) {
        ptr[i] = new char[101];
    }
    char *str = "VTSS;VTSS;Atlantyda;1:30";

    ASSERT_EQ(pars_str(str, ';', ptr), 4);

    for(int i = 0; i < 4; i++) {
        delete ptr[i];
    }
    delete []ptr;
}

TEST(utils, not_ok) {
    char **ptr = new char*[4];
    for(int i = 0; i < 4; i++) {
        ptr[i] = new char[101];
    }
    char *str = "VTSS;VTSS;Atlantyda";

    ASSERT_EQ(pars_str(str, ';', ptr), 3);

    for(int i = 0; i < 4; i++) {
        delete ptr[i];
    }
    delete []ptr;
}