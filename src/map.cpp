#include <random>

#include "map.hpp"
#include "core.hpp"

class MapData {
private:
    std::vector<uint8_t> data;
public:
    const size_t height;
    const size_t width;

public:
    MapData(size_t height, size_t width)
        : height(height), width(width) {
        this->data.assign(height * width, 0);
    }

    size_t size() const {
        return this->data.size();
    }

    auto& operator()(size_t y, size_t x) {
        return this->data.at(y * this->width + x);
    }

    size_t num_open_neighbors(size_t y, size_t x) {
        size_t count = 0;
        for (size_t i = std::max(0, int(y) - 1); i <= std::min(this->height - 1, y + 1); i++) {
            for (size_t j = std::max(0, int(x) - 1); j <= std::min(this->width - 1, x + 1); j++) {
                count += this->operator()(i, j) == 0;
            }
        }
        return count;
    }

    void null_border(size_t border=1) {
        for (size_t b = 0; b < border; b++) {
            for (size_t i = 0; i < this->height; i++) {
                // left + right
                this->operator()(i, b) = 1;
                this->operator()(i, this->width - b - 1) = 1;
            }

            for (size_t j = 0; j < this->width; j++) {
                // top + bottom
                this->operator()(b, j) = 1;
                this->operator()(this->height - b - 1, j) = 1;
            }
        }
    }

    /*
    * Smooth map with cellular automata
    */
    void smooth(size_t rounds=8) {
        for (size_t n = 0; n < rounds; n++) {
            for (size_t y = 0; y < this->height; y++) {
                for (size_t x = 0; x < this->width; x++) {
                    size_t open_neighbors = this->num_open_neighbors(y, x);
                    if (open_neighbors < 4)
                        this->operator()(y, x) = 1;
                    else if (open_neighbors > 4)
                        this->operator()(y, x) = 0;
                }
            }
        }
    }

    auto begin() {
        return this->data.begin();
    }

    auto end() {
        return this->data.end();
    }
};

Map::Map(size_t height, size_t width, size_t seed)
    : height(height), width(width), vb(64), ib(6), shader(shader_path) {
    float asp = float(this->height) / float(this->width);
    float verts[] = {
        -1.0f, -asp, 0.0f, 0.0f, // bottom left
        1.0f, -asp, 1.0f, 0.0f, // bottom right
        1.0f, asp, 1.0f, 1.0f, // top right
        -1.0f, asp, 0.0f, 1.0f, // top left
    };

    this->vb.set_data(verts, sizeof(verts));
    this->ib.set_data(this->indices, 6);

    VertexBufferLayout layout;
    layout.push<float>(2);
    layout.push<float>(2);

    this->vao.add_buffer(this->vb, layout);

    this->regenerate(seed);
}

void Map::regenerate(size_t seed) {
    std::default_random_engine e(seed);
    std::uniform_real_distribution dist(0.0f, 1.0f);

    MapData m(this->height, this->width);
    for (auto& c : m) {
        c = dist(e) < .55;
    }

    m.smooth(10);
    m.null_border();

    std::vector<Color> img_data;
    img_data.reserve(this->height * this->width);

    for (auto& c : m) {
        if (c)
            img_data.push_back(Color::light_brown());
        else
            img_data.push_back(Color::dark_brown());
    }

    this->tex.load_data(this->height, this->width, (uint8_t*)img_data.data());
}
