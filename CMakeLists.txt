cmake_minimum_required(VERSION 3.15)
project(Domination C)

set(CMAKE_C_STANDARD 99)

file(GLOB source "src/*.c")
file(GLOB include "src/*.h")

set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

if(NOT CURSES_HAVE_CURSES_H AND CURSES_HAVE_NCURSES_CURSES_H)
    include_directories(${CURSES_INCLUDE_DIR}/ncurses)
endif()

add_executable(Domination ${source} ${include})
target_link_libraries(Domination m)
target_link_libraries(Domination ${CURSES_LIBRARIES})