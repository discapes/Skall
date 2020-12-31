#include <glm/vec3.hpp>
#include "Defines.hpp"
using namespace glm;

struct alignas(sizeof(vec4)) Light {
	ALIGNED(int) type; // 0 = off, 1 = directional, 2 = point, 3 = spot
	ALIGNED(vec3) color; // 123
  	ALIGNED(vec3) dir; // 13
    ALIGNED(vec3) pos; // 23
	ALIGNED(float) linear; // 23
	ALIGNED(float) quadratic; //23
	ALIGNED(float) innerCutoff; // 3
	ALIGNED(float) outerCutoff; // 3

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
		, quadratic(75.0 / (range * range))
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