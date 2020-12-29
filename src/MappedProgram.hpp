#include "Wrappers.hpp"
#include "ShaderResourceManager.hpp"

enum Resource { TEX_DIFFUSE, TEX_SPECULAR, TEX_NORMAL, MATRICES, LIGHTS, N_RESOURCES };
using ProgramMap = std::array<GLint, N_RESOURCES>;

struct MappedProgram
{
	GLProgram program;
	ProgramMap map;
	MappedProgram(GLProgram&& _program)
	: program(std::move(_program))
	{
		map[TEX_DIFFUSE] = program.GetResourceLocation(GL_UNIFORM, "tex_diffuse");
		map[TEX_SPECULAR] = program.GetResourceLocation(GL_UNIFORM, "tex_specular");
		map[TEX_NORMAL] = program.GetResourceLocation(GL_UNIFORM, "tex_normal");
		map[MATRICES] = program.GetResourceIndex(GL_UNIFORM_BLOCK, "Matrices");
		map[LIGHTS] = program.GetResourceIndex(GL_SHADER_STORAGE_BLOCK, "Lights");
	}

	void SetDiffuseTex(GLuint64 tex_diffuse)
	{
		if (map[TEX_DIFFUSE] != -1)
			program.UniformHandleui64ARB(map[TEX_DIFFUSE], tex_diffuse);
	}
	void SetSpecularTex(GLuint64 tex_specular)
	{
		if (map[TEX_SPECULAR] != -1)
			program.UniformHandleui64ARB(map[TEX_SPECULAR], tex_specular);
	}
	void SetNormalTex(GLuint64 tex_normal)
	{
		if (map[TEX_NORMAL] != -1)
			program.UniformHandleui64ARB(map[TEX_NORMAL], tex_normal);
	}
	void BindData(const ShaderResourceManager& srm)
	{
		if (map[MATRICES] != -1)
			srm.Matrices().BindBase(GL_UNIFORM_BUFFER, 0);
		if (map[LIGHTS] != -1)
			srm.Lights().BindBase(GL_SHADER_STORAGE_BUFFER, 0);
	}
};