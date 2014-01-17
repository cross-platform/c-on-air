cmake_minimum_required(VERSION 2.8)

project(ConAir)

add_definitions(
    -std=c++11
    -fno-permissive
    -pedantic
    -Wall
    -Wextra
)

add_subdirectory(dependencies)
add_subdirectory(example)
add_subdirectory(mediatmos)
add_subdirectory(src)
