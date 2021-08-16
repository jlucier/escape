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