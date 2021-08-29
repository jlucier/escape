#include "renderer.hpp"
#include "buffer.hpp"
#include "shader.hpp"


void GlClearError() {
    while (glGetError() != GL_NO_ERROR);
}

void GlCheckError(const char* func, const char* file, int line) {
    while (uint32_t error = glGetError()) {
        std::stringstream s;
        s << "GL Error [" << error << "] " << func << " " << file << ":" << line;
        throw std::runtime_error(s.str());
    }
}

GLFWwindow* Renderer::init_window(uint16_t height, uint16_t width) {
    if (!glfwInit()) {
        throw std::runtime_error("Could not initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Escape", NULL, NULL);

    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Could not initialize GLFW window");
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("GLEW failed to initialize");
    }
    else {
        std::cout << "RUNNING: " << glGetString(GL_VERSION) << std::endl;
    }

    // set up "camera" / projection
    float aspect_ratio = float(width) / height;
    float h = 2.0f / aspect_ratio;
    float w = 2.0f;

    this->proj = glm::ortho(-w, w, -h, h, -1.0f, 1.0f);

    return window;
}

void Renderer::clear() const {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::draw(const VertexArray& vao, const IndexBuffer& ib, const Shader& s) const {
    s.bind();
    vao.bind();
    ib.bind();

    if (s.has_uniform("u_MVP"))
        s.set_umat4f("u_MVP", this->proj);

    GLCall(glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr));
}
