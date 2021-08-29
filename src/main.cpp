#include <ctime>
#include <random>

#include "core.hpp"
#include "renderer.hpp"
#include "buffer.hpp"
#include "shader.hpp"
#include "map.hpp"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960

int main() {
    Renderer renderer;

    auto window = renderer.init_window(WINDOW_HEIGHT, WINDOW_WIDTH);

    if (!window)
        return -1;

    auto m = std::make_unique<Map>(192, 256);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        renderer.clear();

        m->tex.bind();
        m->shader.bind();
        m->shader.set_u1i("u_Texture", 0);
        renderer.draw(m->vao, m->ib, m->shader);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
