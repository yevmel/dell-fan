cmake_minimum_required (VERSION 2.8)
project (dell-fan) 

option (VERBOSE "print additional information to STDOUT" OFF)

configure_file (sources/config.h.in ${CMAKE_BINARY_DIR}/sources/config.h)
include_directories (${CMAKE_BINARY_DIR}/sources/)

set (SOURCES
	sources/main.c
)

add_executable (${PROJECT_NAME} ${SOURCES})

install (TARGETS ${PROJECT_NAME} DESTINATION /usr/bin)
