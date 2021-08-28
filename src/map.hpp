#pragma once

#include <iostream>
#include <vector>

#include "buffer.hpp"
#include "shader.hpp"


#define MAP_START -0.8f
#define MAP_END 0.8f

struct V2 {
    float x;
    float y;
};

struct V4 {
    float x;
    float y;
    float z;
    float d;
};

struct Vertex {
    V2 position;
    V4 color;

    Vertex(V2&& pos, V4&& color) : position(pos), color(color) {}

    friend std::ostream& operator<<(std::ostream& out, const Vertex& v) {
        return out << "<Vertex pos={"
            << v.position.x << ","
            << v.position.y << "}"
            << " color={"
            << v.color.x << ","
            << v.color.y << ","
            << v.color.z << ","
            << v.color.d << "}"
            << ">";
    }
};

class Map {
private:
    static constexpr char* shader_path = "res/shaders/basic.glsl";

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    size_t height;
    size_t width;

    VertexArray vao;
    VertexBuffer vb;
    IndexBuffer ib;
    Shader s;

    /*
     * Width in vertices
     */
    constexpr size_t v_width() const {
        return this->width + 1;
    }

    /*
     * Height in vertices
     */
    constexpr size_t v_height() const {
        return this->height+ 1;
    }

    void init();

public:

    Map(size_t height, size_t width);
    void draw() const;
    friend std::ostream& operator<<(std::ostream& out, const Map& m);
};
