#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Defines.h"

class GLTexture
{
	GLuint texture;
public:
	GLTexture() { glCreateTextures(GL_TEXTURE_2D, 1, &texture); }
	~GLTexture() { if (texture != 0) { glDeleteTextures(1, &texture); LOG("Deleted texture %d", texture); } }
	GLTexture(GLTexture&& other) noexcept : texture(other.texture) { other.texture = 0; }

	void Bind(GLuint unit) { return glBindTextureUnit(unit, texture); }
	void GenerateMipmap() { return glGenerateTextureMipmap(texture); }
	void Storage2D(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
	{
		return glTextureStorage2D(texture, levels, internalformat, width, height);
	}
	void SubImage2D(GLint level, GLint xoffset, GLint yoffset, GLsizei width,
		GLsizei height, GLenum format, GLenum type, const void* pixels)
	{
		return glTextureSubImage2D(texture, level, xoffset, yoffset, width, height, format, type, pixels);
	}
	void Parameteri(GLenum pname, GLint param) { return glTextureParameteri(texture, pname, param); }
};

class GLVertexArray;

class GLBuffer
{
	friend GLVertexArray;
	GLuint buffer;
public:
	GLBuffer() { glCreateBuffers(1, &buffer); }
	~GLBuffer() { if (buffer != 0) { glDeleteBuffers(1, &buffer); LOG("Deleted buffer %d", buffer); } }
	GLBuffer(GLBuffer&& other) noexcept : buffer(other.buffer) { other.buffer = 0; }

	void storage(GLsizeiptr size, const void *data, GLbitfield flags) { return glNamedBufferStorage(buffer, size, data, flags); }
	void subData(GLintptr offset, GLsizeiptr size, const void *data) { return glNamedBufferSubData(buffer, offset, size, data); }
};

class GLVertexArray
{
	GLuint vertexArray;
public:
	GLVertexArray() { glCreateVertexArrays(1, &vertexArray); }
	~GLVertexArray() { if (vertexArray != 0) { glDeleteVertexArrays(1, &vertexArray); LOG("Deleted vertex array %d", vertexArray); } }
	GLVertexArray(GLVertexArray&& other) noexcept : vertexArray(other.vertexArray) { other.vertexArray = 0; }

	void BindVertexBuffer(GLuint bindingindex, const GLBuffer& buffer, GLintptr offset, GLsizei stride) { 
		return glVertexArrayVertexBuffer(vertexArray, bindingindex, buffer.buffer, offset, stride);
	}
	void BindElementBuffer(const GLBuffer& buffer) { return glVertexArrayElementBuffer(vertexArray, buffer.buffer); }
	void VertexAttribFormat(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset) { 
		return glVertexArrayAttribFormat(vertexArray, attribindex, size, type, normalized, relativeoffset); 
	}
	void VertexAttribBinding(GLuint attribindex, GLuint bindingindex) { return glVertexArrayAttribBinding(vertexArray, attribindex, bindingindex); }
	void EnableVertexAttrib(GLuint index) { return glEnableVertexArrayAttrib(vertexArray, index); }
	void bind() { return glBindVertexArray(vertexArray); }
};

class GLProgram;

class GLShader
{
	GLuint shader;
	friend GLProgram;
public:
	GLShader(GLenum shaderType) : shader(glCreateShader(shaderType)) {}
	~GLShader() { if (shader != 0) { glDeleteShader(shader); LOG("Deleted shader %d", shader); } }
	GLShader(GLShader&& other) noexcept : shader(other.shader) { other.shader = 0; }

	void Compile() { return glCompileShader(shader); }
	void CopySource(GLsizei count, const GLchar** string, const GLint* length) { return glShaderSource(shader, count, string, length); }
	void Getiv(GLenum pname, GLint* params) { return glGetShaderiv(shader, pname, params); }
	void GetInfoLog(GLsizei maxLength, GLsizei* length, GLchar* infoLog) { return glGetShaderInfoLog(shader, maxLength, length, infoLog); }
};

class GLProgram
{
	GLuint program;
public:
	GLProgram() : program(glCreateProgram()) {}
	~GLProgram() { if (program != 0) { glDeleteProgram(program); LOG("Deleted program %d", program); } }
	GLProgram(GLProgram&& other) noexcept : program(other.program) { other.program = 0; }

	void AttachShader(GLShader& shader) { return glAttachShader(program, shader.shader); }
	void DetachShader(GLShader& shader) { return glDetachShader(program, shader.shader); }
	void Link() { return glLinkProgram(program); }
	void Use() { return glUseProgram(program); }
	void Getiv(GLenum pname, GLint* params) { return glGetProgramiv(program, pname, params); }
	void GetInfoLog(GLsizei maxLength, GLsizei* length, GLchar* infoLog) { return glGetProgramInfoLog(program, maxLength, length, infoLog); }
	GLint GetUniformLocation(const GLchar* name) { return glGetUniformLocation(program, name); }
};