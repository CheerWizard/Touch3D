#pragma once

#include <Types.hpp>

#include <chrono>

using namespace std::chrono;
using days = std::chrono::duration<int, std::ratio<86400>>;

struct ClockTime final {
    u32 y;   // year
    u32 m;   // month
    u32 d;   // day
    i64 h;   // hour
    i64 min; // minute
    i64 s;   // second
    i64 ms;  // millisecond
};

class Clock final {

public:
    static ClockTime GetTime();

};