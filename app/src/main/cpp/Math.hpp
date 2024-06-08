#pragma once

#include <Types.hpp>

template<typename T>
union vec2 final {
    T x, y = {};

    inline T& operator [](int i) {
        return *(&x + i);
    }
};

template<typename T>
union vec3 final {
    T x, y, z = {};

    inline T& operator [](int i) {
        return *(&x + i);
    }
};

template<typename T>
union vec4 final {
    T x, y, z, w = {};

    inline T& operator [](int i) {
        return *(&x + i);
    }
};

template<typename T>
struct mat2x2 final {
    vec2<T> m[2] = {};

    inline vec2<T>& operator [](int i) {
        return m[i];
    }
};

template<typename T>
struct mat3x3 final {
    vec3<T> m[3] = {};

    inline vec3<T>& operator [](int i) {
        return m[i];
    }
};

template<typename T>
struct mat4x4 final {
    vec4<T> m[4] = {};

    inline vec4<T>& operator [](int i) {
        return m[i];
    }
};

typedef vec2<float> float2;
typedef vec3<float> float3;
typedef vec4<float> float4;
typedef mat2x2<float> float2x2;
typedef mat3x3<float> float3x3;
typedef mat4x4<float> float4x4;

typedef vec2<int> int2;
typedef vec3<int> int3;
typedef vec4<int> int4;
typedef mat2x2<int> int2x2;
typedef mat3x3<int> int3x3;
typedef mat4x4<int> int4x4;

typedef vec2<double> double2;
typedef vec3<double> double3;
typedef vec4<double> double4;
typedef mat2x2<double> double2x2;
typedef mat3x3<double> double3x3;
typedef mat4x4<double> double4x4;