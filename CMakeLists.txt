cmake_minimum_required(VERSION 3.20)

project(TinyEngine C)

set(CMAKE_C_STANDARD 17)

add_subdirectory(external/SDL)

add_executable(TinyEngine
    src/main.c
)

target_link_libraries(TinyEngine PRIVATE SDL3::SDL3)