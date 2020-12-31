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
		ALIGNED(vec4) ambient;
		ALIGNED(bool) flashlight;
		ALIGNED(uint) nLights;
	};

	GLBuffer SSBO;
	Data* data;
	Light* lights;
	uint slots = 100;
	std::vector<uint> freedSlots;
	uint nLights = 0;
	bool off = false;
	bool flashlight = true;

    public:
	Lighting()
	{
		SSBO.Storage(RealSize(slots), nullptr, GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
		data = (Data*)SSBO.MapRange(0, RealSize(slots),
					    GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
		lights = (Light*)ROUNDTO(uintptr_t(data) + sizeof(Data), sizeof(vec4));
		data->flashlight = flashlight;
		data->ambient = vec4(0.1, 0.1, 0.1, 1);
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
		if (!off) data->nLights = nLights;
		lights[slot] = light;
		return slot;
	}
	void removeLight(uint slot)
	{
		lights[slot].type = 0;
		freedSlots.push_back(slot);
		--nLights;
		if (!off) data->nLights = nLights;
	}
	void SwitchOff() { off = true; data->nLights = 0; data->flashlight = false; }
	void SwitchOn() { off = false; data->nLights = nLights; data->flashlight = flashlight; }
	void SetFlashlight(bool on) {
		flashlight = on;
		data->flashlight = flashlight;
	}
	void SetAmbient(vec4 col) {
		data->ambient = col;
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
		lights = (Light*)ROUNDTO(uintptr_t(data) + sizeof(Data), sizeof(vec4));
		slots = newSlots;
		SSBO = std::move(newSSBO);
	}
};