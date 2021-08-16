#include "buffer.hpp"

// IndexBuffer

IndexBuffer::IndexBuffer(const uint32_t* data, uint32_t count)
    : count(count) {

    GLCall(glGenBuffers(1, &this->renderer_id));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->renderer_id));
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

uint32_t IndexBuffer::getCount() const {
    return this->count;
}

// VertexBuffer

VertexBuffer::VertexBuffer(const void* data, uint32_t size) {
    GLCall(glGenBuffers(1, &this->renderer_id));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, this->renderer_id));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
    GLCall(glDeleteBuffers(1, &this->renderer_id));
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

        offset += gltype_size(elem.type);
        i++;
    }
}
