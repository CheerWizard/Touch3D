#pragma once

#include <Types.hpp>

#include <chrono>

using namespace std::chrono;
using days = std::chrono::duration<int, std::ratio<86400>>;

struct ClockTime final {
    u32 y;
    u32 m;
    u32 d;
    i64 h;
    i64 min;
    i64 s;
    i64 ms;
};

class Clock final {

public:
    static ClockTime GetCurrentTime();

};