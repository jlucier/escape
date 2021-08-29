#pragma once

#include <type_traits>
#include <stdexcept>

#include "GL/glew.h"

#define runtime_assert(x, msg) \
    if (!(x)) { \
        throw std::runtime_error(msg); \
    }

namespace utils {
    template<typename T>
    constexpr uint32_t ctype_to_gl() {
        if constexpr(std::is_same_v<T, float>)
            return GL_FLOAT;
        else if constexpr(std::is_same_v<T, uint32_t>)
            return GL_UNSIGNED_INT;
        else if constexpr(std::is_same_v<T, char> or std::is_same_v<T, uint8_t>)
            return GL_UNSIGNED_BYTE;

        throw std::invalid_argument("Unrecognized type in ctype_to_gl");
    }

    constexpr uint32_t gltype_size(uint32_t type) {
        switch (type) {
            case GL_FLOAT: return 4;
            case GL_UNSIGNED_INT: return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }
        return 0;
    }
}
