#include <glm/glm.hpp>
#include "Wrappers.hpp"

struct InterfaceMap
{
	enum Resource { MV, MVP, MATERIAL, LIGHTS, N_RESOURCES };
	std::array<GLint, N_RESOURCES> map;
	GLProgram& program;
	InterfaceMap(GLProgram& program)
	: program(program)
	{
		map[MV] = program.GetResourceLocation(GL_UNIFORM, "MV");
		map[MVP] = program.GetResourceLocation(GL_UNIFORM, "MVP");
		map[MATERIAL] = program.GetResourceIndex(GL_UNIFORM_BLOCK, "Material");
		map[LIGHTS] = program.GetResourceIndex(GL_SHADER_STORAGE_BLOCK, "Lights");
	}
	bool needsMaterial() {
		return map[MATERIAL] == -1;
	}
	bool needsLights() {
		return map[LIGHTS] == -1;
	}
	void SetMV(glm::mat4 mat)
	{
		if (map[MV] != -1)
			program.UniformMatrix4fv(map[MV], 1, false, glm::value_ptr(mat));
	}
	void SetMVP(glm::mat4 mat)
	{
		if (map[MVP] != -1)
			program.UniformMatrix4fv(map[MVP], 1, false, glm::value_ptr(mat));
	}
};