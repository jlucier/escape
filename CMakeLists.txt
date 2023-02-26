cmake_minimum_required(VERSION 3.24)
include(FetchContent)

project(escape LANGUAGES CXX)
add_executable(escape src/main.cpp)

target_compile_features(escape PUBLIC cxx_std_20)
target_compile_options(escape PRIVATE -Wl,--no-undefined -O3 -Wall)

FetchContent_Declare(
    glfw3
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG        3.3.8
)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG        0.9.9.8
)
FetchContent_MakeAvailable(glfw3 glm)

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

target_include_directories(escape PRIVATE
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRECTORIES}
    src
)
target_link_libraries(escape PRIVATE
    ${OPENGL_LIBRARIES}
    GLEW::glew
    glfw
    glm
)
target_precompile_headers(escape PRIVATE src/core.hpp)
target_sources(escape PRIVATE
    src/buffer.cpp
    src/renderer.cpp
    src/shader.cpp
    src/texture.cpp
    src/map.cpp
    src/application.cpp
)
