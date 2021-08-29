#pragma once

#include "core.hpp"


class IndexBuffer {
private:
    uint32_t renderer_id;
    uint32_t count;

public:
    IndexBuffer(uint32_t count, const uint32_t* data=nullptr);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;
    uint32_t get_count() const;
    void set_data(const uint32_t* data, uint32_t count) const;
};

class VertexBuffer {
private:
    uint32_t renderer_id;

public:
    VertexBuffer(uint32_t size, const void* data=nullptr);
    ~VertexBuffer();

    void set_data(const void* data, uint32_t size) const;
    void bind() const;
    void unbind() const;
};

class VertexBufferLayout {
private:
    struct VertexBufferElement {
        uint32_t type;
        uint32_t count;
        int normalized;
    };

    std::vector<VertexBufferElement> attribs;
    uint32_t stride = 0;

public:
    uint32_t get_stride() const {
        return stride;
    }

    template<typename T>
    void push(uint32_t count, bool normalized=false) {
        uint32_t gltype = utils::ctype_to_gl<T>();
        this->attribs.push_back({gltype, count, normalized ? GL_TRUE : GL_FALSE});
        this->stride += count * utils::gltype_size(gltype);
    }

    auto begin() const {
        return this->attribs.cbegin();
    }

    auto end() const {
        return this->attribs.cend();
    }

    size_t size() const {
        return this->attribs.size();
    }
};

class VertexArray {
private:
    uint32_t renderer_id;
public:
    VertexArray();
    ~VertexArray();

    void bind() const;
    void unbind() const;

    void add_buffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
};
