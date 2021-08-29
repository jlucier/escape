cmake_minimum_required(VERSION 3.16)

project(escape LANGUAGES CXX)
add_executable(escape src/main.cpp)

target_compile_features(escape PUBLIC cxx_std_20)
target_compile_options(escape PRIVATE -Wl,--no-undefined -O3 -Wall)

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

target_include_directories(escape PRIVATE
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRECTORIES}
    src/vendor/glm
    src
)
target_link_libraries(escape PRIVATE
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    glfw
)
target_precompile_headers(escape PRIVATE src/core.hpp)
target_sources(escape PRIVATE
    src/buffer.cpp
    src/renderer.cpp
    src/shader.cpp
    src/texture.cpp
    src/map.cpp
)
