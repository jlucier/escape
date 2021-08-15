cmake_minimum_required(VERSION 3.10)

project(escape LANGUAGES CXX)
add_executable(escape src/main.cpp)

target_compile_features(escape PUBLIC cxx_std_17)
target_compile_options(escape PRIVATE -Wl,--no-undefined -O3 -Wall)

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_include_directories(escape PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(escape PRIVATE ${OPENGL_LIBRARIES} glfw)
target_sources(escape PRIVATE
    src/main.cpp
)
