#include <cstdint>
#include <string>

namespace shaders {
    enum ShaderType {
        NONE = -1, VS = 0, FS = 1
    };

    struct ShaderSource {
        std::string vertex_shader;
        std::string fragment_shader;
    };

    constexpr uint32_t type_to_gl(ShaderType s);

    uint32_t compile(const std::string& source, uint32_t type);
    uint32_t create(const std::string& vertex_src, const std::string& fragment_src);
    ShaderSource parse(const std::string& filepath);
    uint32_t from_file(const std::string& filepath);
};

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
