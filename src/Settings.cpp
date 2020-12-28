#include <GL/gl.h>

namespace Settings
{
	int width = 1024;
	int Width() { return width; }
	int height = 768;
	int Height() { return height; }
	float fov = 90;
	float FOV() { return fov; }
    bool fullscreen = false;
    bool Fullscreen() { return fullscreen; }

	void SetResolution(int width2, int height2)
	{
		width = width2;
		height = height2;
		glViewport(0, 0, width, height);
	}
}