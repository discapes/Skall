#include "Mesh.hpp"

GLVertexArray VAO;
bool VAO_Init = false;

Mesh::Mesh(const std::vector<GLuint> indices, const std::vector<Vertex> vertices)
{
    if (!VAO_Init)
    {
        VAO_Init = true;
        standardVAO.vertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
        standardVAO.vertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
        standardVAO.vertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));
        standardVAO.vertexAttribBinding(0, 0);
        standardVAO.vertexAttribBinding(1, 0);
        standardVAO.vertexAttribBinding(2, 0);
    }
    GLsizei lengthIndices = indices.size() * sizeof(decltype(indices)::value_type);
    GLsizei lengthVertices = vertices.size() * sizeof(decltype(vertices)::value_type);
    VBO.storage(lengthIndices + lengthVertices, nullptr, GL_DYNAMIC_STORAGE_BIT);
    VBO.subData(0, lengthIndices, indices.data());
    VBO.subData(lengthIndices, lengthVertices, vertices.data());
}
