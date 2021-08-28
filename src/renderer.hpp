#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "buffer.hpp"
#include "shader.hpp"

#define GLCall(x) \
    GlClearError(); \
    x; \
    GlCheckError(#x, __FILE__, __LINE__);

void GlClearError();
void GlCheckError(const char* func, const char* file, int line);

namespace renderer {
    GLFWwindow* init_window(uint16_t height=1080, uint16_t width=1920);
    void clear();
    void draw(const VertexArray& vao, const IndexBuffer& ib, const Shader& s);
}
