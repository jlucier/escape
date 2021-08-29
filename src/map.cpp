#include "map.hpp"
#include "core.hpp"


Map::Map(size_t height, size_t width, size_t seed)
    : seed(seed), height(height), width(width), vb(64), ib(6), shader(shader_path) {

    std::vector<Color> img_data;
    img_data.reserve(height * width);

    bool start = false;
    bool color = false;
    float factor = 1 / 32.f;
    size_t border = 2;
    // TODO use seed

    // create two tris for this grid cell
    for (size_t y = 0; y < this->height; y++) {
        for (size_t x = 0; x < this->width; x++) {
            float v;
            if (x < border or y < border or x >= width - border or y >= height - border)
                v = 0.0;
            else
                v = (glm::perlin(glm::vec2(x * factor, y * factor)) + 1) / 2;

            if (v > 0.5)
                img_data.push_back(Color::dark_brown());
            else
                img_data.push_back(Color::light_brown());

            color = !color;
        }
        start = !start;
        color = start;
    }

    this->tex.load_data(height, width, (uint8_t*)img_data.data());

    float asp = float(height) / float(width);
    float verts[] = {
        -1.0f, -asp, 0.0f, 0.0f, // bottom left
        1.0f, -asp, 1.0f, 0.0f, // bottom right
        1.0f, asp, 1.0f, 1.0f, // top right
        -1.0f, asp, 0.0f, 1.0f, // top left
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
