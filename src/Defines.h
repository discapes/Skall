#pragma once
#define DBGOUT
#include <stdio.h>
#include <chrono>
#include <thread>
#define LOG(...) { /*printf("%s:%d: ", __FILE__, __LINE__);*/ printf(__VA_ARGS__); puts(""); }
#define FAIL(...) { LOG(__VA_ARGS__); exit(1); }
#define sleep(n) std::this_thread::sleep_for(std::chrono::milliseconds(n));