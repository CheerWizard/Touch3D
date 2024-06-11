#pragma once

#include <Types.hpp>

#include <cmath>
#include <random>
#include <type_traits>

#define T3D_PI 3.14159265359f
#define T3D_RADIANS(degree) (degree * T3D_PI / 180.0f)

// to avoid using std::pow, std::sin, etc.
using namespace std;

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
};

template<typename T>
struct quat : vec4<T> {

    quat() = default;

    quat(const vec3<T>& v) {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        this->w = 0;
    }

    quat(T x, T y, T z, T w) {
        T radians = T3D_RADIANS(w);
        *this = {
            sinf(0.5f * radians) * x,
            sinf(0.5f * radians) * y,
            sinf(0.5f * radians) * z,
            cosf(0.5f * radians)
        };
    }

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

enum MatOrder {
    MAT_ORDER_ROW,
    MAT_ORDER_COLUMN
};

template<typename T, MatOrder order = MAT_ORDER_ROW>
struct mat2 {
    vec2<T> m[2] = {
        { 1, 0 },
        { 0, 1 },
    };

    mat2(const vec2<T>& v0 = {}, const vec2<T>& v1 = {}) {
        m[0] = v0;
        m[1] = v1;
        if constexpr (order == MAT_ORDER_COLUMN) {
            transpose(*this);
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

template<typename T, MatOrder order = MAT_ORDER_ROW>
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
            transpose(*this);
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

template<typename T, MatOrder order = MAT_ORDER_ROW>
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
            transpose(*this);
        }
    }

