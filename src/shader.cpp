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
    glUseProgram(this->renderer_id);
}

void Shader::unbind() const {
    glUseProgram(0);
}
