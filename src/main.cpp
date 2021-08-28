#include <iostream>
#include <memory>
#include <vector>

#include "renderer.hpp"
#include "buffer.hpp"
#include "shader.hpp"
#include "map.hpp"


int main() {
    auto window = renderer::init_window(1080, 1080);

    if (!window)
        return -1;

    auto m = std::make_unique<Map>(10, 10);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        renderer::clear();

        m->draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
