#include <glm/glm.hpp>
#include "Wrappers.hpp"

struct InterfaceMap
{
	enum Resource { M, MVP, CAMPOS, CAMDIR, MATERIAL, LIGHTS, N_RESOURCES };
	std::array<GLint, N_RESOURCES> map;
	GLProgram& program;
	InterfaceMap(GLProgram& program)
	: program(program)
	{
		map[M] = program.GetResourceLocation(GL_UNIFORM, "M");
		map[MVP] = program.GetResourceLocation(GL_UNIFORM, "MVP");
		map[CAMPOS] = program.GetResourceLocation(GL_UNIFORM, "camPos");
		map[CAMDIR] = program.GetResourceLocation(GL_UNIFORM, "camDir");
		map[MATERIAL] = program.GetResourceIndex(GL_UNIFORM_BLOCK, "Material");
		map[LIGHTS] = program.GetResourceIndex(GL_SHADER_STORAGE_BLOCK, "Lights");
	}
	bool needsMaterial() {
		return map[MATERIAL] == -1;
	}
	bool needsLights() {
		return map[LIGHTS] == -1;
	}
	void SetM(glm::mat4 mat)
	{
		if (map[M] != -1)
			program.UniformMatrix4fv(map[M], 1, false, glm::value_ptr(mat));
	}
	void SetCamPos(glm::vec3 vec)
	{
		if (map[CAMPOS] != -1)
			program.Uniform3f(map[CAMPOS], vec);
	}
	void SetCamDir(glm::vec3 vec)
	{
		if (map[CAMDIR] != -1)
			program.Uniform3f(map[CAMDIR], vec);
	}
	void SetMVP(glm::mat4 mat)
	{
		if (map[MVP] != -1)
			program.UniformMatrix4fv(map[MVP], 1, false, glm::value_ptr(mat));
	}
};