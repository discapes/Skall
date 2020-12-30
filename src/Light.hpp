#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <variant>
#include <array>
using namespace glm;

struct Light {
	alignas(int) int type; // 0 = off, 1 = directional, 2 = point, 3 = spot
	alignas(vec4) vec3 color; // 123
    alignas(vec4) vec3 dir; // 13
    alignas(vec4) vec3 pos; // 23
	alignas(float) float linear; // 23
	alignas(float) float quadratic; //23
	alignas(float) float innerCutoff; // 3
	alignas(float) float outerCutoff; // 3

	Light()
		: type(0)
	{}
	Light(vec3 color, vec3 dir)
		: type(1)
		, color(color)
		, dir(dir)
	{}
	Light(vec3 color, vec3 pos, float range)
		: type(2)
		, color(color)
		, pos(pos)
		, linear(4.5 / range)
		, quadratic(75.0 / range * range)
	{}
	Light(vec3 color, vec3 dir, vec3 pos, float range, float innerCutoff, float outerCutoff)
		: type(3)
		, color(color)
		, dir(dir)
		, pos(pos)
		, linear(4.5 / range)
		, quadratic(75.0 / range * range)
		, innerCutoff(cos(radians(innerCutoff)))
		, outerCutoff(cos(radians(outerCutoff)))
	{}
};