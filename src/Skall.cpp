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
using namespace std;
using namespace glm;

void run()
{
	
	assert(glfwInit());
	LOG("Initialized GLFW");
#ifdef DBGOUT
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	GLFWwindow* window;
	window = glfwCreateWindow(800, 600, "DEMO", NULL, NULL);
	assert(window);
	LOG("Created window");
	glfwMakeContextCurrent(window);
	assert(gladLoadGL());
	LOG("Loaded OpenGL extensions");
	Window::SetWin(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window,
				       [](GLFWwindow*, int width, int height) { glViewport(0, 0, width, height); });
#ifdef DBGOUT
	enableDebug();
#endif
	//glEnable(GL_BLEND);
//	glEnable(GL_CULL_FACE);
	//glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	GLProgram program = Loader::BuildProgram("shader.vert", "shader.frag");
	GLTexture texture = Loader::LoadTexture("texture.jpg");
	Mesh cube("Cube", Cube::indices, Cube::vertices);

	Camera cam;
	Cameraman player(cam);

	LOG("Initialization complete");
	while (!glfwWindowShouldClose(window)) {
		Window::NewFrame();
		{
			glfwPollEvents();
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);
            player.ProcessInput();
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4 M(1.f);
        mat4 V = cam.ViewMatrix();
        mat4 P = cam.ProjMatrix();
        mat4 MVP = M * V * P;
		program.Use();
        GLint uniMVP = program.GetUniformLocation("MVP");
        glUniformMatrix4fv(uniMVP, 1, false, glm::value_ptr(MVP));
		texture.Bind(0);
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
	cout << endl;
	return 0;
}