cmake_minimum_required(VERSION 3.22)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

project(bin_to_c VERSION 0.1)

file(GLOB source src/*.c)
add_executable(bin_to_c ${source})
target_compile_options(bin_to_c PRIVATE -Wall -Werror -Wpedantic)

install(TARGETS bin_to_c)
