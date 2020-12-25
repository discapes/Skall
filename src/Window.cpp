#include <GLFW/glfw3.h>
#include "Window.hpp"
#include "Settings.hpp"

namespace Window
{
	GLFWwindow* win;

	void window_size_callback(GLFWwindow*, int newWidth, int newHeight)
	{
		Settings::SetResolution(newWidth, newHeight);
	}

	void SetWin(GLFWwindow* newWin)
	{
		win = newWin;
		glfwSetWindowSizeCallback(win, window_size_callback);
	}
	bool KeyDown(int key) { return glfwGetKey(win, key) == GLFW_PRESS; }
	glm::dvec2 CursorPos()
	{
		glm::dvec2 pos;
		glfwGetCursorPos(win, &pos.x, &pos.y);
		pos.x -= Settings::Width() / 2;
		pos.y = -pos.y + Settings::Height() / 2;
		return pos;
	}
	void SetCursorPos(glm::dvec2 pos) { glfwSetCursorPos(win, pos.x + Settings::Width() / 2, -pos.y + Settings::Height() / 2); }
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