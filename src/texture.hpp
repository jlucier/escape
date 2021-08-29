#pragma once

#include "renderer.hpp"

class Texture2D {
private:
    uint32_t renderer_id;

public:
    Texture2D();
    template<typename DT>
    Texture2D(size_t height, size_t width, const DT* data) : Texture2D() {
        this->load_data(height, width, data);
    }

    ~Texture2D();

    void bind(uint32_t slot=0) const;
    void unbind() const;
    void load_data(size_t height, size_t width, const void* data,uint32_t fmt=GL_RGBA,
            uint32_t type=GL_UNSIGNED_INT_8_8_8_8_REV) const;
};
