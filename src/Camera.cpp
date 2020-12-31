#include <glm/glm.hpp>
#include "Camera.hpp"
#include "Window.hpp"
#include <iostream>
using namespace glm;

void Cameraman::ProcessMouse()
{
	static dvec2 prevcursorpos = { 0, 0 };
	dvec2 cursorpos = Window::CursorPos();
    dvec2 cpos = cursorpos - prevcursorpos;
	prevcursorpos = cursorpos;
	camera.longtitude += lookSpeed * 0.005 * cpos.x;
	camera.latitude += lookSpeed * 0.005 * cpos.y;
	//camera.latitude = clamp(camera.latitude, -1., 1.);
	if (camera.longtitude > 1.) camera.longtitude = -1.;
    else if (camera.longtitude < -1.) camera.longtitude = 1.;
	
	double latRadians = camera.latitude * half_pi<double>(); // radians in circle = 2pi, 1/4 circle = half_pi, so you can look 90 degrees up or down
	double longRadians = camera.longtitude * pi<double>();
	double sinLong = sin(longRadians);
	double cosLong = cos(longRadians);
	double sinLat = sin(latRadians);
	double cosLat = cos(latRadians);

	camera.forward = vec3(
		sinLong * cosLat,
		sinLat,
		-cosLat * cosLong
	);
	
	vec3 up = vec3( // taking advantage of the fact that sin(x + pi / 2) = cos(x) and cos(x + pi / 2) = -sin(x)
		sinLong * -sinLat,
		cosLat,
		sinLat * cosLong
	);
	
    camera.viewMatrix = glm::lookAt(camera.pos, camera.pos + camera.forward, up);
}

void Cameraman::ProcessKb()
{
    double deltaT = Window::LastFrameDuration();

    if (Window::KeyDown(GLFW_KEY_SPACE))
		camera.pos.y += moveSpeed * deltaT;
	if (Window::KeyDown(GLFW_KEY_LEFT_SHIFT))
		camera.pos.y -= moveSpeed * deltaT;

    double forwardFlatMagnitude = sqrt(camera.forward.x * camera.forward.x + camera.forward.z * camera.forward.z);
	vec3 forwardFlat = vec3(camera.forward.x / forwardFlatMagnitude, 0, camera.forward.z / forwardFlatMagnitude);

	vec3 moveDelta(0,0,0);
	if (Window::KeyDown(GLFW_KEY_W))
	{
		moveDelta.x += (forwardFlat.x * moveSpeed) * deltaT;
		moveDelta.z += (forwardFlat.z * moveSpeed) * deltaT;
	}
	if (Window::KeyDown(GLFW_KEY_S))
	{
		moveDelta.x -= (forwardFlat.x * moveSpeed) * deltaT;
		moveDelta.z -= (forwardFlat.z * moveSpeed) * deltaT;
	}

	if (Window::KeyDown(GLFW_KEY_A))
	{
		moveDelta.x += (forwardFlat.z * moveSpeed) * deltaT;
		moveDelta.z += (-forwardFlat.x * moveSpeed) * deltaT;
	}
	if (Window::KeyDown(GLFW_KEY_D))
	{
		moveDelta.x -= (forwardFlat.z * moveSpeed) * deltaT;
		moveDelta.z -= (-forwardFlat.x * moveSpeed) * deltaT;
	}
	normalize(moveDelta);
	camera.pos += moveDelta;
}

