cmake_minimum_required(VERSION 3.16)
project(fairRepack_C_application C)

set(CMAKE_C_STANDARD 11)

add_executable(extractor
        src/extractor/Configuration.c src/extractor/Configuration.h
        src/extractor/Item.c src/extractor/Item.h
        src/extractor/main.c)

add_executable(merger
        src/merger/main.c)

find_library(CURL REQUIRED)


target_link_libraries(extractor)