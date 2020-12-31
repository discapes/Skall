#include <iostream>
#include <fstream>
#include "Wrappers.hpp"
#include "Defines.h"
#include "Loader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <unordered_map>
#include <vector>
#include <memory>

using namespace std;

static GLShader compileShader(const char* source, GLenum shaderType)
{
	GLShader shader(shaderType);
	shader.CopySource(1, &source, NULL);
	shader.Compile();

	int success;
	shader.Getiv(GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		shader.GetInfoLog(512, NULL, infoLog);
		LOG("Failed to compile shader: %s", infoLog);
	}
	return shader;
}

GLProgram Loader::BuildProgram(string_view vertexShaderName, string_view fragmentShaderName)
{
	string vsSource, fsSource;
	string vsPath = string("rss/shaders/").append(vertexShaderName);
	string fsPath = string("rss/shaders/").append(fragmentShaderName);
	ifstream vsStream(vsPath);
	ifstream fsStream(fsPath);
	bool vsFound = vsStream.good();
	bool fsFound = fsStream.good();
	LOG("Vertex shader '%s' %s, fragment shader '%s' %s", vertexShaderName.data(), vsFound ? "found" : "not found",
	    fragmentShaderName.data(), fsFound ? "found" : "not found");
	assert(vsFound && fsFound);
	getline(vsStream, vsSource, '\0');
	getline(fsStream, fsSource, '\0');

	LOG("Compiling vertex shader '%s'", vertexShaderName.data());
	GLShader vertexShader = compileShader(vsSource.c_str(), GL_VERTEX_SHADER);
	LOG("Compiling fragment shader '%s'", fragmentShaderName.data());
	GLShader fragmentShader = compileShader(fsSource.c_str(), GL_FRAGMENT_SHADER);
	GLProgram program;
	program.AttachShader(vertexShader);
	program.AttachShader(fragmentShader);
	LOG("Linking program");
	program.Link();
	program.DetachShader(vertexShader);
	program.DetachShader(fragmentShader);

	int success;
	program.Getiv(GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		program.GetInfoLog(512, NULL, infoLog);
		FAIL("Failed to link program:\n%s", infoLog);
	}
	return program;
}

