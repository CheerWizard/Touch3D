#include <Time.hpp>

ClockTime GetTime() {
    system_clock::time_point current_time = system_clock::now();
    current_time += 1h;
    auto current_ms = time_point_cast<milliseconds>(current_time);
    auto current_days = time_point_cast<days>(current_ms);

    // Assert an algorithm for this platform
    auto z = current_days.time_since_epoch().count();
    static_assert(std::numeric_limits<unsigned>::digits >= 18,
            "This algorithm has not been ported to a 16 bit unsigned integer!");
    static_assert(std::numeric_limits<int>::digits >= 20,
            "This algorithm has not been ported to a 16 bit signed integer!");

    // Get year, month, day from days
    z += 719468;
    int era = (z >= 0 ? z : z - 146096) / 146097;
    u32 doe = static_cast<unsigned>(z - era * 146097);          // [0, 146096]
    u32 yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365;  // [0, 399]
    int y = static_cast<int>(yoe) + era * 400;
    u32 doy = doe - (365*yoe + yoe/4 - yoe/100);                // [0, 365]
    u32 mp = (5*doy + 2)/153;                                   // [0, 11]
    u32 d = doy - (153*mp+2)/5 + 1;                             // [1, 31]
    u32 m = mp + (mp < 10 ? 3 : -9);                            // [1, 12]
    y += (m <= 2);

    // Get milliseconds since the local midnight
    auto ms = current_ms - current_days;

    // Get hours, minutes, seconds and milliseconds from milliseconds since midnight
    hours h = duration_cast<hours>(ms);
    ms -= h;
    std::chrono::minutes min = duration_cast<minutes>(ms);
    ms -= min;
    seconds s = duration_cast<seconds>(ms);
    ms -= s;

    ClockTime time = {};
    time.y = y;
    time.m = m;
    time.d = d;
    time.h = h.count();
    time.min = min.count();
    time.s = s.count();
    time.ms = ms.count();
    return time;
}