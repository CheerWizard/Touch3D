#pragma once

#include <Types.hpp>

#include <cmath>
#include <random>

#define T3D_PI 3.14159265359f
#define T3D_RADIANS(degree) (degree * T3D_PI / 180.0f)

struct degree final {
    float a;

    explicit degree(float a) : a(a) {}

    operator float() const() { return a; }
};

struct radians final {
    float a;

    explicit radians(float a) : a(a) {}
    explicit radians(const degree& d) : a(T3D_RADIANS(d)) {}

    operator float() const() { return a; }
};

template<typename T>
struct vec2 {
    T x = 0;
    T y = 0;

    T& operator [](int i) {
        return *(&x + i);
    }

    friend vec2 operator +(const vec2& v1, const vec2& v2) {
        return { v1.x + v2.x, v1.y + v2.y };
    }

    friend vec2 operator -(const vec2& v1, const vec2& v2) {
        return { v1.x - v2.x, v1.y - v2.y };
    }

    friend vec2 operator *(const vec2& v1, const vec2& v2) {
        return { v1.x * v2.x, v1.y * v2.y };
    }

    friend vec2 operator /(const vec2& v1, const vec2& v2) {
        return { v1.x / v2.x, v1.y / v2.y };
    }

    friend vec2 operator +(const vec2& v1, const T& s) {
        return { v1.x + s, v1.y + s };
    }

    friend vec2 operator -(const vec2& v1, const T& s) {
        return { v1.x - s, v1.y - s };
    }

    friend vec2 operator *(const vec2& v1, const T& s) {
        return { v1.x * s, v1.y * s };
    }

    friend vec2 operator /(const vec2& v1, const T& s) {
        return { v1.x / s, v1.y / s };
    }

    friend vec2 operator +(const T& s, const vec2& v2) {
        return { s + v2.x, s + v2.y };
    }

    friend vec2 operator -(const T& s, const vec2& v2) {
        return { s - v2.x, s - v2.y };
    }

    friend vec2 operator *(const T& s, const vec2& v2) {
        return { s * v2.x, s * v2.y };
    }

    friend vec2 operator /(const T& s, const vec2& v2) {
        return { s / v2.x, s / v2.y };
    }

    friend vec2 operator ^(const vec2& v, const T& p) {
        return { pow(v.x, p), pow(v.y, p) };
    }

    friend vec2 operator -(const vec2& v) {
        return { -v.x, -v.y };
    }
};

template<typename T>
struct vec3 {
    T x = 0;
    T y = 0;
    T z = 0;

    T& operator [](int i) {
        return *(&x + i);
    }

    friend vec3 operator +(const vec3& v1, const vec3& v2) {
        return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
    }

    friend vec3 operator -(const vec3& v1, const vec3& v2) {
        return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
    }

    friend vec3 operator *(const vec3& v1, const vec3& v2) {
        return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
    }

    friend vec3 operator /(const vec3& v1, const vec3& v2) {
        return { v1.x / v2.x, v1.y / v2.y, v1.z / v2.z };
    }

    friend vec3 operator +(const vec3& v1, const T& s) {
        return { v1.x + s, v1.y + s, v1.z + s };
    }

    friend vec3 operator -(const vec3& v1, const T& s) {
        return { v1.x - s, v1.y - s, v1.z - s };
    }

    friend vec3 operator *(const vec3& v1, const T& s) {
        return { v1.x * s, v1.y * s, v1.z * s };
    }

    friend vec3 operator /(const vec3& v1, const T& s) {
        return { v1.x / s, v1.y / s, v1.z / s };
    }

    friend vec3 operator +(const T& s, const vec3& v2) {
        return { s + v2.x, s + v2.y, s + v2.z };
    }

    friend vec3 operator -(const T& s, const vec3& v2) {
        return { s - v2.x, s - v2.y, s - v2.z };
    }

    friend vec3 operator *(const T& s, const vec3& v2) {
        return { s * v2.x, s * v2.y, s * v2.z };
    }

    friend vec3 operator /(const T& s, const vec3& v2) {
        return { s / v2.x, s / v2.y, s / v2.z };
    }

