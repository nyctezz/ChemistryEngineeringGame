cmake_minimum_required(VERSION 3.20)

project(ChemistryEngineeringGame C)

set(CMAKE_C_STANDARD 17)

add_subdirectory(external/SDL)

add_executable(ChemistryEngineeringGame
    src/main.c
    src/core/window.c
    src/core/application.c
    src/core/renderer.c
    src/core/mesh.c

    src/graphics/buffer.c
    src/graphics/shader.c

    src/game/gameobject.c

    external/glad/src/glad.c
)

add_library(stb_image STATIC
    external/stb/stb_image.c
)

target_include_directories(stb_image PUBLIC
    external/stb
)

target_include_directories(ChemistryEngineeringGame PRIVATE
    external/SDL/include
    external/glad/include
    external/stb
)

target_link_libraries(ChemistryEngineeringGame PRIVATE
    SDL3::SDL3
    stb_image
)

# Automatically copy the assets folder to the build directory on compile
add_custom_command(TARGET ChemistryEngineeringGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "${CMAKE_BINARY_DIR}/assets"
)