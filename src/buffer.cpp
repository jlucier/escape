#include "buffer.hpp"
#include "renderer.hpp"

// IndexBuffer

IndexBuffer::IndexBuffer(uint32_t count, const uint32_t* data) : count(count) {
    GLCall(glGenBuffers(1, &this->renderer_id));
    this->bind();
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
    GLCall(glDeleteBuffers(1, &this->renderer_id));
}

void IndexBuffer::bind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->renderer_id));
}

void IndexBuffer::unbind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

uint32_t IndexBuffer::get_count() const {
    return this->count;
}

void IndexBuffer::set_data(const uint32_t* data, uint32_t count) const {
    this->bind();
    GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(uint32_t), data));
}

// VertexBuffer

VertexBuffer::VertexBuffer(uint32_t size, const void* data) {
    GLCall(glGenBuffers(1, &this->renderer_id));
    this->bind();
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
    GLCall(glDeleteBuffers(1, &this->renderer_id));
}

void VertexBuffer::set_data(const void* data, uint32_t size) const {
    this->bind();
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
}

void VertexBuffer::bind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, this->renderer_id));
}

void VertexBuffer::unbind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}


// Vertex Array stuff

VertexArray::VertexArray() {
    GLCall(glGenVertexArrays(1, &this->renderer_id));
}

VertexArray::~VertexArray() {
    GLCall(glDeleteVertexArrays(1, &this->renderer_id));
}

void VertexArray::bind() const {
    GLCall(glBindVertexArray(this->renderer_id));
}

void VertexArray::unbind() const {
    GLCall(glBindVertexArray(0));
}

void VertexArray::add_buffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
    this->bind();
    vb.bind();

    size_t offset = 0;
    uint32_t i = 0;
    for (const auto& elem : layout) {
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(
            i, elem.count, elem.type, elem.normalized, layout.get_stride(), (const void*)offset
        ));

        offset += elem.count * utils::gltype_size(elem.type);
        i++;
    }
}
