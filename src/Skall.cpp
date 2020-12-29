#include <iostream>
#include <time.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Wrappers.hpp"
#include "Defines.h"
#include "Loader.hpp"
#include "GLDebugContext.hpp"
#include "Mesh.hpp"
#include "Cube.hpp"
#include "Window.hpp"
#include "Camera.hpp"
#include "InterfaceMap.hpp"
#include "Lighting.hpp"
#include "MaterialUBO.hpp"
using namespace std;
using namespace glm;

void run()
{
	assert(glfwInit());
	LOG("Initialized GLFW");
#ifdef DBGOUT
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	if (Settings::Fullscreen()) {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		Settings::SetResolution(mode->width, mode->height);
	}
	GLFWwindow* window = glfwCreateWindow(Settings::Width(), Settings::Height(), "Skall", Settings::Fullscreen() ? glfwGetPrimaryMonitor() : NULL, NULL);

	assert(window);
	LOG("Created window");
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
	assert(gladLoadGL());
	assert(GLAD_GL_ARB_bindless_texture);
	LOG("Loaded OpenGL extensions");
	Window::SetWin(window);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	Window::CenterCursor();

#ifdef DBGOUT
	enableDebug();
#endif
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	Mesh cube("Cube", Cube::indices, Cube::vertices);
	GLProgram program = Loader::BuildProgram("shader.vert", "shader.frag");
	InterfaceMap im(program);
	GLTexture grassTexture = Loader::LoadTexture("block.jpg");
	MaterialUBO grassUBO(32, grassTexture.GetTextureHandleARB(), grassTexture.GetTextureHandleARB()); 
	Lighting lighting;
	Camera camera;
	Cameraman player(camera);

	LOG("Initialization complete");
	while (!glfwWindowShouldClose(window)) {
		Window::NewFrame();
		{
			glfwPollEvents();
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);
			player.ProcessInput();
			mat4 MV = camera.ViewMatrix() * glm::translate(mat4(1.f), { 0, 0, -2 });
			mat4 MVP = camera.ProjMatrix() * MV;
			im.SetMV(MV);
			im.SetMVP(MVP);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		lighting.Bind();
		grassUBO.Bind();
		program.Use();
		cube.Draw();

		glfwSwapBuffers(window);
	}
	glfwTerminate();
}

int main()
{
	srand(time(0));
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	cout << endl;
	run();
	cout << flush;
	return 0;
}