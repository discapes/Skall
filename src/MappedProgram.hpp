#include "Wrappers.hpp"
#include "UniformDataManager.hpp"

enum Resource { TEX_DIFFUSE, TEX_SPECULAR, TEX_NORMAL, MATRICES, LIGHTS, N_RESOURCES };
using ProgramMap = std::array<GLint, N_RESOURCES>;

struct MappedProgram
{
	GLProgram program;
	ProgramMap map;
	MappedProgram(GLProgram&& program)
		: program(std::move(program))
	{
		map[TEX_DIFFUSE] = program.GetResourceLocation(GL_UNIFORM, "tex_diffuse");
		map[TEX_SPECULAR] = program.GetResourceLocation(GL_UNIFORM, "tex_specular");
		map[TEX_NORMAL] = program.GetResourceLocation(GL_UNIFORM, "tex_normal");
		map[MATRICES] = program.GetResourceLocation(GL_UNIFORM_BLOCK, "matrices");
		map[LIGHTS] = program.GetResourceLocation(GL_SHADER_STORAGE_BLOCK, "lights");
	}

	void SetDiffuseTex(GLuint64 tex_diffuse)
	{
		if (map[TEX_DIFFUSE] != -1)
			program.UniformHandleui64ARB(map[TEX_DIFFUSE], tex_diffuse);
	}
	void SetDiffuseTex(GLuint64 tex_specular)
	{
		if (map[TEX_SPECULAR] != -1)
			program.UniformHandleui64ARB(map[TEX_SPECULAR], tex_specular);
	}
	void SetDiffuseTex(GLuint64 tex_normal)
	{
		if (map[TEX_NORMAL] != -1)
			program.UniformHandleui64ARB(map[TEX_NORMAL], tex_normal);
	}
	void BindData(const UniformDataManager& udm)
	{
		if (map[MATRICES] != -1)
			udm.Matrices().BindBase(GL_UNIFORM_BUFFER, 0);
		if (map[LIGHTS] != -1)
			udm.Lights().BindBase(GL_SHADER_STORAGE_BUFFER, 0);
	}
};