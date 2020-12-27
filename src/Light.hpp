#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <variant>
using namespace glm;

struct PointLight {
    vec3 pos;
};

struct DirectionalLight {
    vec3 dir;
};

struct Spotlight {
    vec3 dir;
    vec3 pos;
    float cutoff;
};

using Light = std::variant<PointLight, DirectionalLight, Spotlight>;