    friend vec3 operator ^(const vec3& v, const T& p) {
        return { pow(v.x, p), pow(v.y, p), pow(v.z, p) };
    }

    friend vec3 operator -(const vec3& v) {
        return { -v.x, -v.y, -v.z };
    }

    vec2<T> xy() {
        return { x, y };
    }
};

template<typename T>
struct vec4 {
    T x = 0;
    T y = 0;
    T z = 0;
    T w = 0;

    T& operator [](int i) {
        return *(&x + i);
    }

    friend vec4 operator +(const vec4& v1, const vec4& v2) {
        return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w };
    }

    friend vec4 operator -(const vec4 v1, const vec4& v2) {
        return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w };
    }

    friend vec4 operator *(const vec4& v1, const vec4& v2) {
        return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w };
    }

    friend vec4 operator /(const vec4& v1, const vec4& v2) {
        return { v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w };
    }

    friend vec4 operator +(const vec4& v1, const T& s) {
        return { v1.x + s, v1.y + s, v1.z + s, v1.w + s };
    }

    friend vec4 operator -(const vec4& v1, const T& s) {
        return { v1.x - s, v1.y - s, v1.z - s, v1.w - s };
    }

    friend vec4 operator *(const vec4& v1, const T& s) {
        return { v1.x * s, v1.y * s, v1.z * s, v1.w * s };
    }

    friend vec4 operator /(const vec4& v1, const T& s) {
        return { v1.x / s, v1.y / s, v1.z / s, v1.w / s };
    }

    friend vec4 operator +(const T& s, const vec4& v2) {
        return { s + v2.x, s + v2.y, s + v2.z, s + v2.w };
    }

    friend vec4 operator -(const T& s, const vec4& v2) {
        return { s - v2.x, s - v2.y, s - v2.z, s - v2.w };
    }

    friend vec4 operator *(const T& s, const vec4& v2) {
        return { s * v2.x, s * v2.y, s * v2.z, s * v2.w };
    }

    friend vec4 operator /(const T& s, const vec4& v2) {
        return { s / v2.x, s / v2.y, s / v2.z, s / v2.w };
    }

    friend vec4 operator ^(const vec4& v, const T& p) {
        return { pow(v.x, p), pow(v.y, p), pow(v.z, p), pow(v.w, p) };
    }

    friend vec4 operator -(const vec4& v) {
        return { -v.x, -v.y, -v.z, -v.w };
    }

    vec2<T> xy() {
        return { x, y };
    }

    vec3<T> xyz() {
        return { x, y, z };
    }
};

struct quat {
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 1;

    quat() = default;

    quat(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    quat(float nx, float ny, float nz, const radians& r = radians(0)) {
        x = nx * sinf(r * 0.5f);
        y = ny * sinf(r * 0.5f);
        z = nz * sinf(r * 0.5f);
        w = cosf(r * 0.5f);
    }

    quat(const vec3<float>& n, const radians& r = radians(0)) : quat(n.x, n.y, n.z , r) {}

    friend quat operator *(const quat& q1, const quat& q2) {
        quat q3;
        q3.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
        q3.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
        q3.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
        q3.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
        return q3;
    }

    friend quat operator-(const quat& q) {
        return { -q.x, -q.y, -q.z, q.w };
    }
};

enum MAT_ORDER {
    MAT_ORDER_ROW,
    MAT_ORDER_COLUMN
};

template<typename T, MAT_ORDER order = MAT_ORDER_ROW>
struct mat2 {
    vec2<T> m[2] = {
        { 1, 0 },
        { 0, 1 },
    };

    mat2(const vec2<T>& v0 = {}, const vec2<T>& v1 = {}) {
        m[0] = v0;
        m[1] = v1;
        if constexpr (order == MAT_ORDER_COLUMN) {
            mat2_transpose(*this);
        }
    }

    T* operator [](int i) {
        return m[i];
    }

    friend mat2 operator *(const mat2& m1, const mat2& m2) {
        mat2 m3;
        for (int r = 0 ; r < 2 ; r++) {
            for (int c = 0 ; c < 2 ; c++) {
                for (int i = 0 ; i < 2 ; i++) {
                    m3[r][c] += m1[r][i] * m2[i][c];
                }
            }
        }
        return m3;
    }

