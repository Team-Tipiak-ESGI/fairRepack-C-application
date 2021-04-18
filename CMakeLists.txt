cmake_minimum_required(VERSION 3.16)
project(fairRepack_C_application C)

set(CMAKE_C_STANDARD 11)

add_executable(extractor
        src/extractor/Configuration.c src/extractor/Configuration.h
        src/extractor/Item.c src/extractor/Item.h
        src/common/datestamp.c src/common/datestamp.h
        src/extractor/ftp.c src/extractor/ftp.h
        src/extractor/main.c src/extractor/Location.h)

add_executable(merger
        src/merger/main.c)

add_executable(ftpmem
        src/extractor/ftp.c)


add_executable(ftpmem2
        src/extractor/ftpmem2.c)

set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)
find_library(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(extractor ${CURL_LIBRARIES})
target_link_libraries(ftpmem ${CURL_LIBRARIES})
target_link_libraries(ftpmem2 ${CURL_LIBRARIES})
