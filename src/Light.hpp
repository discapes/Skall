#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <variant>
#include <array>
using namespace glm;

struct PointLight {
    vec3 pos;
    static constexpr int nFloats = 3;
    std::array<float, nFloats> getData() {
        return { pos.x, pos.y, pos.z }; 
    }
};

struct DirectionalLight {
    vec3 dir;
    static constexpr int nFloats = 3;
    std::array<float, nFloats> getData() {
        return { dir.x, dir.y, dir.z }; 
    }
};

struct SpotLight {
    vec3 dir;
    vec3 pos;
    float cutoff;
    static constexpr int nFloats = 7;
    std::array<float, nFloats> getData() {
        return { dir.x, dir.y, dir.z, pos.x, pos.y, pos.z, cutoff };
    }
};

using Light = std::variant<PointLight, DirectionalLight, SpotLight>;