    mat2 operator -() {
        mat2 n;
        for (int r = 0 ; r < 2 ; r++) {
            for (int c = 0 ; c < 2 ; c++) {
                n[r][c] = -m[r][c];
            }
        }
        return n;
    }
};

template<typename T, MAT_ORDER order = MAT_ORDER_ROW>
struct mat3 {
    vec3<T> m[3] = {
        { 1, 0, 0 },
        { 0, 1, 0 },
        { 0, 0, 1 },
    };

    mat3(const vec3<T>& v0 = {}, const vec3<T>& v1 = {}, const vec3<T>& v2 = {}) {
        m[0] = v0;
        m[1] = v1;
        m[2] = v2;
        if constexpr (order == MAT_ORDER_COLUMN) {
            mat3_transpose(*this);
        }
    }

    T* operator [](int i) {
        return m[i];
    }

    friend mat3 operator *(const mat3& m1, const mat3& m2) {
        mat3 m3;
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 3; c++) {
                for (int i = 0; i < 3; i++) {
                    m3[r][c] += m1[r][i] * m2[i][c];
                }
            }
        }
        return m3;
    }

    mat3 operator -() {
        mat3 n;
        for (int r = 0 ; r < 3 ; r++) {
            for (int c = 0 ; c < 3 ; c++) {
                n[r][c] = -m[r][c];
            }
        }
        return n;
    }
};

template<typename T, MAT_ORDER order = MAT_ORDER_ROW>
struct mat4 {
    vec4<T> m[4] = {
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 },
    };

    mat4() = default;

    mat4(const vec4<T>& v0 = {}, const vec4<T>& v1 = {}, const vec4<T>& v2 = {}, const vec4<T>& v3 = {}) {
        m[0] = v0;
        m[1] = v1;
        m[2] = v2;
        m[3] = v3;
        if constexpr (order == MAT_ORDER_COLUMN) {
            mat4_transpose(*this);
        }
    }

    mat4(const vec3<T>& v0 = {}, const vec3<T>& v1 = {}, const vec3<T>& v2 = {}, const vec3<T>& v3 = {}) {
        m[0] = v0;
        m[1] = v1;
        m[2] = v2;
        m[3] = v3;
        if constexpr (order == MAT_ORDER_COLUMN) {
            mat4_transpose(*this);
        }
    }

    mat4(const quat<T>& q) {
        T xx = q.x * q.x;
        T xy = q.x * q.y;
        T xz = q.x * q.z;
        T yy = q.y * q.y;
        T zz = q.z * q.z;
        T yz = q.y * q.z;
        T wx = q.w * q.x;
        T wy = q.w * q.y;
        T wz = q.w * q.z;

        m[0][0] = 1.0f - 2.0f * (yy + zz);
        m[1][0] = 2.0f * (xy - wz);
        m[2][0] = 2.0f * (xz + wy);
        m[3][0] = 0.0;

        m[0][1] = 2.0f * (xy + wz);
        m[1][1] = 1.0f - 2.0f * (xx + zz);
        m[2][1] = 2.0f * (yz - wx);
        m[3][1] = 0.0;

        m[0][2] = 2.0f * (xz - wy);
        m[1][2] = 2.0f * (yz + wx);
        m[2][2] = 1.0f - 2.0f * (xx + yy);
        m[3][2] = 0.0;

        m[0][3] = 0;
        m[1][3] = 0;
        m[2][3] = 0;
        m[3][3] = 1;
    }

    T* operator [](int i) {
        return m[i];
    }

    friend mat4 operator *(const mat4& m1, const mat4& m2) {
        mat4 m3;
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                for (int i = 0; i < 4; i++) {
                    m3[r][c] += m1[r][i] * m2[i][c];
                }
            }
        }
        return m3;
    }

    mat4 operator -() {
        mat4 n;
        for (int r = 0 ; r < 4 ; r++) {
            for (int c = 0 ; c < 4 ; c++) {
                n[r][c] = -m[r][c];
            }
        }
        return n;
    }
};

