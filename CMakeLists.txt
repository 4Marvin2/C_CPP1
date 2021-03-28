cmake_minimum_required (VERSION 3.16) 
 
set (PROJECT HW2) 

project (${PROJECT})

find_package(Threads REQUIRED)

set(INC_DIR ${PROJECT_SOURCE_DIR}/include)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)

include_directories("${PROJECT_SOURCE_DIR}/include")

add_library(most_frequent_seqence_serial STATIC
        ${INC_DIR}/serial/utils.h
        ${SRC_DIR}/serial/utils.c)

add_library(most_frequent_seqence_parallel SHARED
        ${INC_DIR}/parallel/utils.h
        ${SRC_DIR}/parallel/utils.c)

add_executable(HW2 ${SRC_DIR}/main.c)

target_link_libraries(HW2 Threads::Threads pthread most_frequent_seqence_serial most_frequent_seqence_parallel)

target_include_directories(HW2 PUBLIC ${INC_DIR})