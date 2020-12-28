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

	GLBuffer lightSSBO; //ehh if I get 100x 100byte lights its still only 10kb so could make it a UBO for performance
	void* pLightSSBO; // or SSBO for practise
	int nPointFloats = 0; // or both for a benchmark
	int nDirectionalFloats = 0;
	int nSpotFloats = 0;
	GLuint lightSSBOSize = 2 * sizeof(int) + 100 * sizeof(float);
	std::vector<float> lightData;

    public:
	ShaderResourceManager()
	{
		LOG("Initializing shader resource buffers");
		matrixUBO.Storage(MATSLEN, nullptr, GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
		pMatrixUBO = (mat4::value_type*)matrixUBO.MapRange(0, MATSLEN,
						       GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
		lightSSBO.Storage(lightSSBOSize, nullptr,
				  GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
		pLightSSBO = lightSSBO.MapRange(0, lightSSBOSize,
						GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
		int* p = (int*)pLightSSBO;
		p[0] = 0;
		p[1] = 0;
		p[3] = 0;
	}
	const GLBuffer& Matrices() const { return matrixUBO; }
	const GLBuffer& Lights() const { return lightSSBO; }

	void updateMVPMatrix(mat4 MVP) { memcpy(pMatrixUBO, glm::value_ptr(MVP), sizeof(mat4)); }
	void updateMVMatrix(mat4 MV) { memcpy(pMatrixUBO + sizeof(mat4), glm::value_ptr(MV), sizeof(mat4)); }

	bool resizeLightSSBO()
	{
		LOG("Light SSBO full, resizing");
		if (lightSSBOSize < 3 * sizeof(int) + lightData.size() * sizeof(float)) {
			int newSSBOSize = lightSSBOSize * 2;
			GLBuffer newSSBO;
			newSSBO.Storage(newSSBOSize, nullptr,
					GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
			void* pNewSSBO = newSSBO.MapRange(
				0, newSSBOSize, GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

			lightSSBO = std::move(newSSBO);
			lightSSBOSize = newSSBOSize;
			pLightSSBO = pNewSSBO;
			((int*)pLightSSBO)[0] = nPointFloats;
			((int*)pLightSSBO)[1] = nDirectionalFloats;
			((int*)pLightSSBO)[2] = nSpotFloats;
			memcpy(pLightSSBO, lightData.data(),
			       (nSpotFloats + nDirectionalFloats + nSpotFloats) * sizeof(float));
			return true;
		}
		return false;
	}

	void addLight(PointLight pl)
	{
		LOG("Adding point light");
		// nPointFloats = changedPartIndex
		auto data = pl.getData();
		lightData.insert(lightData.begin() + nPointFloats, data.begin(), data.end());
		if (!resizeLightSSBO()) {
			memcpy(pLightSSBO, lightData.data() + nPointFloats,
			       sizeof(float) * (nDirectionalFloats + nSpotFloats + 1));
			nPointFloats += PointLight::nFloats;
			((int*)pLightSSBO)[0] = nPointFloats;
		}
	}
	void addLight(DirectionalLight dl)
	{
		LOG("Adding directional light");
		auto data = dl.getData();
		lightData.insert(lightData.begin() + nPointFloats + nDirectionalFloats, data.begin(), data.end());
		if (!resizeLightSSBO()) {
			memcpy(pLightSSBO, lightData.data() + nPointFloats + nDirectionalFloats,
			       sizeof(float) * (nSpotFloats + 1));
			nDirectionalFloats += DirectionalLight::nFloats;
			((int*)pLightSSBO)[1] = nDirectionalFloats;
		}
	}
	void addLight(SpotLight sl)
	{
		LOG("Adding spotlight");
		auto data = sl.getData();
		lightData.insert(lightData.end(), data.begin(), data.end());
		if (!resizeLightSSBO()) {
			memcpy(pLightSSBO, lightData.data() + nPointFloats + nDirectionalFloats + nSpotFloats,
			       sizeof(float) * (1));
			nSpotFloats += SpotLight::nFloats;
			((int*)pLightSSBO)[2] = nSpotFloats;
		}
	}
	// TODO remove lights
};