typedef vec2<float> float2;
typedef vec3<float> float3;
typedef vec4<float> float4;
typedef vec2<degree> degree2;
typedef vec3<degree> degree3;
typedef vec4<degree> degree4;
typedef vec2<radians> radians2;
typedef vec3<radians> radians3;
typedef vec4<radians> radians4;
using float2x2 = mat2<float>;
using float3x3 = mat3<float>;
using float4x4 = mat4<float>;
using float2x2c = mat2<float, MAT_ORDER_COLUMN>;
using float3x3c = mat3<float, MAT_ORDER_COLUMN>;
using float4x4c = mat4<float, MAT_ORDER_COLUMN>;

typedef vec2<int> int2;
typedef vec3<int> int3;
typedef vec4<int> int4;
using int2x2 = mat2<int>;
using int3x3 = mat3<int>;
using int4x4 = mat4<int>;
using int2x2c = mat2<int, MAT_ORDER_COLUMN>;
using int3x3c = mat3<int, MAT_ORDER_COLUMN>;
using int4x4c = mat4<int, MAT_ORDER_COLUMN>;

typedef vec2<double> double2;
typedef vec3<double> double3;
typedef vec4<double> double4;
using double2x2 = mat2<double>;
using double3x3 = mat3<double>;
using double4x4 = mat4<double>;
using double2x2c = mat2<double, MAT_ORDER_COLUMN>;
using double3x3c = mat3<double, MAT_ORDER_COLUMN>;
using double4x4c = mat4<double, MAT_ORDER_COLUMN>;

template<typename T>
T min(const T& min1, const T& min2) {
    return std::min(min1, min2);
}

template<typename T>
T max(const T& max1, const T& max2) {
    return std::min(max1, max2);
}

template<typename T>
T clamp(const T& a, const T& b, const T& x) {
    return min(max(x, b), a);
}

template<typename T>
T lerp(const T& a, const T& b, const T& x) {
    return x * (b - a) + a;
}

template<typename T>
T step(const T& a, const T& x) {
    return clamp(0, 1, x - a);
}

template<typename T>
T smoothstep(const T& a, const T& b, const T& x) {
    T t = clamp(0, 1, (x - a) / (b - a));
    T y = -2 * t * t * t + 3 * t * t;
    return y;
}

