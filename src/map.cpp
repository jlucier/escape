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

static const std::array<Color, 13> room_colors{
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

using Room = std::unordered_set<Coord>;
using RoomList = std::vector<Room>;


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


void extents(const MapData& m, const Coord& c, Room& r) {
    r.insert(c);
    for (int j = -1; j <= 1; j++) {
        for (int i = -1; i <= 1; i++) {
            if (i == 0 and j == 0)
                continue;

            Coord o{c.x + i, c.y + j};

            if (o.x < 0 or o.x >= m.width or o.y < 0 or o.y >= m.height)
                continue;

            if (m.open(o) and !r.count(o))
                extents(m, o, r);
        }
    }
}


RoomList find_rooms(const MapData& m) {
    Room all;
    std::vector<Room> rooms;

    for (size_t j = 0; j < m.height; j++) {
        for (size_t i = 0; i < m.width; i++) {
            Coord c {i, j};

            if (m.open(c) and !all.count(c)) {
                rooms.emplace_back();
                extents(m, c, rooms.back());

                for (auto& coord : rooms.back())
                    all.insert(coord);
            }
        }
    }

    return rooms;
}

RoomList remove_small_bois(MapData& m, const RoomList& rooms, size_t min_size = 10) {
    RoomList new_rooms;

    for (auto& r : rooms) {
        if (r.size() >= min_size)
            new_rooms.push_back(r);
        else {
            for (auto& c : r)
                m(c) = 1;
        }
    }

    return new_rooms;
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

    auto rooms = remove_small_bois(m, find_rooms(m));

    double rms = now();

    std::vector<Color> img_data;
    img_data.reserve(this->height * this->width);

    for (size_t j = 0; j < m.height; j++) {
        for (size_t i = 0; i < m.width; i++) {
            if (!m.open(j, i))
                img_data.push_back(Color::light_brown());
            else {
                size_t ri = 0;
                for (auto& r : rooms) {
                    if (r.count({i, j}))
                        break;
                    ri++;
                }

                Color rc = room_colors[(ri + rooms.size()) % room_colors.size()];

                img_data.push_back(rc.blend(Color::dark_brown(), 0.05));
            }
        }
    }

    double end = now();

    std::cout << "FINISHED " << rooms.size() << std::endl;
    std::cout << "Gen: " << 1000 * (gen - start) << std::endl;
    std::cout << "Rooms: " << 1000 * (rms - gen) << std::endl;
    std::cout << "Draw: " << 1000 * (end - rms) << std::endl;
    std::cout << "Total: " << 1000 * (end - start) << std::endl;

    // for (auto& c : m) {
    //     if (c)
    //         img_data.push_back(Color::light_brown());
    //     else
    //         img_data.push_back(Color::dark_brown());
    // }

    this->tex.load_data(this->height, this->width, (uint8_t*)img_data.data());
}
