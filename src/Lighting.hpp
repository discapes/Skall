#pragma once
#include "Wrappers.hpp"
#include "Light.hpp"
#include "Defines.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
using namespace glm;

class Lighting
{
	GLBuffer SSBO;
	Light* pLightSSBO;
	uint slots = 100;
	std::vector<uint> freedSlots;
	uint nLights = 0;

    public:
	Lighting()
	{
		SSBO.Storage(RealSize(slots), nullptr,
				  GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
		pLightSSBO = (Light*)SSBO.MapRange(sizeof(uint), slots * sizeof(Light),
							GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
	}
	void Bind() {
		SSBO.BindBase(GL_SHADER_STORAGE_BUFFER, 0);
	}
	uint addLight(Light light) {
		uint slot;
		if (!freedSlots.empty()) {
			slot = freedSlots.back();
			freedSlots.pop_back();
		} else {
			slot = nLights;
			if (slots == nLights)
				ResizeSSBO(slots *= 2);
		}
		++nLights;
		SetNLights(nLights);
		pLightSSBO[slot] = light;
		return slot;
	}
	void removeLight(uint slot) {
		pLightSSBO[slot].type = 0;
		freedSlots.push_back(slot);
		--nLights;
		SetNLights(nLights);
	}
	private:
	uint RealSize(uint slots) { return sizeof(uint) + slots * sizeof(Light); }
	void SetNLights(uint nLights) { SSBO.SubData(0, sizeof(uint), &nLights); }
	void ResizeSSBO(uint newSlots)
	{
		GLBuffer newSSBO;
		newSSBO.Storage(RealSize(newSlots), nullptr, GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
		Light* pNewSSBO = (Light*)newSSBO.MapRange(
			sizeof(uint), newSlots * sizeof(Light), GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
		newSSBO.CopyData(SSBO, 0, 0, RealSize(slots));

		slots = newSlots;
		pLightSSBO = pNewSSBO;
		SSBO = std::move(newSSBO);
	}
};