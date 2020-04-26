cmake_minimum_required(VERSION 3.15)
project(Domination C)

set(CMAKE_C_STANDARD 99)
set(default_build_type Release)

set(SOURCES src/main.c src/init.c src/gameLogic.c src/gui.c)
set(HEADERS src/components.h src/init.h src/gameLogic.h src/gui.h)

add_executable(Domination ${SOURCES} ${HEADERS})

target_link_libraries(Domination m)