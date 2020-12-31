#include "Wrappers.hpp"
#include <string>
#include "Vertex.hpp"
#include <vector>

struct Mesh {
	std::vector<Material*> textures;
	GLBuffer VBO;
	GLuint lengthIndices;

    public:
	Mesh(std::string name, const std::vector<GLuint>& indices, const std::vector<Vertex>& vertices);
	void Draw();
	const std::string& Name() { return name; }
};