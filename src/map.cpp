#include "map.hpp"
#include "renderer.hpp"

Map::Map(size_t height, size_t width) :
    height(height),
    width(width),
    vb((height + 1) * (width + 1) * sizeof(Vertex)),
    ib(height * width * 6),
    s(shader_path) {
    // # num corners
    this->vertices.reserve((height + 1) * (width + 1));
    this->indices.reserve(height * width * 6);

    this->init();
}

void Map::draw() const {
    renderer::draw(this->vao, this->ib, this->s);
}

void Map::init() {
    float map_height = MAP_END - MAP_START;
    float map_width = MAP_END - MAP_START;

    float dx = map_width / this->width;
    float dy = map_height / this->height;

    // create vertices in row major
    for (size_t j = 0; j < this->v_height(); j++) {
        for (size_t i = 0; i < this->v_width(); i++) {
            this->vertices.emplace_back(
                V2{i * dx + MAP_START, j * dy + MAP_START},
                V4{1.0,0.0,0.0,1.0}
            );
        }
    }

    // create two tris for this grid cell
    for (size_t y = 0; y < this->height; y++) {
        for (size_t x = 0; x < this->width; x++) {
            size_t tl = x + y * this->v_width();
            size_t bl = x + (y + 1) * this->v_width();
            size_t tr = x + 1 + y * this->v_width();
            size_t br = x + 1 + (y + 1) * this->v_width();

            // lower tri
            this->indices.push_back(tl);
            this->indices.push_back(bl);
            this->indices.push_back(tr);

            // upper tri
            this->indices.push_back(bl);
            this->indices.push_back(br);
            this->indices.push_back(tr);
        }
    }

    // intialize "texture"
    this->vb.set_data(this->vertices.data(), this->vertices.size() * sizeof(Vertex));
    this->ib.set_data(this->indices.data(), this->indices.size());

    VertexBufferLayout layout;
    layout.push<float>(2);
    layout.push<float>(4);

    vao.add_buffer(this->vb, layout);
}

std::ostream& operator<<(std::ostream& out, const Map& m) {
    out << "indices: {\n";
    size_t count = 0;
    for (const auto& i : m.indices) {
        out << i << ",";

        if (++count % 6 == 0)
            out << std::endl;
    }

    out << "}\nVertices:\n";

    for (const auto& v : m.vertices) {
        out << v << std::endl;
    }

    return out;
}
