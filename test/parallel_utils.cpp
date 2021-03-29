// Copyright 2021 Mikhail Popov

#include <cstdio>
#include <fstream>
#include <iostream>

#include "gtest/gtest.h"

extern "C" {
#include "parallel/utils.h"
#include "../src/parallel/utils.c"
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
    // fp = fopen(file_name, "r");
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

// тесты finding_number_of_processes
class TestFindingNumberOfProcesses : public ::testing::Test {
 protected:
    size_t size_xxs = 50;       // самый маленький размер - 1 процесс
    size_t size_xs = 500;       // размер чуть больше - 2 процесса
    size_t size_s = 2500;       // маленький размер - 3 процесса
    size_t size_m = 7500;       // средний размер - 4 процесса
    size_t size_l = 15000;      // большой размер - 6 процессов
    size_t size_xl = 50000;     // размер больше - 7 процессов
    size_t size_xxl = 200000;   // огромный размер - 10 процессов
};

TEST_F(TestFindingNumberOfProcesses, correct_finding_xxs) {
    ASSERT_EQ(1, finding_number_of_processes(size_xxs));
}

TEST_F(TestFindingNumberOfProcesses, correct_finding_xs) {
    ASSERT_EQ(2, finding_number_of_processes(size_xs));
}

TEST_F(TestFindingNumberOfProcesses, correct_finding_s) {
    ASSERT_EQ(3, finding_number_of_processes(size_s));
}

TEST_F(TestFindingNumberOfProcesses, correct_finding_m) {
    ASSERT_EQ(4, finding_number_of_processes(size_m));
}

TEST_F(TestFindingNumberOfProcesses, correct_finding_l) {
    ASSERT_EQ(6, finding_number_of_processes(size_l));
}

TEST_F(TestFindingNumberOfProcesses, correct_finding_xl) {
    ASSERT_EQ(7, finding_number_of_processes(size_xl));
}

TEST_F(TestFindingNumberOfProcesses, correct_finding_xxl) {
    ASSERT_EQ(10, finding_number_of_processes(size_xxl));
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
        out << "qwerthhhjjjqq";
        out.close();
        pids = reinterpret_cast<int *>(malloc(sizeof(int) * number_of_processes));
        size = create_shared_file(file_name, &arr, fd);
        create_shared_memory(&arr_splitting, number_of_processes);
        finding_partition_of_arr_char(arr_splitting, number_of_processes, arr, size);
        create_shared_memory(&arr_counter, size);

        pthread_mutexattr_init(&attr);
        pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&mutex, &attr);

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
        remove(file_name);
        close(fd);
        free(correct_arr_counter);
        pthread_mutexattr_destroy(&attr);
        pthread_mutex_destroy(&mutex);
        if (pid == PARENT_PID) {
            free_main_resources(fd, arr, size, arr_counter, arr_splitting, number_of_processes);
        }
        
    }
    int *pids = nullptr, *arr_splitting = nullptr, *arr_counter = nullptr, *correct_arr_counter = nullptr;
    char *arr = nullptr;
    int number_of_processes = 3;
    int fd = 0;
    int size;
    int pid;
    char file_name[11] = "./test.txt";
    pthread_mutexattr_t attr;
    pthread_mutex_t mutex;
};

TEST_F(TestSearchNumberOfRepeatingLengthParallel, corect_searching) {
    pid = create_forks(number_of_processes, pids);
    if (pid != PARENT_PID) {
        // for (int i = 0; i < size; i++) {
        //     std::cout << arr[i] << " ";
        // }
        // std::cout << std::endl;
        // for (int i = 0; i < number_of_processes; i++) {
        //     std::cout << arr_splitting[i] << " ";
        // }
        // std::cout << std::endl;
        // for (int i = 0; i < size; i++) {
        //     std::cout << arr_counter[i] << " ";
        // }
        // std::cout << std::endl;
        ASSERT_EQ(CORRECT, search_number_of_repeating_length_parallel(arr,
                                                                      arr_splitting,
                                                                      arr_counter,
                                                                      number_of_processes,
                                                                      mutex));
        for (int i = 0; i < size; i++) {
            std::cout << arr_counter[i] << " ";
        }
        std::cout << std::endl;
        std::cout << pid << std::endl;
    } else {
        for (int i = 0; i < number_of_processes; i++) {
            while (waitpid(pids[i], NULL, 0) > 0) {}
        }
        
        for (int i = 0; i < size; i++) {
        //     std::cout << arr_counter[i] << " ";
            // ASSERT_EQ(correct_arr_counter[i], arr_counter[i]);
        // }
        // std::cout << std::endl;
    }
}