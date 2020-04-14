cmake_minimum_required(VERSION 3.15)
project(Domination C)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

set(CMAKE_C_STANDARD 99)

set(SOURCES src/main.c src/init.c src/gameLogic.c src/gui.c)
set(HEADERS src/components.h src/init.h src/gameLogic.h src/gui.h)

if (CMAKE_BUILD_TYPE EQUAL Debug)
    set(HEADERS ${HEADERS} src/log.h)
    set(SOURCES ${SOURCES} src/log.c)
endif()

add_executable(Domination ${SOURCES} ${HEADERS})

target_link_libraries(Domination m)