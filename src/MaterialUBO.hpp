#include <glad/glad.h>
#include <assert.h>
#include "Wrappers.hpp"

class MaterialUBO
{
	GLBuffer UBO;
    public:
	MaterialUBO(float shine, GLuint64 diffuse, GLuint64 specular)
	{
		size_t s = sizeof(shine) + sizeof(diffuse) + sizeof(specular);
		size_t w = 0;
		uint8_t data[s];

		*(float*)(data + w) = shine;
		w += sizeof(shine);

		*(GLuint64*)(data + w) = diffuse;
		w += sizeof(diffuse);

		*(GLuint64*)(data + w) = specular;
		w += sizeof(specular);

		assert(w == s);
		UBO.Storage(s, data, 0);
	}
	void Bind() {
        UBO.BindBase(GL_UNIFORM_BUFFER, 0);
    }
};