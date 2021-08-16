#define runtime_assert(x, msg) \
    if (!(x)) { \
        throw std::runtime_error(msg); \
    }

namespace utils {
}
