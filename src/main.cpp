#include <ctime>

#include "core.hpp"
#include "application.hpp"
#include "renderer.hpp"
#include "buffer.hpp"
#include "shader.hpp"
#include "map.hpp"

#define MAP_H 60
#define MAP_W 80

class Escape : public Application {
    mutable std::unique_ptr<Map> m;

    void init_map() {
        this->m->regenerate(time(NULL));
    }

public:
    Escape() : Application("Escape") {
        this->m = std::make_unique<Map>(MAP_H, MAP_W);
    }


protected:
    void on_update() override {
        this->m->tex.bind();
        this->m->shader.bind();
        this->m->shader.set_u1i("u_Texture", 0);
        this->renderer.draw(this->m->vao, this->m->ib, this->m->shader);
    }

    void on_key(int key, int mods) override {
        this->init_map();
    }
};


int main() {
    Escape app;
    app.run();
    return 0;
}
