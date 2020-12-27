#include <iostream>
#include <fstream>
#include "Wrappers.hpp"
#include "Defines.h"
#include "Loader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;

static GLShader compileShader(const char *source, GLenum shaderType)
{
    GLShader shader(shaderType);
    shader.CopySource(1, &source, NULL);
    shader.Compile();

    int success;
    shader.Getiv(GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        shader.GetInfoLog(512, NULL, infoLog);
        LOG("Failed to compile shader: %s", infoLog);
    }
    return shader;
}

GLProgram Loader::BuildProgram(string_view vertexShaderName, string_view fragmentShaderName)
{
    string vsSource, fsSource;
    ifstream vsStream(vertexShaderName.data());
    ifstream fsStream(fragmentShaderName.data());
    bool vsFound = vsStream.good();
    bool fsFound = fsStream.good();
    LOG("Vertex shader %s %s, fragment shader %s %s",
     vertexShaderName.data(), vsFound ? "found" : "not found",
     fragmentShaderName.data(), fsFound ? "found" : "not found");
    assert(vsFound && fsFound);
    getline(vsStream, vsSource, '\0');
    getline(fsStream, fsSource, '\0');

    LOG("Compiling vertex shader %s", vertexShaderName.data());
    GLShader vertexShader = compileShader(vsSource.c_str(), GL_VERTEX_SHADER);
    LOG("Compiling fragment shader %s", fragmentShaderName.data());
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
    if (!success)
    {
        char infoLog[512];
        program.GetInfoLog(512, NULL, infoLog);
        FAIL("Failed to link program: %s", infoLog);
    }
    return program;
}

GLTexture Loader::LoadTexture(string_view name)
{
    LOG("Loading texture %s", name.data());
    int width, height, nChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(name.data(), &width, &height, &nChannels, 0);
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