    mat4(const vec3<T>& v0 = {}, const vec3<T>& v1 = {}, const vec3<T>& v2 = {}, const vec3<T>& v3 = {}) {
        m[0] = v0;
        m[1] = v1;
        m[2] = v2;
        m[3] = v3;
        if constexpr (order == MAT_ORDER_COLUMN) {
            transpose(*this);
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
using float2x2 = mat2<float>;
using float3x3 = mat3<float>;
using float4x4 = mat4<float>;
using float2x2c = mat2<float, MAT_ORDER_COLUMN>;
using float3x3c = mat3<float, MAT_ORDER_COLUMN>;
using float4x4c = mat4<float, MAT_ORDER_COLUMN>;
typedef quat<float> quatf;

typedef vec2<int> int2;
typedef vec3<int> int3;
typedef vec4<int> int4;
using int2x2 = mat2<int>;
using int3x3 = mat3<int>;
using int4x4 = mat4<int>;
using int2x2c = mat2<int, MAT_ORDER_COLUMN>;
using int3x3c = mat3<int, MAT_ORDER_COLUMN>;
using int4x4c = mat4<int, MAT_ORDER_COLUMN>;
typedef quat<int> quati;

typedef vec2<double> double2;
typedef vec3<double> double3;
typedef vec4<double> double4;
using double2x2 = mat2<double>;
using double3x3 = mat3<double>;
using double4x4 = mat4<double>;
using double2x2c = mat2<double, MAT_ORDER_COLUMN>;
using double3x3c = mat3<double, MAT_ORDER_COLUMN>;
using double4x4c = mat4<double, MAT_ORDER_COLUMN>;
typedef quat<double> quatd;

template<typename T>
T clamp(const T& a, const T& b, const T& x) {
    return min(max(x, b), a);
}

template<typename T>
T lerp(const T& a, const T& b, const T& x) {
    return x * (b - a) + a;
}

template<typename T>
quat<T> slerp(const quat<T>& q1, const quat<T>& q2, T t) {
    quat<T> q3;
    T dot = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
    T theta, st, sut, sout, coeff1, coeff2;

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

    q3.normalize();

    return q3;
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

inline double randomd() {
    return rand() / static_cast<double>(RAND_MAX);
}

inline float randomf() {
    return rand() / static_cast<float>(RAND_MAX);
}

inline quatf rotate(const float3& v, const quatf& q) {
    return q * quat(v) * -q;
}

template<typename T>
mat2<T> transpose(const mat2<T>& m) {
    mat2<T> t = m;

    // swap(t[0][0], t[0][0]);
    swap(t[0][1], t[1][0]);

    // swap(t[1][0], t[0][1]);
    // swap(t[1][1], t[1][1]);

    return t;
}

template<typename T>
mat3<T> transpose(const mat3<T>& m) {
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
mat4<T> transpose(const mat4<T>& m) {
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
T det(const mat2<T>& m) {
    T d = m[0][0] * m[1][1] - m[1][0] * m[0][1];
    return d;
}

template<typename T>
T det(const mat3<T>& m) {
    T d = m[0][0] * m[1][1] * m[2][2]
        + m[0][1] * m[1][2] * m[2][0]
        + m[0][2] * m[1][0] * m[2][1]
        - m[2][0] * m[1][1] * m[0][2]
        - m[2][1] * m[1][2] * m[0][0]
        - m[2][2] * m[1][0] * m[0][1];
    return d;
}

template<typename T>
T det(const mat4<T>& m) {
    T d = m[0][0] * m[1][1] * m[2][2] * m[3][3]
        + m[0][1] * m[1][2] * m[2][3] * m[3][0]
        + m[0][2] * m[1][3] * m[2][0] * m[3][1]
        - m[3][0] * m[2][1] * m[1][2] * m[0][3]
        - m[3][1] * m[2][2] * m[1][3] * m[0][0]
        - m[3][2] * m[2][3] * m[1][0] * m[0][1];
    return d;
}

template<typename T, MatOrder O>
mat2<T> inverse(const mat2<T, O>& m) {
    mat2<T> c;
    T d = det(m);
    c[0][0] = m[1][1];
    c[0][1] =-m[1][0];
    c[1][0] =-m[0][1];
    c[1][1] = m[0][0];
    c /= d;
    return c;
}

template<typename T, MatOrder O>
mat3<T> inverse(const mat3<T, O>& m) {
    T d = det(m);
    mat3<T> c;

    c[0][0] = det(mat2<T> {
        m[1][1], m[1][2],
        m[2][1], m[2][2]
    });

    c[0][1] = -det(mat2<T> {
        m[1][0], m[1][2],
        m[2][0], m[2][2]
    });

    c[0][2] = det(mat2<T> {
        m[1][0], m[1][1],
        m[2][0], m[2][1]
    });

    c[1][0] = -det(mat2<T> {
        m[0][0], m[0][1],
        m[2][0], m[2][1]
    });

    c[1][1] = det(mat2<T> {
        m[0][0], m[0][2],
        m[2][0], m[2][2]
    });

    c[1][2] = -det(mat2<T> {
        m[0][0], m[0][1],
        m[2][0], m[2][1]
    });

    c[2][0] = det(mat2<T> {
        m[0][1], m[0][2],
        m[1][1], m[1][2]
    });

    c[2][1] = -det(mat2<T> {
        m[0][0], m[0][2],
        m[1][0], m[1][2]
    });

    c[2][2] = det(mat2<T> {
        m[0][0], m[0][1],
        m[1][0], m[1][1]
    });

    c /= d;

    return c;
}

template<typename T, MatOrder O>
mat4<T> inverse(const mat4<T, O>& m) {
    T d = det(m);
    mat4<T> c;

    c[0][0] = det(mat3<T> {
        m[1][1], m[1][2], m[1][3],
        m[2][1], m[2][2], m[2][3],
        m[3][1], m[3][2], m[3][3]
    });

    c[0][1] = -det(mat3<T> {
        m[1][0], m[1][2], m[1][3],
        m[2][0], m[2][2], m[2][3],
        m[3][0], m[3][2], m[3][3]
    });

    c[0][2] = det(mat3<T> {
        m[1][0], m[1][1], m[1][3],
        m[2][0], m[2][1], m[2][3],
        m[3][0], m[3][1], m[3][3]
    });

    c[0][3] = -det(mat3<T> {
        m[1][0], m[1][1], m[1][2],
        m[2][0], m[2][1], m[2][2],
        m[3][0], m[3][1], m[3][2]
    });

    c[1][0] = -det(mat3<T> {
        m[0][1], m[0][2], m[0][3],
        m[2][1], m[2][2], m[2][3],
        m[3][1], m[3][2], m[3][3]
    });

    c[1][1] = det(mat3<T> {
        m[0][0], m[0][2], m[0][3],
        m[2][0], m[2][2], m[2][3],
        m[3][0], m[3][2], m[3][3]
    });

    c[1][2] = -det(mat3<T> {
        m[0][0], m[0][1], m[0][3],
        m[2][0], m[2][1], m[2][3],
        m[3][0], m[3][1], m[3][3]
    });

    c[1][3] = det(mat3<T> {
        m[0][0], m[0][1], m[0][2],
        m[2][0], m[2][1], m[2][2],
        m[3][0], m[3][1], m[3][2]
    });

    c[2][0] = det(mat3<T> {
        m[0][1], m[0][2], m[0][3],
        m[1][1], m[1][2], m[1][3],
        m[3][1], m[3][2], m[3][3]
    });

    c[2][1] = -det(mat3<T> {
        m[0][0], m[0][2], m[0][3],
        m[1][0], m[1][2], m[1][3],
        m[3][0], m[3][2], m[3][3]
    });

    c[2][2] = det(mat3<T> {
        m[0][0], m[0][1], m[0][3],
        m[1][0], m[1][1], m[1][3],
        m[3][0], m[3][1], m[3][3]
    });

    c[2][3] = -det(mat3<T> {
        m[0][0], m[0][1], m[0][2],
        m[1][0], m[1][1], m[1][2],
        m[3][0], m[3][1], m[3][2]
    });

    c[3][0] = -det(mat3<T> {
        m[0][1], m[0][2], m[0][3],
        m[1][1], m[1][2], m[1][3],
        m[2][1], m[2][2], m[2][3]
    });

    c[3][1] = det(mat3<T> {
        m[0][0], m[0][2], m[0][3],
        m[1][0], m[1][2], m[1][3],
        m[2][0], m[2][2], m[2][3]
    });

    c[3][2] = -det(mat3<T> {
        m[0][0], m[0][1], m[0][3],
        m[1][0], m[1][1], m[1][3],
        m[2][0], m[2][1], m[2][3]
    });

    c[3][3] = det(mat3<T> {
        m[0][0], m[0][1], m[0][2],
        m[1][0], m[1][1], m[1][2],
        m[2][0], m[2][1], m[2][2]
    });

    c /= d;

    return c;
}

// NOTE: Fast inverse is only possible with model matrix!
template<typename T, MatOrder M>
mat4<T> inverseFast(const mat4<T, M>& m) {
    // TODO(cheerwizard): not implemented!
}

float4x4 translate(const float3& translation) {
    float4x4 m;
    m[0][3] = translation.x;
    m[1][3] = translation.y;
    m[2][3] = translation.z;
    return m;
}

inline float4x4 scale(const float3& scalar) {
    float4x4 m;
    m[0][0] = scalar.x;
    m[1][1] = scalar.y;
    m[2][2] = scalar.z;
    return m;
}

inline float4x4 rotateEuler(const float3& rotation, const float3& axis) {
    float4x4 rx;
    {
        float r = T3D_RADIANS(rotation.x);
        float sin = sin(r);
        float cos = cos(r);
        rx[1][1] = cos;
        rx[1][2] = -sin;
        rx[2][1] = sin;
        rx[2][2] = cos;
    }

    float4x4 ry;
    {
        float r = T3D_RADIANS(rotation.y);
        float sin = sin(r);
        float cos = cos(r);
        rx[0][0] = cos;
        rx[0][2] = sin;
        rx[2][0] = -sin;
        rx[2][2] = cos;
    }

    float4x4 rz;
    {
        float r = T3D_RADIANS(rotation.z);
        float sin = sin(r);
        float cos = cos(r);
        rx[0][0] = cos;
        rx[0][1] = -sin;
        rx[1][0] = sin;
        rx[1][1] = cos;
    }

    return rz * ry * rx;
}

inline float4x4 model(const float3& translation, const float3& rotation, const float3& scalar) {
    float4x4 m;
    m = translate(translation);
    m = m * rotateEuler(rotation, { 1, 1, 1 });
    m = m * scale(scalar);
    return m;
}

inline float4x4 model(const float3& translation, const quatf& rotation, const float3& scalar) {
    float4x4 m;
    m = translate(translation);
    m = m * float4x4(rotation);
    m = m * scale(scalar);
    return m;
}

inline float4x4 rigid(const float3& translation, const float3& rotation) {
    float4x4 m;
    m = translate(translation);
    m = m * rotateEuler(rotation, { 1, 1, 1 });
    return m;
}

inline float4x4 rigid(const float3& translation, const quatf& rotation) {
    float4x4 m;
    m = translate(translation);
    m = m * float4x4(rotation);
    return m;
}

inline float4x4 view(const float3& pos, const float3& dir, const float3& up) {
    float3 right = normalize(cross(dir, up));
    return inverseFast(float4x4c { right, cross(right, dir), -dir, pos });
}

inline float4x4 ortho(float left, float right, float bottom, float top, float near, float far) {
    return float4x4 {
        { 2.0f / (right - left), 0.0f, 0.0f, 0.0f },
        { 0.0f, 2.0f / (bottom - top), 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f / (near - far), 0.0f },
        { -(right + left) / (right - left), -(bottom + top) / (bottom - top), near / (near - far), 1.0f }
    };
}

inline float4x4 perspective(float aspect, float fov, float near, float far) {
    float f = 1.0f / tan(T3D_RADIANS(0.5f * fov));
    return float4x4 {
            { f / aspect, 0.0f, 0.0f, 0.0f },
            { 0.0f, -f, 0.0f, 0.0f },
            { 0.0f, 0.0f, far / (near - far), -1.0f },
            { 0.0f, 0.0f, near * far / (near - far), 0.0f }
    };
}