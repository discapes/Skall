#pragma once
#include "Defines.h"
#ifdef DBGOUT 
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei, const char* message,
			    const void*)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
		return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source) {
	case GL_DEBUG_SOURCE_API: std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION: std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER: std::cout << "Source: Other"; break;
	}
	std::cout << std::endl;

	switch (type) {
	case GL_DEBUG_TYPE_ERROR: std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY: std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE: std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER: std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP: std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP: std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER: std::cout << "Type: Other"; break;
	}
	std::cout << std::endl;

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH: std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM: std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW: std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	}
	std::cout << std::endl;
	std::cout << std::endl;
}

void enableDebug()
{
	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	} else
		LOG("Could not enable debug output! Did you set window hint GLFW_OPENGL_DEBUG_CONTEXT to true?");
}
#endif