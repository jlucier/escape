#include <sstream>
#include "renderer.hpp"

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

namespace renderer {
    void clear() {
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }

    void draw(const VertexArray& vao, const IndexBuffer& ib, const Shader& s) {
        s.bind();
        vao.bind();
        ib.bind();

        GLCall(glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr));
    }
}
