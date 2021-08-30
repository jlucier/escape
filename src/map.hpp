#pragma once

#include "buffer.hpp"
#include "shader.hpp"
#include "texture.hpp"

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    static constexpr Color dark_brown() { return Color(62, 39, 35); }
    static constexpr Color light_brown() { return Color(109, 76, 65); }

    static constexpr Color red() { return Color(255, 0, 0); }
    static constexpr Color green() { return Color(0, 255, 0); }
    static constexpr Color blue() { return Color(0, 0, 255); }
    static constexpr Color orange() { return Color(255, 140, 0); }
    static constexpr Color yellow() { return Color(255, 255, 0); }
    static constexpr Color teal() { return Color(0, 128, 128); }
    static constexpr Color cyan() { return Color(0, 255, 255); }
    static constexpr Color purple() { return Color(148, 0, 211); }
    static constexpr Color pink() { return Color(255, 105, 180); }
    static constexpr Color black() { return Color(0, 0, 0); }
    static constexpr Color white() { return Color(255, 255, 255); }
    static constexpr Color magenta() { return Color(255, 0, 255); }
    static constexpr Color maroon() { return Color(128, 25, 20); }
    static constexpr Color gray() { return Color(128, 128, 128); }

    constexpr Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b), a(1) {}
};


class Map {
private:
    static constexpr char* shader_path = "res/shaders/basic_tex.glsl";
    static constexpr uint32_t indices[] = {
        0, 1, 2,
        2, 3, 0
    };

public:
    size_t height;
    size_t width;

    Texture2D tex;
    VertexArray vao;
    VertexBuffer vb;
    IndexBuffer ib;
    Shader shader;

    Map(size_t height, size_t width, size_t seed=0);
    void regenerate(size_t seed=0);
};
