cmake_minimum_required(VERSION 4.4)
project(gps VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(example)

find_package(Doxygen REQUIRED)
set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "README.md")
doxygen_add_docs(docs src/gps.h src/gps.c README.md ALL USE_STAMP_FILE)