template<typename T>
T length(const vec2<T>& v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

template<typename T>
T length(const vec3<T>& v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

template<typename T>
T length(const vec4<T>& v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

template<typename T>
vec2<T> normalize(const vec2<T>& v) {
    T l = length(v);
    return { v.x / l, v.y / l };
}

template<typename T>
vec3<T> normalize(const vec3<T>& v) {
    T l = length(v);
    return { v.x / l, v.y / l, v.z / l };
}

template<typename T>
vec4<T> normalize(const vec4<T>& v) {
    T l = length(v);
    return { v.x / l, v.y / l, v.z / l, v.w / l };
}

template<typename T>
T dot(const vec2<T>& v1, const vec2<T>& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

template<typename T>
T dot(const vec3<T>& v1, const vec3<T>& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template<typename T>
T dot(const vec4<T>& v1, const vec4<T>& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

template<typename T>
T cross(const vec2<T>& v1, const vec2<T>& v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

template<typename T>
vec3<T> cross(const vec3<T>& v1, const vec3<T>& v2) {
    return {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x,
    };
}

// TODO(cheerwizard): do I really need a wedged/geometry products?
template<typename T>
vec4<T> cross(const vec4<T>& v1, const vec4<T>& v2) {
    // TODO(cheerwizard): not implemented!
    return {};
}

inline float length(const quat& q) {
    return sqrtf(q.x * q.x + q.y * q.y + q.z * q.z);
}

inline quat normalize(const quat& q) {
    float l = length(q);
    return { q.x / l, q.y / l, q.z / l, q.w };
}

inline quat rotate(const float3& n, const quat& q) {
    return q * quat(n) * -q;
}

inline quat slerp(const quat& q1, const quat& q2, float t) {
    quat q3;
    float dot = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
    float theta, st, sut, sout, coeff1, coeff2;

    t /= 2.0;

    theta = acos(dot);
    if (theta < 0.0) {
        theta=-theta;
    }

    st = sin(theta);
    sut = sin(t*theta);
    sout = sin((1-t)*theta);
    coeff1 = sout/st;
    coeff2 = sut/st;

    q3.x = coeff1*q1.x + coeff2*q2.x;
    q3.y = coeff1*q1.y + coeff2*q2.y;
    q3.z = coeff1*q1.z + coeff2*q2.z;
    q3.w = coeff1*q1.w + coeff2*q2.w;

    normalize(q3);

    return q3;
}

template<typename T>
mat2<T> mat2_transpose(const mat2<T>& m) {
    mat2<T> t = m;

    // swap(t[0][0], t[0][0]);
    swap(t[0][1], t[1][0]);

    // swap(t[1][0], t[0][1]);
    // swap(t[1][1], t[1][1]);

    return t;
}

template<typename T>
mat3<T> mat3_transpose(const mat3<T>& m) {
    mat3<T> t = m;

    // swap(t[0][0], t[0][0]);
    swap(t[0][1], t[1][0]);
    swap(t[0][2], t[2][0]);

    // swap(t[1][0], t[0][1]);
    // swap(t[1][1], t[1][1]);
    swap(t[1][2], t[2][1]);

    // swap(t[2][0], t[0][2]);
    // swap(t[2][1], t[1][2]);
    // swap(t[2][2], t[2][2]);

    return t;
}

template<typename T>
mat4<T> mat4_transpose(const mat4<T>& m) {
    mat4<T> t = m;

    // swap(t[0][0], t[0][0]);
    swap(t[0][1], t[1][0]);
    swap(t[0][2], t[2][0]);
    swap(t[0][3], t[3][0]);

    // swap(t[1][0], t[0][1]);
    // swap(t[1][1], t[1][1]);
    swap(t[1][2], t[2][1]);
    swap(t[1][3], t[3][1]);

    // swap(t[2][0], t[0][2]);
    // swap(t[2][1], t[1][2]);
    // swap(t[2][2], t[2][2]);
    swap(t[2][3], t[3][2]);

    // swap(t[3][0], t[0][3]);
    // swap(t[3][1], t[1][3]);
    // swap(t[3][2], t[2][3]);
    // swap(t[3][3], t[3][3]);

    return t;
}

template<typename T>
T mat2_det(const mat2<T>& m) {
    T d = m[0][0] * m[1][1] - m[1][0] * m[0][1];
    return d;
}

template<typename T>
T mat3_det(const mat3<T>& m) {
    T d = m[0][0] * m[1][1] * m[2][2]
        + m[0][1] * m[1][2] * m[2][0]
        + m[0][2] * m[1][0] * m[2][1]
        - m[2][0] * m[1][1] * m[0][2]
        - m[2][1] * m[1][2] * m[0][0]
        - m[2][2] * m[1][0] * m[0][1];
    return d;
}

template<typename T>
T mat4_det(const mat4<T>& m) {
    T d = m[0][0] * m[1][1] * m[2][2] * m[3][3]
        + m[0][1] * m[1][2] * m[2][3] * m[3][0]
        + m[0][2] * m[1][3] * m[2][0] * m[3][1]
        - m[3][0] * m[2][1] * m[1][2] * m[0][3]
        - m[3][1] * m[2][2] * m[1][3] * m[0][0]
        - m[3][2] * m[2][3] * m[1][0] * m[0][1];
    return d;
}

template<typename T, MAT_ORDER O>
mat2<T> mat2_inverse(const mat2<T, O>& m) {
    mat2<T> c;
    T d = mat2_det(m);
    c[0][0] = m[1][1];
    c[0][1] =-m[1][0];
    c[1][0] =-m[0][1];
    c[1][1] = m[0][0];
    c /= d;
    return c;
}

template<typename T, MAT_ORDER O>
mat3<T> mat3_inverse(const mat3<T, O>& m) {
    T d = mat3_det(m);
    mat3<T> c;

    c[0][0] = mat2_det(mat2<T> {
        m[1][1], m[1][2],
        m[2][1], m[2][2]
    });

    c[0][1] = -mat2_det(mat2<T> {
        m[1][0], m[1][2],
        m[2][0], m[2][2]
    });

    c[0][2] = mat2_det(mat2<T> {
        m[1][0], m[1][1],
        m[2][0], m[2][1]
    });

    c[1][0] = -mat2_det(mat2<T> {
        m[0][0], m[0][1],
        m[2][0], m[2][1]
    });

    c[1][1] = mat2_det(mat2<T> {
        m[0][0], m[0][2],
        m[2][0], m[2][2]
    });

    c[1][2] = -mat2_det(mat2<T> {
        m[0][0], m[0][1],
        m[2][0], m[2][1]
    });

    c[2][0] = mat2_det(mat2<T> {
        m[0][1], m[0][2],
        m[1][1], m[1][2]
    });

    c[2][1] = -mat2_det(mat2<T> {
        m[0][0], m[0][2],
        m[1][0], m[1][2]
    });

    c[2][2] = mat2_det(mat2<T> {
        m[0][0], m[0][1],
        m[1][0], m[1][1]
    });

    c /= d;

    return c;
}

template<typename T, MAT_ORDER O>
mat4<T> mat4_inverse(const mat4<T, O>& m) {
    T d = mat4_det(m);
    mat4<T> c;

    c[0][0] = mat3_det(mat3<T> {
        m[1][1], m[1][2], m[1][3],
        m[2][1], m[2][2], m[2][3],
        m[3][1], m[3][2], m[3][3]
    });

    c[0][1] = -mat3_det(mat3<T> {
        m[1][0], m[1][2], m[1][3],
        m[2][0], m[2][2], m[2][3],
        m[3][0], m[3][2], m[3][3]
    });

    c[0][2] = mat3_det(mat3<T> {
        m[1][0], m[1][1], m[1][3],
        m[2][0], m[2][1], m[2][3],
        m[3][0], m[3][1], m[3][3]
    });

    c[0][3] = -mat3_det(mat3<T> {
        m[1][0], m[1][1], m[1][2],
        m[2][0], m[2][1], m[2][2],
        m[3][0], m[3][1], m[3][2]
    });

    c[1][0] = -mat3_det(mat3<T> {
        m[0][1], m[0][2], m[0][3],
        m[2][1], m[2][2], m[2][3],
        m[3][1], m[3][2], m[3][3]
    });

    c[1][1] = mat3_det(mat3<T> {
        m[0][0], m[0][2], m[0][3],
        m[2][0], m[2][2], m[2][3],
        m[3][0], m[3][2], m[3][3]
    });

    c[1][2] = -mat3_det(mat3<T> {
        m[0][0], m[0][1], m[0][3],
        m[2][0], m[2][1], m[2][3],
        m[3][0], m[3][1], m[3][3]
    });

    c[1][3] = mat3_det(mat3<T> {
        m[0][0], m[0][1], m[0][2],
        m[2][0], m[2][1], m[2][2],
        m[3][0], m[3][1], m[3][2]
    });

    c[2][0] = mat3_det(mat3<T> {
        m[0][1], m[0][2], m[0][3],
        m[1][1], m[1][2], m[1][3],
        m[3][1], m[3][2], m[3][3]
    });

    c[2][1] = -mat3_det(mat3<T> {
        m[0][0], m[0][2], m[0][3],
        m[1][0], m[1][2], m[1][3],
        m[3][0], m[3][2], m[3][3]
    });

    c[2][2] = mat3_det(mat3<T> {
        m[0][0], m[0][1], m[0][3],
        m[1][0], m[1][1], m[1][3],
        m[3][0], m[3][1], m[3][3]
    });

    c[2][3] = -mat3_det(mat3<T> {
        m[0][0], m[0][1], m[0][2],
        m[1][0], m[1][1], m[1][2],
        m[3][0], m[3][1], m[3][2]
    });

    c[3][0] = -mat3_det(mat3<T> {
        m[0][1], m[0][2], m[0][3],
        m[1][1], m[1][2], m[1][3],
        m[2][1], m[2][2], m[2][3]
    });

    c[3][1] = mat3_det(mat3<T> {
        m[0][0], m[0][2], m[0][3],
        m[1][0], m[1][2], m[1][3],
        m[2][0], m[2][2], m[2][3]
    });

    c[3][2] = -mat3_det(mat3<T> {
        m[0][0], m[0][1], m[0][3],
        m[1][0], m[1][1], m[1][3],
        m[2][0], m[2][1], m[2][3]
    });

    c[3][3] = mat3_det(mat3<T> {
        m[0][0], m[0][1], m[0][2],
        m[1][0], m[1][1], m[1][2],
        m[2][0], m[2][1], m[2][2]
    });

    c /= d;

    return c;
}

// NOTE: Fast inverse is only possible with model matrix!
template<typename T, MAT_ORDER M>
mat4<T> mat4_inverse_fast(const mat4<T, M>& m) {
    // TODO(cheerwizard): not implemented!
    return {};
}

inline float4x4 mat4_translate(const float3& translation) {
    float4x4 m;
    m[0][3] = translation.x;
    m[1][3] = translation.y;
    m[2][3] = translation.z;
    return m;
}

inline float4x4 mat4_scale(const float3& scalar) {
    float4x4 m;
    m[0][0] = scalar.x;
    m[1][1] = scalar.y;
    m[2][2] = scalar.z;
    return m;
}

inline float4x4 mat4_rotate(const radians3& rotation, const float3& axis) {
    float4x4 rx;
    {
        float sin = sin(rotation.x);
        float cos = cos(rotation.x);
        rx[1][1] = cos;
        rx[1][2] = -sin;
        rx[2][1] = sin;
        rx[2][2] = cos;
        rx[0][0] = axis.x;
    }

    float4x4 ry;
    {
        float sin = sin(rotation.y);
        float cos = cos(rotation.y);
        ry[0][0] = cos;
        ry[0][2] = sin;
        ry[2][0] = -sin;
        ry[2][2] = cos;
        ry[1][1] = axis.y;
    }

    float4x4 rz;
    {
        float sin = sin(rotation.z);
        float cos = cos(rotation.z);
        rz[0][0] = cos;
        rz[0][1] = -sin;
        rz[1][0] = sin;
        rz[1][1] = cos;
        rz[2][2] = axis.z;
    }

    return rz * ry * rx;
}

inline float4x4 mat4_model(const float3& translation, const radians3& rotation, const float3& scalar) {
    float4x4 m;
    m = mat4_translate(translation);
    m = m * mat4_rotate(rotation, { 1, 1, 1 });
    m = m * mat4_scale(scalar);
    return m;
}

inline float4x4 mat4_model(const float3& translation, const quat& rotation, const float3& scalar) {
    float4x4 m;
    m = mat4_translate(translation);
    m = m * float4x4(rotation);
    m = m * mat4_scale(scalar);
    return m;
}

inline float4x4 mat4_rigid(const float3& translation, const radians3& rotation) {
    float4x4 m;
    m = mat4_translate(translation);
    m = m * mat4_rotate(rotation, { 1, 1, 1 });
    return m;
}

inline float4x4 mat4_rigid(const float3& translation, const quat& rotation) {
    float4x4 m;
    m = mat4_translate(translation);
    m = m * float4x4(rotation);
    return m;
}

inline float4x4 mat4_view(const float3& position, const float3& front, const float3& up) {
    float3 right = normalize(cross(front, up));
    return mat4_inverse_fast(float4x4c { right, cross(right, front), -front, position });
}

inline float4x4 mat4_ortho(float left, float right, float bottom, float top, float z_near, float z_far) {
    return float4x4 {
        { 2.0f / (right - left), 0.0f, 0.0f, 0.0f },
        { 0.0f, 2.0f / (bottom - top), 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f / (z_near - z_far), 0.0f },
        { -(right + left) / (right - left), -(bottom + top) / (bottom - top), z_near / (z_near - z_far), 1.0f }
    };
}

inline float4x4 mat4_perspective(float aspect, degree fov, float z_near, float z_far) {
    float f = 1.0f / tan(radians(0.5f * fov));
    return float4x4 {
            { f / aspect, 0.0f, 0.0f, 0.0f },
            { 0.0f, -f, 0.0f, 0.0f },
            { 0.0f, 0.0f, z_far / (z_near - z_far), -1.0f },
            { 0.0f, 0.0f, z_near * z_far / (z_near - z_far), 0.0f }
    };
}

inline float4x4 mat4_normal(const float4x4& model) {
    return mat4_transpose(mat4_inverse_fast(model));
}