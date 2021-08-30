#pragma once

#include "core.hpp"
#include "renderer.hpp"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960

class Application {
protected:
    GLFWwindow* window;
    Renderer renderer;

    virtual void on_update() {}
    virtual void on_key(int key, int mods) {}
public:

    Application(const std::string& name, size_t height=WINDOW_HEIGHT, size_t width=WINDOW_WIDTH);
    void run();

private:
    static void raw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};
