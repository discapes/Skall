#include <iostream>
#include <fstream>
#include "Wrappers.hpp"
#include "Defines.h"
#include "Loader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

static GLShader compileShader(const char *source, GLenum shaderType)
{
    GLShader shader(shaderType);
    shader.copySource(1, &source, NULL);
    shader.compile();

    int success;
    shader.getiv(GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        shader.getInfoLog(512, NULL, infoLog);
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
    program.attachShader(vertexShader);
    program.attachShader(fragmentShader);
    LOG("Linking program");
    program.link();
    program.detachShader(vertexShader);
    program.detachShader(fragmentShader);

    int success;
    program.getiv(GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        program.getInfoLog(512, NULL, infoLog);
        FAIL("Failed to link program: %s", infoLog);
    }
    return program;
}

GLTexture Loader::LoadTexture(string_view name)
{
    LOG("Loading texture %s", name.data());
    int width, height, nChannels;

    unsigned char *data = stbi_load(name.data(), &width, &height, &nChannels, 0);
    if (!data)
        FAIL("Couldn't read image: %s", stbi_failure_reason());

    glActiveTexture(GL_TEXTURE0);
    GLTexture texture;
    texture.parameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    texture.parameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    constexpr GLsizei num_mipmaps = 4; // TODO
    texture.storage2D(num_mipmaps, GL_RGBA8, width, height);
    texture.subImage2D(0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    texture.generateMipmap();

    stbi_image_free(data);
    return texture;
}