#pragma once

#include <Types.hpp>

#include <cmath>
#include <random>

#define T3D_PI 3.14159265359f
#define T3D_RADIANS(degree) (degree * T3D_PI / 180.0f)
#define T3D_E 2.71828f

using namespace std;

template<typename T>
inline T clamp(const T& a, const T& b, const T& x) {
    return min(max(x, b), a);
}

template<typename T>
inline T lerp(const T& a, const T& b, const T& x) {
    return x * (b - a) + a;
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

struct degree final {
    float a;

    explicit degree(float a = 0) : a(a) {}

    operator float() const { return a; }
};

struct radians final {
    float a;

    explicit radians(float a = 0) : a(a) {}
    explicit radians(const degree& d) : a(T3D_RADIANS(d)) {}

    operator float() const { return a; }
};

template<typename T>
struct vec2 {
    T x, y;

    vec2() = default;

    vec2(T x, T y) : x(x), y(y) {}

    vec2(const vec2<T>& v) = default;

    inline T& operator [](int i) {
        return *(&x + i);
    }

    inline friend vec2 operator +(const vec2& v1, const vec2& v2) {
        return { v1.x + v2.x, v1.y + v2.y };
    }

    inline friend vec2 operator -(const vec2& v1, const vec2& v2) {
        return { v1.x - v2.x, v1.y - v2.y };
    }

    inline friend vec2 operator *(const vec2& v1, const vec2& v2) {
        return { v1.x * v2.x, v1.y * v2.y };
    }

    inline friend vec2 operator /(const vec2& v1, const vec2& v2) {
        return { v1.x / v2.x, v1.y / v2.y };
    }

    inline friend vec2 operator +(const vec2& v1, const T& s) {
        return { v1.x + s, v1.y + s };
    }

    inline friend vec2 operator -(const vec2& v1, const T& s) {
        return { v1.x - s, v1.y - s };
    }

    inline friend vec2 operator *(const vec2& v1, const T& s) {
        return { v1.x * s, v1.y * s };
    }

    inline friend vec2 operator /(const vec2& v1, const T& s) {
        return { v1.x / s, v1.y / s };
    }

    inline friend vec2 operator +(const T& s, const vec2& v2) {
        return { s + v2.x, s + v2.y };
    }

    inline friend vec2 operator -(const T& s, const vec2& v2) {
        return { s - v2.x, s - v2.y };
    }

    inline friend vec2 operator *(const T& s, const vec2& v2) {
        return { s * v2.x, s * v2.y };
    }

    inline friend vec2 operator /(const T& s, const vec2& v2) {
        return { s / v2.x, s / v2.y };
    }

    inline friend vec2 operator ^(const vec2& v, const T& p) {
        return { pow(v.x, p), pow(v.y, p) };
    }

    inline friend vec2 operator -(const vec2& v) {
        return { -v.x, -v.y };
    }

    inline T length() const {
        return sqrt(x * x + y * y);
    }

    inline vec2 normalize() const {
        T l = length();
        return { x / l, y / l };
    }

    inline T dot(const vec2& v2) const {
        return x * v2.x + y * v2.y;
    }

    inline T cross(const vec2& v2) const {
        return x * v2.y - y * v2.x;
    }
};

template<typename T>
struct vec3 {
    T x, y, z;

    vec3() = default;

    vec3(T x, T y, T z) : x(x), y(y), z(z) {}

    vec3(const vec2<T>& v) : x(v.x), y(v.y) {}

    vec3(const vec3<T>& v) = default;

    inline T& operator [](int i) {
        return *(&x + i);
    }

    inline friend vec3 operator +(const vec3& v1, const vec3& v2) {
        return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
    }

    inline friend vec3 operator -(const vec3& v1, const vec3& v2) {
        return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
    }

    inline friend vec3 operator *(const vec3& v1, const vec3& v2) {
        return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
    }

    inline friend vec3 operator /(const vec3& v1, const vec3& v2) {
        return { v1.x / v2.x, v1.y / v2.y, v1.z / v2.z };
    }

    inline friend vec3 operator +(const vec3& v1, const T& s) {
        return { v1.x + s, v1.y + s, v1.z + s };
    }

    inline friend vec3 operator -(const vec3& v1, const T& s) {
        return { v1.x - s, v1.y - s, v1.z - s };
    }

    inline friend vec3 operator *(const vec3& v1, const T& s) {
        return { v1.x * s, v1.y * s, v1.z * s };
    }

    inline friend vec3 operator /(const vec3& v1, const T& s) {
        return { v1.x / s, v1.y / s, v1.z / s };
    }

    inline friend vec3 operator +(const T& s, const vec3& v2) {
        return { s + v2.x, s + v2.y, s + v2.z };
    }

    inline friend vec3 operator -(const T& s, const vec3& v2) {
        return { s - v2.x, s - v2.y, s - v2.z };
    }

    inline friend vec3 operator *(const T& s, const vec3& v2) {
        return { s * v2.x, s * v2.y, s * v2.z };
    }

    inline friend vec3 operator /(const T& s, const vec3& v2) {
        return { s / v2.x, s / v2.y, s / v2.z };
    }

    inline friend vec3 operator ^(const vec3& v, const T& p) {
        return { pow(v.x, p), pow(v.y, p), pow(v.z, p) };
    }

    inline friend vec3 operator -(const vec3& v) {
        return { -v.x, -v.y, -v.z };
    }

    inline vec2<T> xy() const {
        return { x, y };
    }

    inline T length() const {
        return sqrt(x * x + y * y + z * z);
    }

    inline vec3 normalize() const {
        T l = length();
        return { x / l, y / l, z / l };
    }

    inline T dot(const vec3& v2) const {
        return x * v2.x + y * v2.y + z * v2.z;
    }

    inline vec3 cross(const vec3& v2) const {
        return {
                y * v2.z - z * v2.y,
                z * v2.x - x * v2.z,
                x * v2.y - y * v2.x,
        };
    }
};

template<typename T>
struct vec4 {
    T x, y, z, w;

    vec4() = default;

    vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

    vec4(const vec2<T>& v) : x(v.x), y(v.y) {}

    vec4(const vec3<T>& v) : x(v.x), y(v.y), z(v.z) {}

    vec4(const vec4<T>& v) = default;

    inline T& operator [](int i) {
        return *(&x + i);
    }

    inline friend vec4 operator +(const vec4& v1, const vec4& v2) {
        return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w };
    }

    inline friend vec4 operator -(const vec4 v1, const vec4& v2) {
        return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w };
    }

    inline friend vec4 operator *(const vec4& v1, const vec4& v2) {
        return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w };
    }

    inline friend vec4 operator /(const vec4& v1, const vec4& v2) {
        return { v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w };
    }

    inline friend vec4 operator +(const vec4& v1, const T& s) {
        return { v1.x + s, v1.y + s, v1.z + s, v1.w + s };
    }

    inline friend vec4 operator -(const vec4& v1, const T& s) {
        return { v1.x - s, v1.y - s, v1.z - s, v1.w - s };
    }

    inline friend vec4 operator *(const vec4& v1, const T& s) {
        return { v1.x * s, v1.y * s, v1.z * s, v1.w * s };
    }

    inline friend vec4 operator /(const vec4& v1, const T& s) {
        return { v1.x / s, v1.y / s, v1.z / s, v1.w / s };
    }

    inline friend vec4 operator +(const T& s, const vec4& v2) {
        return { s + v2.x, s + v2.y, s + v2.z, s + v2.w };
    }

    inline friend vec4 operator -(const T& s, const vec4& v2) {
        return { s - v2.x, s - v2.y, s - v2.z, s - v2.w };
    }

    inline friend vec4 operator *(const T& s, const vec4& v2) {
        return { s * v2.x, s * v2.y, s * v2.z, s * v2.w };
    }

    inline friend vec4 operator /(const T& s, const vec4& v2) {
        return { s / v2.x, s / v2.y, s / v2.z, s / v2.w };
    }

    inline friend vec4 operator ^(const vec4& v, const T& p) {
        return { pow(v.x, p), pow(v.y, p), pow(v.z, p), pow(v.w, p) };
    }

    inline friend vec4 operator -(const vec4& v) {
        return { -v.x, -v.y, -v.z, -v.w };
    }

    inline vec2<T> xy() const {
        return { x, y };
    }

    inline vec3<T> xyz() const {
        return { x, y, z };
    }

    inline T length() const {
        return sqrt(x * x + y * y + z * z + w * w);
    }

    inline vec4 normalize() const {
        T l = length();
        return { x / l, y / l, z / l, w / l };
    }

    inline T dot(const vec4& v2) const {
        return x * v2.x + y * v2.y + z * v2.z + w * v2.w;
    }

    // TODO(cheerwizard): do I really need a wedged/geometry products?
    inline vec4 cross(const vec4& v2) const {
        // TODO(cheerwizard): not implemented!
        return {};
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
        x = nx * sin(r * 0.5f);
        y = ny * sin(r * 0.5f);
        z = nz * sin(r * 0.5f);
        w = cos(r * 0.5f);
    }

    quat(const vec3<float>& n, const radians& r = radians(0)) : quat(n.x, n.y, n.z , r) {}

    inline friend quat operator *(const quat& q1, const quat& q2) {
        quat q3;
        q3.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
        q3.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
        q3.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
        q3.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
        return q3;
    }

    inline friend quat operator-(const quat& q) {
        return { -q.x, -q.y, -q.z, q.w };
    }

    inline vec2<float> xy() const {
        return { x, y };
    }

    inline vec3<float> xyz() const {
        return { x, y, z };
    }

    inline float length() const {
        return sqrt(x * x + y * y + z * z);
    }

    inline quat normalize() {
        float l = length();
        return { x / l, y / l, z / l, w };
    }

    inline quat rotate(const vec3<float>& n) const {
        return *this * quat(n) * -*this;
    }

    inline quat slerp(const quat& q2, float t) {
        quat& q1 = *this;
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

        q3 = q3.normalize();

        return q3;
    }
};

