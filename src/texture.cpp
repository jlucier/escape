#include "texture.hpp"


Texture2D::Texture2D() {
    GLCall(glGenTextures(1, &this->renderer_id));
    this->bind();
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
}

Texture2D::~Texture2D() {
    GLCall(glDeleteTextures(1, &this->renderer_id));
}

void Texture2D::bind(uint32_t slot) const {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, this->renderer_id));
}

void Texture2D::unbind() const {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture2D::load_data(size_t height, size_t width, const void* data, uint32_t fmt, uint32_t type) const {
    GLCall(glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, fmt, type, data
    ));
}
