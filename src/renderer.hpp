#pragma once

#include "core.hpp"

#define PROJ_FACTOR 2.0f

// forwards

class VertexArray;
class IndexBuffer;
class Shader;

#define GLCall(x) \
    GlClearError(); \
    x; \
    GlCheckError(#x, __FILE__, __LINE__);

void GlClearError();
void GlCheckError(const char* func, const char* file, int line);

class Renderer {
private:
    glm::mat4 proj;
    GLFWwindow* window;

public:
    Renderer() {}
    ~Renderer();

    GLFWwindow* init_window(const std::string& name, uint16_t height=960, uint16_t width=1280);
    void clear() const;
    void swap_buffers() const;
    void draw(const VertexArray& vao, const IndexBuffer& ib, const Shader& s) const;
};
