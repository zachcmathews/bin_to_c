cmake_minimum_required(VERSION 3.22)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

project(spv_to_h VERSION 1.0)

file(GLOB source src/*.c)
add_executable(spv_to_h ${source})
target_compile_options(spv_to_h PRIVATE -Wall -Werror -Wpedantic)

install(TARGETS spv_to_h)
