#pragma once

#include <unordered_map>

#include "core.hpp"

class Shader {
private:
    uint32_t renderer_id;
    mutable std::unordered_map<std::string, uint32_t> uniform_locations;

    int get_uniform_loc(const std::string& name) const;
    int get_loc_or_throw(const std::string& name) const;

public:
    Shader(const std::string& filepath);
    ~Shader();

    uint32_t id() const;
    void bind() const;
    void unbind() const;
    bool has_uniform(const std::string& name) const;

    void set_u4f(const std::string& name, float a, float b, float c, float d) const;
    void set_u1i(const std::string& name, int i) const;
    void set_umat4f(const std::string& name, const glm::mat4& mat) const;
};
