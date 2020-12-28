#include <GLFW/glfw3.h>
#include <iostream>
#include "Window.hpp"
#include "Settings.hpp"
#include "Defines.h"

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
	void SetCursorPos(glm::dvec2 pos)
	{
		glfwSetCursorPos(win, pos.x + Settings::Width() / 2, -pos.y + Settings::Height() / 2);
	}
	void CenterCursor() { SetCursorPos({ 0, 0 }); }

	double lastFrameDuration = 0;
	double fps = 0;
	double LastFrameDuration() { return lastFrameDuration; }
	double FPS() { return fps; }
	void NewFrame()
	{
		static double lastTime = glfwGetTime();
		static double lastFPSCount = glfwGetTime();
		static int nFrames = 0;
		double time = glfwGetTime();

		lastFrameDuration = time - lastTime;
		if (time - lastFPSCount >= 1.0) {
			fps = nFrames;
			nFrames = 0;
			lastFPSCount = time;
#ifdef PRINTFPS
			std::cout << fps << std::endl;
#endif
		}

		nFrames++;
		lastTime = time;
	}
}