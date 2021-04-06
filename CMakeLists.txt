cmake_minimum_required (VERSION 3.16) 
 
set (PROJECT HW2) 

project (${PROJECT})

find_package(Threads REQUIRED)

##
### Coverage definitions ###
##
set(CMAKE_C_FLAGS "-pedantic -fprofile-arcs -ftest-coverage -O0 -g -Wall -Werror -Wextra")
set(CMAKE_CXX_FLAGS "-pedantic -fprofile-arcs -ftest-coverage -O0 -g")
##
### Coverage definitions ###
##

##
### Test definitions ###
##
configure_file(CMakeLists.txt.in
        googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )

add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build)

enable_testing()
add_subdirectory(test)
##
### Test definitions ###
##

##
### Source definitions ###
##
set(INC_DIR ${PROJECT_SOURCE_DIR}/include)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)

include_directories("${PROJECT_SOURCE_DIR}/include")

add_library(most_frequent_seqence_serial STATIC
        ${INC_DIR}/general/general_functions.h
        ${SRC_DIR}/general/general_functions.c
        ${INC_DIR}/utils.h
        ${SRC_DIR}/serial/utils.c)

add_library(most_frequent_seqence_parallel MODULE
        ${INC_DIR}/general/general_functions.h
        ${SRC_DIR}/general/general_functions.c
        ${INC_DIR}/utils.h
        ${SRC_DIR}/parallel/utils.c)

add_executable(HW2 ${SRC_DIR}/main.c)
add_executable(time_comparison ${SRC_DIR}/time_comparison.c)

target_link_libraries(HW2 Threads::Threads pthread -ldl most_frequent_seqence_serial)
target_link_libraries(time_comparison Threads::Threads pthread -ldl most_frequent_seqence_serial)

target_include_directories(HW2 PUBLIC ${INC_DIR})
target_include_directories(time_comparison PUBLIC ${INC_DIR})
##
### Source definitions ###
##