cmake_minimum_required(VERSION 3.16)
project(fairRepack_C_application C)

set(CMAKE_C_STANDARD 11)

add_executable(extractor
        src/extractor/main.c)

find_package(curl REQUIRED)
target_link_libraries(extractor )