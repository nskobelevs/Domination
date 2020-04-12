cmake_minimum_required(VERSION 3.15)
project(Domination C    )

set(CMAKE_C_STANDARD 99)

add_executable(Domination
        src/main.c
        src/init.c
        src/gameLogic.c
        src/gui.c

        src/components.h
        src/init.c
        src/gameLogic.h
        src/gui.h
        )

target_link_libraries(Domination m)