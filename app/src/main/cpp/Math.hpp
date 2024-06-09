#pragma once

#include <Types.hpp>

#include <cmath>
#include <random>

// to avoid using std::pow, std::sin, etc.
using namespace std;

template<typename T>
struct vec2 final {
    T x, y;

    inline T& operator [](int i) {
        return *(&x + i);
    }

    inline T length() {
        return sqrt(x * x + y * y);
    }

    inline void normalize() {
        T l = length();
        x /= l;
        y /= l;
    }

    inline friend vec2 operator +(const vec2<T>& v1, const vec2<T>& v2) {
        return { v1.x + v2.x, v1.y + v2.y };
    }

    inline friend vec2 operator -(const vec2<T>& v1, const vec2<T>& v2) {
        return { v1.x - v2.x, v1.y - v2.y };
    }

    inline friend vec2 operator *(const vec2<T>& v1, const vec2<T>& v2) {
        return { v1.x * v2.x, v1.y * v2.y };
    }

    inline friend vec2 operator /(const vec2<T>& v1, const vec2<T>& v2) {
        return { v1.x / v2.x, v1.y / v2.y };
    }

    inline friend vec2 operator +(const vec2<T>& v1, const T& s) {
        return { v1.x + s, v1.y + s };
    }

    inline friend vec2 operator -(const vec2<T>& v1, const T& s) {
        return { v1.x - s, v1.y - s };
    }

    inline friend vec2 operator *(const vec2<T>& v1, const T& s) {
        return { v1.x * s, v1.y * s };
    }

    inline friend vec2 operator /(const vec2<T>& v1, const T& s) {
        return { v1.x / s, v1.y / s };
    }

    inline friend vec2 operator +(const T& s, const vec2<T>& v2) {
        return { s + v2.x, s + v2.y };
    }

    inline friend vec2 operator -(const T& s, const vec2<T>& v2) {
        return { s - v2.x, s - v2.y };
    }

    inline friend vec2 operator *(const T& s, const vec2<T>& v2) {
        return { s * v2.x, s * v2.y };
    }

    inline friend vec2 operator /(const T& s, const vec2<T>& v2) {
        return { s / v2.x, s / v2.y };
    }

    inline vec2 operator ^(const T& p) {
        return { pow(x, p), pow(y, p) };
    }
};

template<typename T>
struct vec3 final {
    T x, y, z;

    inline T& operator [](int i) {
        return *(&x + i);
    }

    inline T length() {
        return sqrt(x * x + y * y + z * z);
    }

    inline void normalize() {
        T l = length();
        x /= l;
        y /= l;
        z /= l;
    }

    inline friend vec3 operator +(const vec3<T>& v1, const vec3<T>& v2) {
        return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
    }

    inline friend vec3 operator -(const vec3<T>& v1, const vec3<T>& v2) {
        return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
    }

    inline friend vec3 operator *(const vec3<T>& v1, const vec3<T>& v2) {
        return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
    }

    inline friend vec3 operator /(const vec3<T>& v1, const vec3<T>& v2) {
        return { v1.x / v2.x, v1.y / v2.y, v1.z / v2.z };
    }

    inline friend vec3 operator +(const vec3<T>& v1, const T& s) {
        return { v1.x + s, v1.y + s, v1.z + s };
    }

    inline friend vec3 operator -(const vec3<T>& v1, const T& s) {
        return { v1.x - s, v1.y - s, v1.z - s };
    }

    inline friend vec3 operator *(const vec3<T>& v1, const T& s) {
        return { v1.x * s, v1.y * s, v1.z * s };
    }

    inline friend vec3 operator /(const vec3<T>& v1, const T& s) {
        return { v1.x / s, v1.y / s, v1.z / s };
    }

    inline friend vec3 operator +(const T& s, const vec3<T>& v2) {
        return { s + v2.x, s + v2.y, s + v2.z };
    }

    inline friend vec3 operator -(const T& s, const vec3<T>& v2) {
        return { s - v2.x, s - v2.y, s - v2.z };
    }

    inline friend vec3 operator *(const T& s, const vec3<T>& v2) {
        return { s * v2.x, s * v2.y, s * v2.z };
    }

