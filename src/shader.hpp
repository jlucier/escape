#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

class Shader {
private:
    uint32_t renderer_id;
    mutable std::unordered_map<std::string, uint32_t> uniform_locations;

    uint32_t get_uniform_loc(const std::string& name) const;

public:
    Shader(const std::string& filepath);
    ~Shader();

    uint32_t id() const;
    void bind() const;
    void unbind() const;

    void set_u4f(const std::string& name, float a, float b, float c, float d) const;
};
