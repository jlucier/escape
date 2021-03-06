#include <fstream>
#include <stdexcept>
#include <sstream>
#include <string>

#include "renderer.hpp"
#include "shader.hpp"
#include "utils.hpp"


namespace shaders {
    enum ShaderType {
        NONE = -1, VS = 0, FS = 1
    };

    struct ShaderSource {
        std::string vertex_shader;
        std::string fragment_shader;
    };

    constexpr uint32_t stype_to_gl(ShaderType s) {
        switch(s) {
            case ShaderType::VS:
                return GL_VERTEX_SHADER;
            case ShaderType::FS:
                return GL_FRAGMENT_SHADER;
            default:
                throw std::runtime_error("Unknown shader type");
        }
    }

    uint32_t compile(const std::string& source, ShaderType type) {
        uint32_t s = glCreateShader(stype_to_gl(type));
        const char* src = source.c_str();

        glShaderSource(s, 1, &src, NULL);
        glCompileShader(s);

        int result;
        glGetShaderiv(s, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            int length;
            glGetShaderiv(s, GL_INFO_LOG_LENGTH, &length);

            char* message = (char*) alloca(length * sizeof(char));
            glGetShaderInfoLog(s, length, &length, message);

            std::stringstream error;
            error << "Error compiling " << (type == ShaderType::VS ? "vertex" : "fragment")
                << " shader: " << message;

            glDeleteShader(s);
            throw std::runtime_error(error.str());
        }


        return s;
    }

    uint32_t create(const std::string& vertex_src, const std::string& fragment_src) {
        uint32_t prog = glCreateProgram();
        auto vs = compile(vertex_src, ShaderType::VS);
        auto fs = compile(fragment_src, ShaderType::FS);
        glAttachShader(prog, vs);
        glAttachShader(prog, fs);

        glLinkProgram(prog);
        glValidateProgram(prog);

        // TODO errors

        glDeleteShader(vs);
        glDeleteShader(fs);

        return prog;
    }

    ShaderSource parse(const std::string& filepath) {
        std::ifstream f(filepath);

        std::string line;
        std::stringstream vs;
        std::stringstream fs;

        ShaderType stype = ShaderType::NONE;

        while(std::getline(f, line)) {
            if (line.starts_with("#shader")) {
                if (line.find("vertex") != std::string::npos) {
                    stype = ShaderType::VS;
                }
                else if (line.find("fragment") != std::string::npos) {
                    stype = ShaderType::FS;
                }
            }
            else {
                switch(stype) {
                    case ShaderType::VS:
                        vs << line << std::endl;
                        break;
                    case ShaderType::FS:
                        fs << line << std::endl;
                        break;
                    default:
                        throw std::runtime_error("Error parsing shader file: " + filepath);
                }

            }
        }

        return {vs.str(), fs.str()};
    }

    uint32_t from_file(const std::string& filepath) {
        auto src = parse(filepath);
        return create(src.vertex_shader, src.fragment_shader);
    }
};

Shader::Shader(const std::string& filepath)
    : renderer_id(shaders::from_file(filepath)) {}

Shader::~Shader() {
    glDeleteProgram(this->renderer_id);
}

uint32_t Shader::id() const {
    return this->renderer_id;
}

void Shader::bind() const {
    GLCall(glUseProgram(this->renderer_id));
}

void Shader::unbind() const {
    glUseProgram(0);
}

int Shader::get_uniform_loc(const std::string& name) const {
    if (this->uniform_locations.contains(name))
        return this->uniform_locations[name];

    GLCall(int loc = glGetUniformLocation(this->renderer_id, name.c_str()));
    this->uniform_locations[name] = loc;
    return loc;
}

int Shader::get_loc_or_throw(const std::string& name) const {
    int loc = this->get_uniform_loc(name);
    if (loc == -1)
        throw std::runtime_error("Cannot set uniform with unknown location: " + name);
    return loc;
}

bool Shader::has_uniform(const std::string& name) const {
    return this->get_uniform_loc(name) != -1;
}

void Shader::set_u4f(const std::string& name, float a, float b, float c, float d) const {
    GLCall(glUniform4f(this->get_loc_or_throw(name), a, b, c, d));
}

void Shader::set_u1i(const std::string& name, int i) const {
    GLCall(glUniform1i(this->get_loc_or_throw(name), i));
}

void Shader::set_umat4f(const std::string& name, const glm::mat4& mat) const {
    GLCall(glUniformMatrix4fv(this->get_loc_or_throw(name), 1, GL_FALSE, &mat[0][0]));
}
