#pragma once

#include <cstdint>
#include <cstddef>
#include <immintrin.h>

using u8 = uint8_t;
using i8 = int8_t;
using u16 = uint16_t;
using i16 = int16_t;
using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;
using i64 = int64_t;
using f32 = float;
using f64 = double;

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

// 16-bit floating point number
class f16 final
{

public:
    inline f16() : m_value(0) {}
    inline f16(const f16&) = default;
    inline f16(f16&&) = default;
    inline f16(const float f) : m_value(_cvtss_sh(f, _MM_FROUND_CUR_DIRECTION)) {}

    inline f16& operator =(const f16&) = default;
    inline f16& operator =(f16&&) = default;
    inline f16& operator =(const float f) {
        m_value = _cvtss_sh(f, _MM_FROUND_CUR_DIRECTION);
        return *this;
    }

    inline operator float() const {
        return _cvtsh_ss(m_value);
    }

private:
    uint16_t m_value;
};