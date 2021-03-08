cmake_minimum_required (VERSION 3.16) 
 
set (PROJECT search_my_author) 
 
project (${PROJECT}) 
 
set (HEADERS 
	../utils.h)
 
set (SOURCES 
	../utils.c
	../main.c) 
 
add_executable (${PROJECT} ${HEADERS} ${SOURCES})