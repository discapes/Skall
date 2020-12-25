#include "Mesh.hpp"
#include "Defines.h"

Mesh::Mesh(std::string name, const std::vector<GLuint>& indices, const std::vector<Vertex>& vertices)
{
	LOG("Loading mesh %s", name.c_str());
	this->name = std::move(name);
	this->lengthIndices = indices.size() * sizeof(GLuint);
	GLsizei lengthVertices = vertices.size() * sizeof(Vertex);
	VBO.storage(lengthIndices + lengthVertices, nullptr, GL_DYNAMIC_STORAGE_BIT);
	VBO.subData(0, lengthIndices, indices.data());
	VBO.subData(lengthIndices, lengthVertices, vertices.data());
}

void Mesh::Draw()
{
	static GLVertexArray VAO;
	static bool VAO_Init = false;
	if (!VAO_Init) {
		VAO_Init = true;
		VAO.VertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
		VAO.VertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
		VAO.VertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));
		VAO.VertexAttribBinding(0, 0);
		VAO.VertexAttribBinding(1, 0);
		VAO.VertexAttribBinding(2, 0);
		VAO.EnableVertexAttrib(0);
		VAO.EnableVertexAttrib(1);
		VAO.EnableVertexAttrib(2);
	}
	VAO.BindElementBuffer(VBO);
	VAO.BindVertexBuffer(0, VBO, this->lengthIndices, sizeof(Vertex));
	VAO.bind();
	glDrawElements(GL_TRIANGLES, lengthIndices / sizeof(Vertex), GL_UNSIGNED_INT, 0);
}