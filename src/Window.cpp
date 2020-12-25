#include <GLFW/glfw3.h>
#include "Window.hpp"

namespace Window
{
GLFWwindow* win;
int height;
int width;

void window_size_callback(GLFWwindow*, int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
}

void SetWin(GLFWwindow* newWin)
{
	win = newWin;
	glfwGetWindowSize(win, &width, &height);
	glfwSetWindowSizeCallback(win, window_size_callback);
}
bool KeyDown(int key) { return glfwGetKey(win, key) == GLFW_PRESS; }
glm::dvec2 CursorPos()
{
	glm::dvec2 pos;
	glfwGetCursorPos(win, &pos.x, &pos.y);
	pos.x -= width / 2;
	pos.y = -pos.y + height / 2;
	return pos;
}
void SetCursorPos(glm::dvec2 pos) { glfwSetCursorPos(win, pos.x + width / 2, -pos.y + height / 2); }
void CenterCursor() { SetCursorPos({ 0, 0 }); }

double lastFrameDuration = 0;
double LastFrameDuration() { return lastFrameDuration; }
void NewFrame()
{
	static double lastFrameBegin = glfwGetTime();
	double lastFrameEnd = glfwGetTime();
	lastFrameDuration = lastFrameEnd - lastFrameBegin;

	lastFrameBegin = lastFrameEnd;
}
}