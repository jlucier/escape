#pragma once

#include <cstdint>
#include <string>

class Shader {
private:
    uint32_t renderer_id;
public:
    Shader(const std::string& filepath);
    ~Shader();

    uint32_t id() const;
    void bind() const;
    void unbind() const;
};
