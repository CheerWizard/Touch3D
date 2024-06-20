#pragma once

#include <cstdint>
#include <cstddef>

using u8 = uint8_t;
using i8 = int8_t;
using u16 = uint16_t;
using i16 = int16_t;
using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;
using i64 = int64_t;

#if defined(T3D_16)
    using isize = i16;
    using usize = u16;
#elif defined(T3D_32)
    using isize = i32;
    using usize = u32;
#else
    using isize = i64;
    using usize = u64;
#endif