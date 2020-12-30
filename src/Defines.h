#pragma once
#define DBGOUT 1
#define PRINTFPS 1
#include <stdio.h>
#include <chrono>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <thread>
#define LOG(...) { /*printf("%s:%d: ", __FILE__, __LINE__);*/ printf(__VA_ARGS__); puts(""); }
#define FAIL(...) { LOG(__VA_ARGS__); exit(1); }
#define sleep(n) std::this_thread::sleep_for(std::chrono::milliseconds(n));
#define ROUNDTO(offset, alignment) (((offset + alignment - 1) / alignment) * alignment)

template <typename T>
constexpr unsigned int getAlignment() {
    return sizeof(T);
}
template <>
constexpr unsigned int getAlignment<glm::vec3>() {
    return sizeof(glm::vec4);
}
template <>
constexpr unsigned int getAlignment<bool>() {
    return sizeof(uint32_t);
}

#define ALIGNED(T) alignas(getAlignment<T>()) T