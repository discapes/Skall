#pragma once
#include "Wrappers.hpp"
#include "Light.hpp"
#include "Defines.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
using namespace glm;
#define MATSLEN sizeof(float) * (4 * 4 + 4 * 4)

class ShaderResourceManager
{
	GLBuffer matrixUBO;
	mat4::value_type* pMatrixUBO;

	GLBuffer lightSSBO;
	Light* pLightSSBO;
	uint lightSSBOSize = sizeof(uint) + sizeof(Light) * 100; // 5,2kb
	std::vector<GLuint> freeSlots;

    public:
	ShaderResourceManager()
	{
		LOG("Initializing shader resource buffers");
		matrixUBO.Storage(MATSLEN, nullptr, GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
		pMatrixUBO = (mat4::value_type*)matrixUBO.MapRange(
			0, MATSLEN, GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
		lightSSBO.Storage(lightSSBOSize, nullptr,
				  GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
		pLightSSBO = (Light*)lightSSBO.MapRange(sizeof(uint), lightSSBOSize,
							GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
	}
	const GLBuffer& Matrices() const { return matrixUBO; }
	const GLBuffer& Lights() const { return lightSSBO; }

	void updateMVPMatrix(mat4 MVP) { memcpy(pMatrixUBO, glm::value_ptr(MVP), sizeof(mat4)); }
	void updateMVMatrix(mat4 MV) { memcpy(pMatrixUBO + sizeof(mat4), glm::value_ptr(MV), sizeof(mat4)); }

	Light* LightMapping() { return pLightSSBO; }
	uint LightBufferSize() { return lightSSBOSize; }
	void setNLights(uint nLights) { lightSSBO.SubData(0, sizeof(uint), &nLights); }
	void resizeLightBuffer(uint newSSBOSize)
	{
		GLBuffer newSSBO;
		newSSBO.Storage(newSSBOSize, nullptr, GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
		Light* pNewSSBO = (Light*)newSSBO.MapRange(
			sizeof(uint), newSSBOSize, GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
		newSSBO.CopyData(lightSSBO, 0, 0, lightSSBOSize);

		lightSSBOSize = newSSBOSize;
		pLightSSBO = pNewSSBO;
		lightSSBO = std::move(newSSBO);
	}
};