    inline friend vec3 operator /(const T& s, const vec3<T>& v2) {
        return { s / v2.x, s / v2.y, s / v2.z };
    }

    inline vec3 operator ^(const T& p) {
        return { pow(x, p), pow(y, p), pow(z, p) };
    }
};

template<typename T>
struct vec4 {
    T x, y, z, w;

    inline T& operator [](int i) {
        return *(&x + i);
    }

    inline T length() {
        return sqrt(x * x + y * y + z * z + w * w);
    }

    inline void normalize() {
        T l = length();
        x /= l;
        y /= l;
        z /= l;
        w /= l;
    }

    inline friend vec4 operator +(const vec4<T>& v1, const vec4<T>& v2) {
        return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w };
    }

    inline friend vec4 operator -(const vec4<T>& v1, const vec4<T>& v2) {
        return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w };
    }

    inline friend vec4 operator *(const vec4<T>& v1, const vec4<T>& v2) {
        return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w };
    }

    inline friend vec4 operator /(const vec4<T>& v1, const vec4<T>& v2) {
        return { v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w };
    }

    inline friend vec4 operator +(const vec4<T>& v1, const T& s) {
        return { v1.x + s, v1.y + s, v1.z + s, v1.w + s };
    }

    inline friend vec4 operator -(const vec4<T>& v1, const T& s) {
        return { v1.x - s, v1.y - s, v1.z - s, v1.w - s };
    }

    inline friend vec4 operator *(const vec4<T>& v1, const T& s) {
        return { v1.x * s, v1.y * s, v1.z * s, v1.w * s };
    }

    inline friend vec4 operator /(const vec4<T>& v1, const T& s) {
        return { v1.x / s, v1.y / s, v1.z / s, v1.w / s };
    }

    inline friend vec4 operator +(const T& s, const vec4<T>& v2) {
        return { s + v2.x, s + v2.y, s + v2.z, s + v2.w };
    }

    inline friend vec4 operator -(const T& s, const vec4<T>& v2) {
        return { s - v2.x, s - v2.y, s - v2.z, s - v2.w };
    }

    inline friend vec4 operator *(const T& s, const vec4<T>& v2) {
        return { s * v2.x, s * v2.y, s * v2.z, s * v2.w };
    }

    inline friend vec4 operator /(const T& s, const vec4<T>& v2) {
        return { s / v2.x, s / v2.y, s / v2.z, s / v2.w };
    }

    inline vec4 operator ^(const T& p) {
        return { pow(x, p), pow(y, p), pow(z, p), pow(w, p) };
    }
};

struct quat : vec4<float> {

    quat() = default;

    quat(const vec3<float>& v, float angle, const vec3<float>& axis) {
        quat q,p,qinv;

        q.w=cosf(0.5f*angle);
        q.x=sinf(0.5f*angle)*axis.x;
        q.y=sinf(0.5f*angle)*axis.y;
        q.z=sinf(0.5f*angle)*axis.z;

        p.x=v.x;
        p.y=v.y;
        p.z=v.z;
        p.w=0;

        qinv=q;
        qinv.inverse();

        *this = (q*p)*qinv;
    }

    inline void conjugate() {
        x = -x;
        y = -y;
        z = -z;
    }

    inline void inverse() {
        float l = length();
        conjugate();
        x /= l;
        y /= l;
        z /= l;
        w /= l;
    }

    inline friend quat operator *(const quat& q1, const quat& q2) {
        quat q3;
        q3.x = ((q1.w * q2.x) + (q1.x * q2.w) + (q1.y * q2.z) - (q1.z * q2.y));
        q3.y = ((q1.w * q2.y) - (q1.x * q2.z) + (q1.y * q2.w) + (q1.z * q2.x));
        q3.z = ((q1.w * q2.z) + (q1.x * q2.y) - (q1.y * q2.x) + (q1.z * q2.w));
        q3.w = ((q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z));
        return q3;
    }

};

template<typename T>
struct mat2 final {
    vec2<T> m[2];

    inline vec2<T>& operator [](int i) {
        return m[i];
    }
};

template<typename T>
struct mat3 final {
    vec3<T> m[3];

    inline vec3<T>& operator [](int i) {
        return m[i];
    }
};

template<typename T>
struct mat4 final {
    vec4<T> m[4];

