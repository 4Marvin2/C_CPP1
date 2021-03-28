// Copyright 2021 Mikhail Popov

#include <time.h>
#include <sys/time.h>

#include "serial/utils.h"
#include "parallel/utils.h"

int main() {
    char *result_serial = NULL;
    char *result_parallel = NULL;
    struct  timeval start;
    gettimeofday(&start, NULL);

    const char *filename = "text.txt";
    int condition_of_function = search_substring_of_the_most_common_length_serial(filename, &result_serial);
    if (condition_of_function != CORRECT) {
        free(result_serial);
        return 1;
    }

    struct  timeval end;
        gettimeofday(&end, NULL);

    long time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("Время однопроцессорной версии: %ld миллисекунд\n", time);

    gettimeofday(&start, NULL);

    int processes_number =  search_substring_of_the_most_common_length_parallel(filename, &result_parallel);
    if (processes_number == PARENT_PID) {
        gettimeofday(&end, NULL);
        time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
        printf("Время многопроцессорной версии: %ld миллисекунд\n", time);
        if (strncmp(result_serial, result_parallel, strlen(result_parallel)) == 0) {
            printf("Результаты последовательной и параллельной реализации совпадают\n");
        } else {
            printf("Результаты последовательной и параллельной реализации не совпадают\n");
        }
        free(result_parallel);
    } else if (processes_number < 0) {
        free(result_serial);
        free(result_parallel);
        return 1;
    }

    free(result_serial);

    return 0;
}
