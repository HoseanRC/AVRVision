cmake_minimum_required(VERSION 3.28)
project("AVRVision")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

file(GLOB_RECURSE sources "src/*.c" "src/*.h")

add_executable(avrvision ${sources})
target_compile_options(avrvision PUBLIC -std=gnu17)
target_include_directories(avrvision PUBLIC src)