    mat4(const quat& q) {
        float wx, wy, wz, xx, yy, yz, xy, xz, zz;

        xx = q.x * q.x;
        xy = q.x * q.y;
        xz = q.x * q.z;
        yy = q.y * q.y;
        zz = q.z * q.z;
        yz = q.y * q.z;

        wx = q.w * q.x;
        wy = q.w * q.y;
        wz = q.w * q.z;

        float* mm = &m[0][0];

        mm[0] = 1.0f - 2.0f * (yy + zz);
        mm[4] = 2.0f * (xy - wz);
        mm[8] = 2.0f * (xz + wy);
        mm[12] = 0.0;

        mm[1] = 2.0f * (xy + wz);
        mm[5] = 1.0f - 2.0f * (xx + zz);
        mm[9] = 2.0f * (yz - wx);
        mm[13] = 0.0;

        mm[2] = 2.0f * (xz - wy);
        mm[6] = 2.0f * (yz + wx);
        mm[10] = 1.0f - 2.0f * (xx + yy);
        mm[14] = 0.0;

        mm[3] = 0;
        mm[7] = 0;
        mm[11] = 0;
        mm[15] = 1;
    }

    inline vec4<T>& operator [](int i) {
        return m[i];
    }
};

typedef vec2<float> float2;
typedef vec3<float> float3;
typedef vec4<float> float4;
typedef mat2<float> float2x2;
typedef mat3<float> float3x3;
typedef mat4<float> float4x4;

typedef vec2<int> int2;
typedef vec3<int> int3;
typedef vec4<int> int4;
typedef mat2<int> int2x2;
typedef mat3<int> int3x3;
typedef mat4<int> int4x4;

typedef vec2<double> double2;
typedef vec3<double> double3;
typedef vec4<double> double4;
typedef mat2<double> double2x2;
typedef mat3<double> double3x3;
typedef mat4<double> double4x4;

template<typename T>
inline T clamp(const T& a, const T& b, const T& x) {
    return min(max(x, b), a);
}

template<typename T>
inline T lerp(const T& a, const T& b, const T& x) {
    return x * (b - a) + a;
}

inline quat slerp(const quat& q1, const quat& q2, float t) {
    quat q3;
    float dot = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
    float theta, st, sut, sout, coeff1, coeff2;

    t /= 2.0;

    theta = (float) acos(dot);
    if (theta < 0.0) {
        theta=-theta;
    }

    st = (float) sin(theta);
    sut = (float) sin(t*theta);
    sout = (float) sin((1-t)*theta);
    coeff1 = sout/st;
    coeff2 = sut/st;

    q3.x = coeff1*q1.x + coeff2*q2.x;
    q3.y = coeff1*q1.y + coeff2*q2.y;
    q3.z = coeff1*q1.z + coeff2*q2.z;
    q3.w = coeff1*q1.w + coeff2*q2.w;

    q3.normalize();

    return q3;
}

template<typename T>
inline T step(const T& a, const T& x) {
    return clamp(0, 1, x - a);
}

template<typename T>
inline T smoothstep(const T& a, const T& b, const T& x) {
    T t = clamp(0, 1, (x - a) / (b - a));
    T y = -2 * t * t * t + 3 * t * t;
    return y;
}

template<typename T>
inline T dot(const vec2<T>& v1, const vec2<T>& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

template<typename T>
inline T dot(const vec3<T>& v1, const vec3<T>& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template<typename T>
inline T dot(const vec4<T>& v1, const vec4<T>& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

template<typename T>
inline T cross(const vec2<T>& v1, const vec2<T>& v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

template<typename T>
inline vec3<T> cross(const vec3<T>& v1, const vec3<T>& v2) {
    return {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x,
    };
}

// TODO(cheerwizard): do I really need a wedged/geometry products?
template<typename T>
inline vec4<T> cross(const vec4<T>& v1, const vec4<T>& v2) {
    // TODO(cheerwizard): not implemented!
    return {};
}

inline constexpr double radians(double v) {
    return v * (M_PI / 180.0);
}

inline double randomd() {
    return rand() / (double) RAND_MAX;
}

inline float randomf() {
    return rand() / (float) RAND_MAX;
}