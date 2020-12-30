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
	struct Data
	{
		uint nLights;
	};

	GLBuffer SSBO;
	Light* lights;
	Data* data;
	uint slots = 100;
	std::vector<uint> freedSlots;
	uint nLights = 0;

    public:
	Lighting()
	{
		SSBO.Storage(RealSize(slots), nullptr, GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
		data = (Data*)SSBO.MapRange(0, RealSize(slots),
					    GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
		lights = (Light*)&data[1];
	}
	void Bind() { SSBO.BindBase(GL_SHADER_STORAGE_BUFFER, 0); }
	uint addLight(Light light)
	{
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
		data->nLights = nLights;
		lights[slot] = light;
		return slot;
	}
	void removeLight(uint slot)
	{
		lights[slot].type = 0;
		freedSlots.push_back(slot);
		--nLights;
		data->nLights = nLights;
	}

    private:
	uint RealSize(uint slots) { return sizeof(Data) + slots * sizeof(Light); }
	void ResizeSSBO(uint newSlots)
	{
		GLBuffer newSSBO;
		newSSBO.Storage(RealSize(newSlots), nullptr,
				GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
		newSSBO.CopyData(SSBO, 0, 0, RealSize(slots));
		data = (Data*)newSSBO.MapRange(0, RealSize(newSlots),
							GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

		slots = newSlots;
		lights = (Light*)&data[1];
		SSBO = std::move(newSSBO);
	}
};