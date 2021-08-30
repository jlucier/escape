#include "application.hpp"

Application::Application(const std::string& name, size_t height, size_t width) {
    this->window = this->renderer.init_window(name, height, width);
    glfwSetWindowUserPointer(this->window, this);
    glfwSetKeyCallback(this->window, raw_key_callback);
}

void Application::run() {
    while (!glfwWindowShouldClose(window)) {
        this->renderer.clear();

        this->on_update();

        this->renderer.swap_buffers();
        glfwPollEvents();
    }
}

void Application::raw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (action == GLFW_PRESS)
        app->on_key(key, mods);
}
