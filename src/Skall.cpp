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
#include "Material.hpp"
using namespace std;
using namespace glm;

GLFWwindow* createWindow();
void configureContex();

// todo flashlight, world ambient

void run()
{
	LOG("Initializing GLFW");
	assert(glfwInit());

	LOG("Creating window");
	GLFWwindow* window = createWindow();
	Window::SetWindow(window);

	LOG("Loading OpenGL functions");
	assert(gladLoadGL());
	if (!GLAD_GL_ARB_bindless_texture)
		FAIL("GPU unsupported (GL_ARB_bindless_texture)");

	LOG("Configuring context")
	configureContex();

	LOG("Loading assets");
	Mesh cube("Cube", Cube::indices, Cube::vertices);
	GLTexture grass_diffuse = Loader::LoadTexture("grass_diffuse.png");
	GLTexture grass_specular = Loader::LoadTexture("grass_specular.png");
	GLProgram program = Loader::BuildProgram("shader.vert", "shader.frag");

	LOG("Sending assets to GPU");
	GLuint64 grass_diffuse_handle = grass_diffuse.GetTextureHandleARB();
	GLuint64 grass_specular_handle = grass_specular.GetTextureHandleARB();
	glMakeTextureHandleResidentARB(grass_diffuse_handle);
	glMakeTextureHandleResidentARB(grass_specular_handle);
	Material grassBlock(32, grass_diffuse_handle, grass_specular_handle);

	LOG("Initializing world");
	Lighting lighting;
	lighting.addLight(Light({ 1, 1, 1 }, { 1, 1, -1 }, 10));
	lighting.addLight(Light({1,1,1}, {1/3.f, 1/3.f, 1/3.f}));
	lighting.SetFlashlight(true);
	Camera camera;
	Cameraman player(camera);
	InterfaceMap im(program);

	LOG("Initialization complete\n");
	Window::CenterCursor();
	while (!glfwWindowShouldClose(window)) {
		Window::NewFrame();
		{ // INPUT
			glfwPollEvents();
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);
			player.ProcessInput();
			mat4 M = glm::translate(mat4(1.f), { 0, 0, -2 });
			mat4 MVP = camera.ProjMatrix() * camera.ViewMatrix() * M;
			im.SetM(M);
			im.SetMVP(MVP);
			im.SetCamPos(camera.Pos());
			im.SetCamDir(camera.Forward());
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		{ // DRAWING
			lighting.Bind();
			grassBlock.Bind();
			program.Use();
			cube.Draw();
		}
		glfwSwapBuffers(window);
	}
	LOG("\n");
}

void configureContex()
{
	glfwSwapInterval(0);
	glfwSetInputMode(Window::Window(), GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(Window::Window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (DBGOUT)
		enableDebug();
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glClearColor(0.1, 0.1, 0.1, 1);
}

GLFWwindow* createWindow()
{
	if (DBGOUT)
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
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
	GLFWwindow* window = glfwCreateWindow(Settings::Width(), Settings::Height(), "Skall",
					      Settings::Fullscreen() ? glfwGetPrimaryMonitor() : NULL, NULL);
	assert(window);
	glfwMakeContextCurrent(window);
	return window;
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