GLTexture Loader::LoadTexture(string_view name)
{
	LOG("Loading texture '%s'", name.data());
	int width, height, nChannels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(string("rss/textures/").append(name).data(), &width, &height, &nChannels, 0);
	if (!data)
		FAIL("Couldn't read image: %s", stbi_failure_reason());

	GLTexture texture;
	texture.Parameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	texture.Parameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	constexpr GLsizei num_mipmaps = 4; // TODO
	texture.Storage2D(num_mipmaps, nChannels == 4 ? GL_RGBA8 : GL_RGB8, width, height);
	texture.SubImage2D(0, 0, 0, width, height, nChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
	texture.GenerateMipmap();

	stbi_image_free(data);
	return texture;
}

namespace Cube
{
	extern const std::vector<GLuint> indices;
	extern const std::vector<Vertex> vertices;
}

std::unordered_map<const std::string&, Material> loadedMats;

Mesh Loader::LoadMesh(std::string_view name, std::vector<std::string>& matNames)
{
	std::vector<std::shared_ptr<GLTexture> > texLifetimes;
	SSBO<void, Material> matSSBO;
	for (const std::string& matName : matNames) {
		if (loadedMats.contains(matName)) {
			matSSBO.Push(loadedMats.find(matName)->second);
			// push texlifetimes
		} else {
			std::shared_ptr<GLTexture> diffuseTex = make_shared<GLTexture>();
			std::shared_ptr<GLTexture> specularTex = make_shared<GLTexture>();
			texLifetimes.push_back(diffuseTex);
			texLifetimes.push_back(specularTex);
			GLuint64 diffuseHandle = diffuseTex->GetTextureHandleARB();
			GLuint64 specularHandle = diffuseTex->GetTextureHandleARB();
			glMakeTextureHandleResidentARB(diffuseHandle);
			glMakeTextureHandleResidentARB(specularHandle);
			Material material{ 32, diffuseHandle, specularHandle };
			matSSBO.Push(material);
		}
	}
	if (name == "Cube") {
		Mesh mesh(std::move(matSSBO), Cube::indices, Cube::vertices);
		mesh.texLifetimes = std::move(texLifetimes);
		return mesh;
	} else {
		std::vector<GLuint> indices = Cube::indices;
		std::vector<Vertex> vertices = Cube::vertices;
		Mesh mesh(std::move(matSSBO), Cube::indices, Cube::vertices);
		mesh.texLifetimes = std::move(texLifetimes);
		return mesh;
	}
}

// 24
const std::vector<Vertex> Cube::vertices = {
	{ { 0.5, -0.5, 0.5 }, { 1, 0, 0 }, { 0, 1.f / 3 } },   { { 0.5, -0.5, 0.5 }, { 0, -1, 0 }, { 1, 1.f / 3 } },
	{ { 0.5, -0.5, 0.5 }, { 0, 0, 1 }, { 1, 1.f / 3 } },   { { 0.5, 0.5, 0.5 }, { 1, 0, 0 }, { 0, 2.f / 3 } },
	{ { 0.5, 0.5, 0.5 }, { 0, 1, 0 }, { 1, 2.f / 3 } },    { { 0.5, 0.5, 0.5 }, { 0, 0, 1 }, { 1, 2.f / 3 } },
	{ { -0.5, 0.5, 0.5 }, { -1, 0, 0 }, { 1, 2.f / 3 } },  { { -0.5, 0.5, 0.5 }, { 0, 1, 0 }, { 0, 2.f / 3 } },
	{ { -0.5, 0.5, 0.5 }, { 0, 0, 1 }, { 0, 2.f / 3 } },   { { -0.5, -0.5, 0.5 }, { -1, 0, 0 }, { 1, 1.f / 3 } },
	{ { -0.5, -0.5, 0.5 }, { 0, -1, 0 }, { 0, 1.f / 3 } }, { { -0.5, -0.5, 0.5 }, { 0, 0, 1 }, { 0, 1.f / 3 } },
	{ { 0.5, -0.5, -0.5 }, { 1, 0, 0 }, { 1, 1.f / 3 } },  { { 0.5, -0.5, -0.5 }, { 0, -1, 0 }, { 1, 0 } },
	{ { 0.5, -0.5, -0.5 }, { 0, 0, -1 }, { 0, 1.f / 3 } }, { { 0.5, 0.5, -0.5 }, { 1, 0, 0 }, { 1, 2.f / 3 } },
	{ { 0.5, 0.5, -0.5 }, { 0, 1, 0 }, { 1, 1 } },	       { { 0.5, 0.5, -0.5 }, { 0, 0, -1 }, { 0, 2.f / 3 } },
	{ { -0.5, 0.5, -0.5 }, { -1, 0, 0 }, { 0, 2.f / 3 } }, { { -0.5, 0.5, -0.5 }, { 0, 1, 0 }, { 0, 1 } },
	{ { -0.5, 0.5, -0.5 }, { 0, 0, -1 }, { 1, 2.f / 3 } }, { { -0.5, -0.5, -0.5 }, { -1, 0, 0 }, { 0, 1.f / 3 } },
	{ { -0.5, -0.5, -0.5 }, { 0, -1, 0 }, { 0, 0 } },      { { -0.5, -0.5, -0.5 }, { 0, 0, -1 }, { 1, 1.f / 3 } },
};

// 36
const std::vector<GLuint> Cube::indices = { 0 * 3 + 2, 1 * 3 + 2, 2 * 3 + 2, 2 * 3 + 2, 3 * 3 + 2, 0 * 3 + 2,

					    3 * 3 + 0, 2 * 3 + 0, 6 * 3 + 0, 6 * 3 + 0, 7 * 3 + 0, 3 * 3 + 0,

					    1 * 3 + 1, 5 * 3 + 1, 6 * 3 + 1, 6 * 3 + 1, 2 * 3 + 1, 1 * 3 + 1,

					    4 * 3 + 2, 7 * 3 + 2, 6 * 3 + 2, 6 * 3 + 2, 5 * 3 + 2, 4 * 3 + 2,

					    0 * 3 + 0, 4 * 3 + 0, 5 * 3 + 0, 5 * 3 + 0, 1 * 3 + 0, 0 * 3 + 0,

					    0 * 3 + 1, 3 * 3 + 1, 7 * 3 + 1, 7 * 3 + 1, 4 * 3 + 1, 0 * 3 + 1 };