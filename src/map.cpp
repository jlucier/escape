#include "map.hpp"
#include "core.hpp"


Map::Map(size_t height, size_t width)
    : height(height), width(width), vb(4 * 4 * sizeof(float)), ib(6), shader(shader_path) {

    std::vector<Color> img_data;
    img_data.reserve(height * width);

    bool start = false;
    bool color = false;

    // create two tris for this grid cell
    for (size_t y = 0; y < this->height; y++) {
        for (size_t x = 0; x < this->width; x++) {

            if (color)
                img_data.push_back(Color::dark_brown());
            else
                img_data.push_back(Color::light_brown());

            color = !color;
        }
        start = !start;
        color = start;
    }

    this->tex.load_data(height, width, (uint8_t*)img_data.data());

    float verts[] = {
        -0.8f, -0.8f, 0.0f, 0.0f, // bottom left
        0.8f, -0.8f, 1.0f, 0.0f, // bottom right
        0.8f, 0.8f, 1.0f, 1.0f, // top right
        -0.8f, 0.8f, 0.0f, 1.0f, // top left
    };
    uint32_t indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    this->vb.set_data(verts, sizeof(verts));
    this->ib.set_data(indices, 6);

    VertexBufferLayout layout;
    layout.push<float>(2);
    layout.push<float>(2);

    vao.add_buffer(this->vb, layout);
}
