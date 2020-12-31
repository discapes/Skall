#include <vector>
#include <tuple>
#include "Lighting.hpp"
#include "Camera.hpp"

class World {
    Lighting lighting;
    std::vector<std::pair<GLuint, Light>> lights;

};

class Player {
    Camera camera;
	Cameraman controller(camera);
};