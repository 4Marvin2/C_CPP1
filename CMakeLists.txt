cmake_minimum_required (VERSION 3.16) 
 
set (PROJECT HW1) 
 
project (${PROJECT}) 
 
include_directories("${PROJECT_SOURCE_DIR}/include")

file(GLOB sources
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.c")
 
add_executable (${PROJECT} ${sources})