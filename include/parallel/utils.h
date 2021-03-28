#ifndef INCLUDE_PARALLEL_UTILS_H_
#define INCLUDE_PARALLEL_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#define CORRECT 0
#define NULL_PTR -1
#define MMAP_FAILED -2
#define FORK_FAILED -3
#define PARENT_PID -4
#define OPEN_FILE_FAILED -5
#define NULL_STREAM -6
#define MUNMAP_FAILED -7

// нахождение подстроки самой часто встречаемой длины (параллельно)
int search_substring_of_the_most_common_length_parallel(const char *filename, char **result);

#endif  // INCLUDE_PARALLEL_UTILS_H_
