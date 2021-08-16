#include <iostream>
#include <stdexcept>

#include "renderer.hpp"
#include "buffer.hpp"
#include "shader.hpp"


GLFWwindow* initWindow() {
    if (!glfwInit()) {
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 920, "Escape", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return nullptr;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW failed to initialize" << std::endl;
        return nullptr;
    }
    else {
        std::cout << "RUNNING: " << glGetString(GL_VERSION) << std::endl;
    }

    return window;
}


int main() {
    auto window = initWindow();
    if (!window)
        return -1;

    float tri_data[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f,
    };

    uint32_t indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    VertexArray vao;

    VertexBuffer vb(tri_data, sizeof(float) * 4 * 2);
    VertexBufferLayout vblayout;
    vblayout.push<float>(2);

    vao.add_buffer(vb, vblayout);

    IndexBuffer ib(indices, 6);

    Shader s("res/shaders/basic.glsl");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        renderer::draw(vao, ib, s);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
