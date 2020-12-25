#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

	float FOV = 110;
	int width = 1024;
	int height = 768;

	vec3 pos{ 0, 0, 0 };
	vec3 forward{ 0, 0, -1 };
	double latitude{ 0 };
	double longtitude{ 0 };

	mat4 viewMatrix{ glm::lookAt(pos, pos + vec3(0, 0, -1), vec3(0, 1, 0)) };
	mat4 projMatrix{ glm::perspective(radians(FOV), (float)width / height, 0.1f, 100.0f) };
    
    public:
    mat4 ViewMatrix() { return viewMatrix; }
    mat4 ProjMatrix() { return projMatrix; }
};