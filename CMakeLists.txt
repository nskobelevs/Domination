cmake_minimum_required(VERSION 3.15)
project(Domination C)

set(CMAKE_C_STANDARD 99)

set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)
find_package(Curses QUIET)

include_directories(${CURSES_INCLUDE_DIR})

if(NOT CURSES_HAVE_CURSES_H AND CURSES_HAVE_NCURSES_CURSES_H)
    include_directories(${CURSES_INCLUDE_DIR}/ncurses)
endif()

if (CURSES_FOUND AND FALSE)
    add_executable(DominationCurses
            src/main.c
            src/init.c
            src/gameLogic.c
            src/guiCurses.c

            src/components.h
            src/init.c
            src/gameLogic.h
            src/gui.h
            )

    target_link_libraries(DominationCurses m)
    target_link_libraries(DominationCurses ${CURSES_LIBRARIES})

    message(STATUS "\n\n\n\t\t-----ATTENTION-----\nFound Curses library. Build curses version along side simple version\n\n\n")
else()
    message(STATUS "\n\n\n\t\t-----ATTENTION-----\nCurses library not found. Build simple version only\n\n\n")
endif()

add_executable(DominationSimple
        src/main.c
        src/init.c
        src/gameLogic.c
        src/guiSimple.c

        src/components.h
        src/init.c
        src/gameLogic.h
        src/gui.h
        )

target_link_libraries(DominationSimple m)
