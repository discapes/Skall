#pragma once
#include "Defines.hpp"
#include "Wrappers.hpp"

template <typename T>
constexpr size_t CalcStd430ArraySize(size_t n) {
    if (n == 0) return 0;
    size_t alignedSize = ROUNDTO(sizeof(T), alignof(T));
    size_t total = alignedSize * (n-1) + sizeof(T);
}
template <typename Vars, typename Array>
constexpr size_t CalcStd430SSBOSize(size_t n) {
    size_t arraySize = CalcStd430ArraySize<Array>(n);
    size_t varsSize = ROUNDTO(sizeof(Vars), alignof(Array));
    return arraySize + varsSize;
}

template <typename V, typename A> struct SSBO
{
	GLBuffer ssbo;
	V* vars;
	A* arr;
	uint n = 0;
	uint slots = 100;
	std::vector<uint> freedSlots;

    public:
	SSBO()
	{
		ssbo.Storage(CalcStd430SSBOSize<V, A>(slots), nullptr,
			     GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
		vars = (V*)SSBO.MapRange(0, CalcStd430SSBOSize<V, A>(slots),
					 GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
		arr = (A*)ROUNDTO(uintptr_t(vars) + sizeof(V), alignof(A));
	}
	size_t Size() { return n; }
	size_t Reserved() { return slots; }
	A& Array() { return *arr; }
	V& Vars() { return *vars; }
	size_t Push(const A& a)
	{
		uint slot;
		if (!freedSlots.empty()) {
			slot = freedSlots.back();
			freedSlots.pop_back();
		} else {
			slot = nLights;
			if (slots == nLights)
				Resize(slots *= 2);
		}
		++n;
		Array[slot] = a;
		return slot;
	}
	void Pop(size_t slot, const A& disabled)
	{
		Array[slot] = disabled;
		freedSlots.push_back(slot);
		--n;
	}
	void Resize(uint newSlots)
	{
		GLBuffer newSSBO;
		newSSBO.Storage(CalcStd430SSBOSize<V,A>(newSlots), nullptr,
				GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
		newSSBO.CopyData(SSBO, 0, 0, CalcStd430SSBOSize<V,A>(slots));
		vars = (V*)newSSBO.MapRange(0, CalcStd430SSBOSize<V,A>(newSlots),
						 GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
		arr = (A*)ROUNDTO(uintptr_t(vars) + sizeof(V), alignof(A));
		slots = newSlots;
		SSBO = std::move(newSSBO);
	}
	GLBuffer& Buffer() {
		return ssbo;
	}
};