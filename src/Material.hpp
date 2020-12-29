#include <glad/glad.h>
#include <assert.h>
#include "Wrappers.hpp"

class Material
{
	GLBuffer UBO;
    public:
	Material(float shine, GLuint64 diffuse, GLuint64 specular)
	{
		size_t s = sizeof(shine) + sizeof(diffuse) + sizeof(specular);
		uint8_t data[s];

		*(float*)(data) = shine;
		*(GLuint64*)(data + sizeof(GLuint64)) = diffuse;
		*(GLuint64*)(data + sizeof(GLuint64) * 2) = specular;
		UBO.Storage(s, data, 0);
	}
	void Bind() {
        UBO.BindBase(GL_UNIFORM_BUFFER, 0);
    }
};