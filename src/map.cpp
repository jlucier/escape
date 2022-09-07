#include <algorithm>
#include <array>
#include <chrono>
#include <random>
#include <unordered_set>
#include <vector>

#include "map.hpp"
#include "core.hpp"


double now() {
    std::chrono::duration<double> now = std::chrono::system_clock::now().time_since_epoch();
    return now.count();
}

static const std::array<Color, 13> cave_colors{
    Color::red(),
    Color::green(),
    Color::blue(),
    Color::orange(),
    Color::yellow(),
    Color::teal(),
    Color::cyan(),
    Color::purple(),
    Color::pink(),
    Color::black(),
    Color::magenta(),
    Color::maroon(),
    Color::gray(),
};

struct Coord {
    size_t x;
    size_t y;

    bool operator==(const Coord& o) const {
        return this->x == o.x and this->y == o.y;
    }
};

template <>
struct std::hash<Coord> {
  size_t operator()(const Coord& c) const noexcept {
    return c.x * 1e6 + c.y;
  }
};

class Cave {
public:

    size_t size() const {
        return this->all_coords.size();
    }

    void add(const Coord& c, bool border = false) {
        if (this->all_coords.count(c))
            return;

        this->all_coords.insert(c);
        if (border)
            this->border_coords.push_back(c);
    }

    bool contains(const Coord& c) const {
        return this->all_coords.count(c) > 0;
    }

    auto begin() const {
        return this->all_coords.cbegin();
    }

    auto end() const {
        return this->all_coords.cend();
    }

    auto border_begin() const {
        return this->border_coords.cbegin();
    }

    auto border_end() const {
        return this->border_coords.cend();
    }

private:
    std::unordered_set<Coord> all_coords;
    std::vector<Coord> border_coords;
};

class MapData {
private:
    std::vector<uint8_t> data;

public:
    const size_t height;
    const size_t width;
    Coord entrance;
    Coord exit;
    Coord key;
    std::vector<Cave> caves;

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

    auto& operator()(const Coord& c) {
        return this->operator()(c.y, c.x);
    }

    const auto& operator()(size_t y, size_t x) const {
        return this->data.at(y * this->width + x);
    }

    const auto& operator()(const Coord& c) const {
        return this->operator()(c.y, c.x);
    }

    bool open(size_t y, size_t x) const {
        return !this->operator()(y, x);
    }

    bool open(const Coord& c) const {
        return !this->operator()(c);
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


/*
 * Return true if the coordinate is adjacent to any non-open cell
 */
bool coord_is_edge(const MapData& m, const Coord& c) {
    return (
        !m.open(c.y + 1, c.x)
        || !m.open(c.y - 1, c.x)
        || !m.open(c.y, c.x + 1)
        || !m.open(c.y, c.x - 1)
    );
}


void extents(const MapData& m, const Coord& c, Cave& cave) {
    bool edge = coord_is_edge(m, c);
    cave.add(c, edge);

    for (int j = -1; j <= 1; j++) {
        for (int i = -1; i <= 1; i++) {
            if (i == 0 and j == 0)
                continue;

            Coord o{c.x + i, c.y + j};

            if (o.x < 0 or o.x >= m.width or o.y < 0 or o.y >= m.height)
                continue;

            if (m.open(o) and !cave.contains(o))
                extents(m, o, cave);
        }
    }
}

void find_caves(MapData& m) {
    for (size_t j = 0; j < m.height; j++) {
        for (size_t i = 0; i < m.width; i++) {
            Coord c {i, j};

            if (!m.open(c))
                continue;

            bool unique = true;
            for (auto& cave : m.caves) {
                unique &= !cave.contains(c);
                if (!unique)
                    break;
            }

            if (unique) {
                m.caves.emplace_back();
                extents(m, c, m.caves.back());
            }
        }
    }

    std::sort(m.caves.begin(), m.caves.end(), [](const Cave& a, const Cave& b) {
        return a.size() > b.size();
    });
}

void remove_small_bois(MapData& m, size_t min_size = 10) {
    std::vector<Cave> new_caves;

    for (auto& c : m.caves) {
        if (c.size() >= min_size)
            new_caves.push_back(c);
        else {
            for (auto& coord : c)
                m(coord) = 1;
        }
    }

    m.caves = std::move(new_caves);
}


void create_game_points(MapData& m, size_t seed) {
    std::default_random_engine e(seed);
    std::uniform_real_distribution dist(0.0f, 1.0f);

    Cave& cave = m.caves.front();
    size_t candidates = std::distance(cave.border_begin(), cave.border_end());

    m.entrance = *(size_t(dist(e) * candidates) + cave.border_begin());
    m.exit = *(size_t(dist(e) * candidates) + cave.border_begin());
    m.key = *(size_t(dist(e) * candidates) + cave.border_begin());
}

std::vector<Color> draw_caves(MapData& m) {
    std::vector<Color> img_data;
    img_data.reserve(m.height * m.width);

    // draw normal

    for (size_t j = 0; j < m.height; j++) {
        for (size_t i = 0; i < m.width; i++) {
            if (!m.open(j, i))
                img_data.push_back(Color::light_brown());
            else {
                size_t ri = 0;
                for (auto& c : m.caves) {
                    if (c.contains({i, j}))
                        break;
                    ri++;
                }

                Color rc = cave_colors[(ri + m.caves.size()) % cave_colors.size()];

                img_data.push_back(rc.blend(Color::dark_brown(), 0.05));
            }
        }
    }

    auto big = m.caves.front();

    // draw borders on biggest cave
    // for (auto c = big.border_begin(); c != big.border_end(); c++) {
    //     img_data.at(c->y * m.width + c->x) = Color::red();
    // }

    // draw keypoints

    img_data.at(m.entrance.y * m.width + m.entrance.x) = Color::green();
    img_data.at(m.exit.y * m.width + m.exit.x) = Color::red();
    img_data.at(m.key.y * m.width + m.key.x) = Color::yellow();

    return img_data;
}

void Map::regenerate(size_t seed) {
    double start = now();
    std::cout << "Start" << std::endl;

    std::default_random_engine e(seed);
    std::uniform_real_distribution dist(0.0f, 1.0f);

    MapData m(this->height, this->width);
    for (auto& c : m) {
        c = dist(e) < .55;
    }

    m.smooth(10);
    m.null_border();

    double gen = now();

    find_caves(m);
    remove_small_bois(m);

    double rms = now();

    create_game_points(m, seed);

    std::vector<Color> img_data = draw_caves(m);

    double end = now();

    std::cout << "FINISHED " << m.caves.size() << std::endl;
    std::cout << "Gen: " << 1000 * (gen - start) << std::endl;
    std::cout << "Caves: " << 1000 * (rms - gen) << std::endl;
    std::cout << "Draw: " << 1000 * (end - rms) << std::endl;
    std::cout << "Total: " << 1000 * (end - start) << std::endl;

    this->tex.load_data(this->height, this->width, (uint8_t*)img_data.data());
}
