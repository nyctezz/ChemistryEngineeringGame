cmake_minimum_required(VERSION 3.20)

project(ChemistryEngineeringGame C)

set(CMAKE_C_STANDARD 17)

add_subdirectory(external/SDL)

add_executable(ChemistryEngineeringGame
    src/main.c
    src/core/window.c
    src/core/application.c
    src/core/renderer.c

    external/glad/src/glad.c
)

target_include_directories(ChemistryEngineeringGame PRIVATE
    external/SDL/include
    external/glad/include
)

target_link_libraries(ChemistryEngineeringGame PRIVATE
    SDL3::SDL3
)