template<typename T>
struct mat2 {
    vec2<T> m[2];

    mat2() = default;

    mat2(const vec2<T>& v0, const vec2<T>& v1) {
        m[0] = v0;
        m[1] = v1;
    }

    inline vec2<T>& operator [](int i) {
        return m[i];
    }

    inline friend mat2 operator *(const mat2& m1, const mat2& m2) {
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

    inline mat2 operator -() const {
        mat2 n;
        for (int r = 0 ; r < 2 ; r++) {
            for (int c = 0 ; c < 2 ; c++) {
                n[r][c] = -m[r][c];
            }
        }
        return n;
    }

    inline mat2 transpose() const {
        mat2 t = *this;

        // swap(t[0][0], t[0][0]);
        swap(t[0][1], t[1][0]);

        // swap(t[1][0], t[0][1]);
        // swap(t[1][1], t[1][1]);

        return t;
    }

    inline T det() const {
        T d = m[0][0] * m[1][1] - m[1][0] * m[0][1];
        return d;
    }

    mat2 inverse() const {
        mat2 c;
        T d = det();
        c[0][0] = m[1][1];
        c[0][1] =-m[1][0];
        c[1][0] =-m[0][1];
        c[1][1] = m[0][0];
        c /= d;
        return c;
    }
};

template<typename T>
struct mat3 {
    vec3<T> m[3];

