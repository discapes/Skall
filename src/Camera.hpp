#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Settings.hpp"
using namespace glm;

class Camera;

class Cameraman {
    static constexpr int lookSpeed = 1;
    static constexpr int moveSpeed = 3;

	Camera& camera;
	void ProcessMouse();
	void ProcessKb();

    public:
	Cameraman(Camera& camera)
		: camera(camera)
	{}
	void ProcessInput()
	{
		ProcessMouse();
		ProcessKb();
	}
};

class Camera {
	friend Cameraman;

	vec3 pos{ 0, 0, 0 };
	vec3 forward{ 0, 0, -1 };
	double latitude{ 0 };
	double longtitude{ 0 };

	mat4 viewMatrix{ glm::lookAt(pos, pos + vec3(0, 0, -1), vec3(0, 1, 0)) };
	mat4 projMatrix{ glm::perspective(Settings::FOV(), (float)Settings::Width() / Settings::Height(), 0.1f, 100.0f) };
    
    public:
    mat4 ViewMatrix() { return viewMatrix; }
    mat4 ProjMatrix() { return projMatrix; }
	vec3 Pos() { return pos; }
	vec3 Forward() { return forward; }
};