#include "Wrappers.hpp"
#include <string>
#include "Vertex.hpp"
#include "SSBO.hpp"
#include "Material.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <memory>

struct Mesh
{
	SSBO<void, Material> materials;
	GLBuffer VBO;
	GLuint lengthIndices;
	glm::mat4 modelMatrix;
	std::vector<std::shared_ptr<GLTexture>> texLifetimes;

	Mesh(SSBO<void, Material>&& materials, const std::vector<GLuint>& indices, const std::vector<Vertex>& vertices)
		: materials(std::move(materials))
	{
		this->lengthIndices = indices.size() * sizeof(GLuint);
		GLsizei lengthVertices = vertices.size() * sizeof(Vertex);
		VBO.Storage(lengthIndices + lengthVertices, nullptr, GL_DYNAMIC_STORAGE_BIT);
		VBO.SubData(0, lengthIndices, indices.data());
		VBO.SubData(lengthIndices, lengthVertices, vertices.data());
	}
};