    mat3() = default;

    mat3(const vec3<T>& v0, const vec3<T>& v1, const vec3<T>& v2) {
        m[0] = v0;
        m[1] = v1;
        m[2] = v2;
    }

    inline vec3<T>& operator [](int i) {
        return m[i];
    }

    inline friend mat3 operator *(const mat3& m1, const mat3& m2) {
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

    inline mat3 operator -() const {
        mat3 n;
        for (int r = 0 ; r < 3 ; r++) {
            for (int c = 0 ; c < 3 ; c++) {
                n[r][c] = -m[r][c];
            }
        }
        return n;
    }

    inline mat3 transpose() const {
        mat3 t = *this;

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

    inline T det() const {
        T d = m[0][0] * m[1][1] * m[2][2]
              + m[0][1] * m[1][2] * m[2][0]
              + m[0][2] * m[1][0] * m[2][1]
              - m[2][0] * m[1][1] * m[0][2]
              - m[2][1] * m[1][2] * m[0][0]
              - m[2][2] * m[1][0] * m[0][1];
        return d;
    }

    mat3 inverse() const {
        T d = det();
        mat3 c;

        c[0][0] = (mat2<T> {
                m[1][1], m[1][2],
                m[2][1], m[2][2]
        }).det();

        c[0][1] = -(mat2<T> {
                m[1][0], m[1][2],
                m[2][0], m[2][2]
        }).det();

        c[0][2] = (mat2<T> {
                m[1][0], m[1][1],
                m[2][0], m[2][1]
        }).det();

        c[1][0] = -(mat2<T> {
                m[0][0], m[0][1],
                m[2][0], m[2][1]
        }).det();

        c[1][1] = (mat2<T> {
                m[0][0], m[0][2],
                m[2][0], m[2][2]
        }).det();

        c[1][2] = -(mat2<T> {
                m[0][0], m[0][1],
                m[2][0], m[2][1]
        }).det();

        c[2][0] = (mat2<T> {
                m[0][1], m[0][2],
                m[1][1], m[1][2]
        }).det();

        c[2][1] = -(mat2<T> {
                m[0][0], m[0][2],
                m[1][0], m[1][2]
        }).det();

        c[2][2] = (mat2<T> {
                m[0][0], m[0][1],
                m[1][0], m[1][1]
        }).det();

        c /= d;

        return c;
    }
};

template<typename T>
struct mat4 {
    vec4<T> m[4];

    mat4() = default;

    mat4(const vec4<T>& v0, const vec4<T>& v1, const vec4<T>& v2, const vec4<T>& v3) {
        m[0] = v0;
        m[1] = v1;
        m[2] = v2;
        m[3] = v3;
    }

    mat4(const vec3<T>& v0, const vec3<T>& v1, const vec3<T>& v2, const vec3<T>& v3) {
        m[0] = v0;
        m[1] = v1;
        m[2] = v2;
        m[3] = v3;
    }

    mat4(const quat& q) {
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

    inline vec4<T>& operator [](int i) {
        return m[i];
    }

    inline friend mat4 operator *(const mat4& m1, const mat4& m2) {
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

    inline mat4 operator -() const {
        mat4 n;
        for (int r = 0 ; r < 4 ; r++) {
            for (int c = 0 ; c < 4 ; c++) {
                n[r][c] = -m[r][c];
            }
        }
        return n;
    }

    inline mat4 transpose() const {
        mat4 t = *this;

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

    inline T det() const {
        T d = m[0][0] * m[1][1] * m[2][2] * m[3][3]
              + m[0][1] * m[1][2] * m[2][3] * m[3][0]
              + m[0][2] * m[1][3] * m[2][0] * m[3][1]
              - m[3][0] * m[2][1] * m[1][2] * m[0][3]
              - m[3][1] * m[2][2] * m[1][3] * m[0][0]
              - m[3][2] * m[2][3] * m[1][0] * m[0][1];
        return d;
    }

    mat4 inverse() const {
        T d = det();
        mat4 c;

        c[0][0] = (mat3<T> {
                m[1][1], m[1][2], m[1][3],
                m[2][1], m[2][2], m[2][3],
                m[3][1], m[3][2], m[3][3]
        }).det();

        c[0][1] = -(mat3<T> {
                m[1][0], m[1][2], m[1][3],
                m[2][0], m[2][2], m[2][3],
                m[3][0], m[3][2], m[3][3]
        }).det();

        c[0][2] = (mat3<T> {
                m[1][0], m[1][1], m[1][3],
                m[2][0], m[2][1], m[2][3],
                m[3][0], m[3][1], m[3][3]
        }).det();

        c[0][3] = -(mat3<T> {
                m[1][0], m[1][1], m[1][2],
                m[2][0], m[2][1], m[2][2],
                m[3][0], m[3][1], m[3][2]
        }).det();

        c[1][0] = -(mat3<T> {
                m[0][1], m[0][2], m[0][3],
                m[2][1], m[2][2], m[2][3],
                m[3][1], m[3][2], m[3][3]
        }).det();

        c[1][1] = (mat3<T> {
                m[0][0], m[0][2], m[0][3],
                m[2][0], m[2][2], m[2][3],
                m[3][0], m[3][2], m[3][3]
        }).det();

        c[1][2] = -(mat3<T> {
                m[0][0], m[0][1], m[0][3],
                m[2][0], m[2][1], m[2][3],
                m[3][0], m[3][1], m[3][3]
        }).det();

        c[1][3] = (mat3<T> {
                m[0][0], m[0][1], m[0][2],
                m[2][0], m[2][1], m[2][2],
                m[3][0], m[3][1], m[3][2]
        }).det();

        c[2][0] = (mat3<T> {
                m[0][1], m[0][2], m[0][3],
                m[1][1], m[1][2], m[1][3],
                m[3][1], m[3][2], m[3][3]
        }).det();

        c[2][1] = -(mat3<T> {
                m[0][0], m[0][2], m[0][3],
                m[1][0], m[1][2], m[1][3],
                m[3][0], m[3][2], m[3][3]
        }).det();

        c[2][2] = (mat3<T> {
                m[0][0], m[0][1], m[0][3],
                m[1][0], m[1][1], m[1][3],
                m[3][0], m[3][1], m[3][3]
        }).det();

        c[2][3] = -(mat3<T> {
                m[0][0], m[0][1], m[0][2],
                m[1][0], m[1][1], m[1][2],
                m[3][0], m[3][1], m[3][2]
        }).det();

        c[3][0] = -(mat3<T> {
                m[0][1], m[0][2], m[0][3],
                m[1][1], m[1][2], m[1][3],
                m[2][1], m[2][2], m[2][3]
        }).det();

        c[3][1] = (mat3<T> {
                m[0][0], m[0][2], m[0][3],
                m[1][0], m[1][2], m[1][3],
                m[2][0], m[2][2], m[2][3]
        }).det();

        c[3][2] = -(mat3<T> {
                m[0][0], m[0][1], m[0][3],
                m[1][0], m[1][1], m[1][3],
                m[2][0], m[2][1], m[2][3]
        }).det();

        c[3][3] = (mat3<T> {
                m[0][0], m[0][1], m[0][2],
                m[1][0], m[1][1], m[1][2],
                m[2][0], m[2][1], m[2][2]
        }).det();

        c /= d;

        return c;
    }

    mat4 inverse_fast() const {
        // TODO(cheerwizard): not implemented!
        return {};
    }

    void translate(const vec3<T>& translation) {
        m[0][3] += translation.x;
        m[1][3] += translation.y;
        m[2][3] += translation.z;
    }

    void scale(const vec3<T>& scalar) {
        m[0][0] *= scalar.x;
        m[1][1] *= scalar.y;
        m[2][2] *= scalar.z;
    }

    void rotate(const vec3<radians>& r, const vec3<float>& axis) {
        mat4 rx;
        {
            float sinx = sin(r.x);
            float cosx = cos(r.x);
            rx[1][1] = cosx;
            rx[1][2] = -sinx;
            rx[2][1] = sinx;
            rx[2][2] = cosx;
            rx[0][0] = axis.x;
        }

        mat4 ry;
        {
            float siny = sin(r.y);
            float cosy = cos(r.y);
            ry[0][0] = cosy;
            ry[0][2] = siny;
            ry[2][0] = -siny;
            ry[2][2] = cosy;
            ry[1][1] = axis.y;
        }

        mat4 rz;
        {
            float sinz = sin(r.z);
            float cosz = cos(r.z);
            rz[0][0] = cosz;
            rz[0][1] = -sinz;
            rz[1][0] = sinz;
            rz[1][1] = cosz;
            rz[2][2] = axis.z;
        }

        *this = *this * rz * ry * rx;
    }

    static mat4 model(const vec3<float>& translation, const vec3<radians>& rotation, const vec3<float>& scalar) {
        mat4 m;
        m.translate(translation);
        m.rotate(rotation, { 1, 1, 1 });
        m.scale(scalar);
        return m;
    }

    static mat4 model(const vec3<float>& translation, const quat& rotation, const vec3<float>& scalar) {
        mat4 m;
        m.translate(translation);
        m = m * mat4(rotation);
        m.scale(scalar);
        return m;
    }

    static mat4 rigid(const vec3<float>& translation, const vec3<radians>& rotation) {
        mat4 m;
        m.translate(translation);
        m.rotate(rotation, { 1, 1, 1 });
        return m;
    }

    static mat4 rigid(const vec3<float>& translation, const quat& rotation) {
        mat4 m;
        m.translate(translation);
        m = m * mat4(rotation);
        return m;
    }

    static mat4<float> view(const vec3<float>& position, const vec3<float>& front, const vec3<float>& up) {
        vec3<float> right = front.cross(up).normalize();
        return mat4<float>(
                right,
                right.cross(front),
                -front,
                position
        ).transpose().inverse_fast();
    }

    static mat4<float> ortho(float left, float right, float bottom, float top, float z_near, float z_far) {
        return mat4<float> {
                { 2.0f / (right - left), 0.0f, 0.0f, 0.0f },
                { 0.0f, 2.0f / (bottom - top), 0.0f, 0.0f },
                { 0.0f, 0.0f, 1.0f / (z_near - z_far), 0.0f },
                { -(right + left) / (right - left), -(bottom + top) / (bottom - top), z_near / (z_near - z_far), 1.0f }
        };
    }

    static mat4<float> perspective(float aspect, degree fov, float z_near, float z_far) {
        float f = 1.0f / tan(radians(0.5f * fov));
        return mat4<float> {
                { f / aspect, 0.0f, 0.0f, 0.0f },
                { 0.0f, -f, 0.0f, 0.0f },
                { 0.0f, 0.0f, z_far / (z_near - z_far), -1.0f },
                { 0.0f, 0.0f, z_near * z_far / (z_near - z_far), 0.0f }
        };
    }

    static mat4<float> normal(const mat4<float>& model) {
        return model.inverse_fast().transpose();
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

typedef vec2<int> int2;
typedef vec3<int> int3;
typedef vec4<int> int4;
using int2x2 = mat2<int>;
using int3x3 = mat3<int>;
using int4x4 = mat4<int>;

typedef vec2<double> double2;
typedef vec3<double> double3;
typedef vec4<double> double4;
using double2x2 = mat2<double>;
using double3x3 = mat3<double>;
using double4x4 = mat4<double>;