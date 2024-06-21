#pragma once

#include <Types.hpp>

#include <cmath>
#include <random>

#define T3D_PI 3.14159265359f
#define T3D_RADIANS(degree) ((degree) * T3D_PI / 180.0f)
#define T3D_E 2.71828f

namespace sf {

    inline float clamp(float a, float b, float x) {
        return std::min(std::max(x, b), a);
    }

    inline float lerp(float a, float b, float x) {
        return x * (b - a) + a;
    }

    inline float step(float a, float x) {
        return clamp(0, 1, x - a);
    }

    inline float smoothstep(float a, float b, float x) {
        float t = clamp(0, 1, (x - a) / (b - a));
        float y = -2 * t * t * t + 3 * t * t;
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

        inline const T& operator [](int i) const {
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
            return { Pow(v.x, p), Pow(v.y, p) };
        }

        inline friend vec2 operator -(const vec2& v) {
            return { -v.x, -v.y };
        }
    };

    template<typename T>
    inline T length(const vec2<T>& v) {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }

    template<typename T>
    inline vec2<T> normalize(const vec2<T>& v) {
        T l = length(v);
        return { v.x / l, v.y / l };
    }

    template<typename T>
    inline T dot(const vec2<T>& v1, const vec2<T>& v2) {
        return v1.x * v2.x + v1.y * v2.y;
    }

    template<typename T>
    inline T cross(const vec2<T>& v1, const vec2<T>& v2) {
        return v1.x * v2.y - v1.y * v2.x;
    }

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

        inline const T& operator [](int i) const {
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
            return { Pow(v.x, p), Pow(v.y, p), Pow(v.z, p) };
        }

        inline friend vec3 operator -(const vec3& v) {
            return { -v.x, -v.y, -v.z };
        }

        inline vec2<T> xy() const {
            return { x, y };
        }
    };

    template<typename T>
    inline T length(const vec3<T>& v) {
        return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    }

    template<typename T>
    inline vec3<T> normalize(const vec3<T>& v) {
        T l = length(v);
        return { v.x / l, v.y / l, v.z / l };
    }

    template<typename T>
    inline T dot(const vec3<T>& v1, const vec3<T>& v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    template<typename T>
    inline vec3<T> cross(const vec3<T>& v1, const vec3<T>& v2) {
        return {
                v1.y * v2.z - v1.z * v2.y,
                v1.z * v2.x - v1.x * v2.z,
                v1.x * v2.y - v1.y * v2.x,
        };
    }

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

        inline const T& operator [](int i) const {
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
            return { Pow(v.x, p), Pow(v.y, p), Pow(v.z, p), Pow(v.w, p) };
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
    };

    template<typename T>
    inline T length(const vec4<T>& v) {
        return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
    }

    template<typename T>
    inline vec4<T> normalize(const vec4<T>& v) {
        T l = length(v);
        return { v.x / l, v.y / l, v.z / l, v.w / l };
    }

    template<typename T>
    inline T dot(const vec4<T>& v1, const vec4<T>& v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }

    // TODO(cheerwizard): do I really need a wedged/geometry products?
    template<typename T>
    inline vec4<T> cross(const vec4<T>& v1, const vec4<T>& v2) {
        // TODO(cheerwizard): not implemented!
        return {};
    }

    struct quat {
        float x = 0;
        float y = 0;
        float z = 0;
        float w = 1;

        quat() = default;

        quat(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        quat(float nx, float ny, float nz, const radians& r = radians(0)) {
            x = nx * std::sin(r * 0.5f);
            y = ny * std::sin(r * 0.5f);
            z = nz * std::sin(r * 0.5f);
            w = std::cos(r * 0.5f);
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
    };

    inline float length(const quat& q) {
        return std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z);
    }

    inline quat normalize(const quat& q) {
        float l = length(q);
        return { q.x / l, q.y / l, q.z / l, q.w };
    }

    inline quat rotate(const quat& q, const vec3<float>& n) {
        return q * quat(n) * -q;
    }

    inline quat slerp(const quat& q1, const quat& q2, float t) {
        quat q3;
        float dot = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
        float theta, st, sut, sout, coeff1, coeff2;

        t /= 2.0;

        theta = std::acos(dot);
        if (theta < 0.0) {
            theta=-theta;
        }

        st = std::sin(theta);
        sut = std::sin(t*theta);
        sout = std::sin((1-t)*theta);
        coeff1 = sout/st;
        coeff2 = sut/st;

        q3.x = coeff1*q1.x + coeff2*q2.x;
        q3.y = coeff1*q1.y + coeff2*q2.y;
        q3.z = coeff1*q1.z + coeff2*q2.z;
        q3.w = coeff1*q1.w + coeff2*q2.w;

        q3 = normalize(q3);

        return q3;
    }

    template<typename T>
    struct mat2 {
        vec2<T> m[2];

        mat2() = default;

        mat2(
                T m00, T m01,
                T m10, T m11
        ) {
            m[0][0] = m00;
            m[0][1] = m01;

            m[1][0] = m10;
            m[1][1] = m11;
        }

        mat2(const vec2<T>& v0, const vec2<T>& v1) {
            m[0] = v0;
            m[1] = v1;
        }

        inline vec2<T>& operator [](int i) {
            return m[i];
        }

        inline const vec2<T>& operator [](int i) const {
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
    };

    template<typename T>
    inline mat2<T> transpose(const mat2<T>& m) {
        mat2 t = m;

        // Swap(t[0][0], t[0][0]);
        std::swap(t[0][1], t[1][0]);

        // Swap(t[1][0], t[0][1]);
        // Swap(t[1][1], t[1][1]);

        return t;
    }

    template<typename T>
    inline T det(const mat2<T>& m) {
        return m[0][0] * m[1][1] - m[1][0] * m[0][1];
    }

    template<typename T>
    mat2<T> inverse(const mat2<T>& m) {
        mat2<T> c;
        T d = det(m);
        c[0][0] = m[1][1];
        c[0][1] =-m[1][0];
        c[1][0] =-m[0][1];
        c[1][1] = m[0][0];
        c /= d;
        return c;
    }

    template<typename T>
    struct mat3 {
        vec3<T> m[3];

        mat3() = default;

        mat3(
                T m00, T m01, T m02,
                T m10, T m11, T m12,
                T m20, T m21, T m22
        ) {
            m[0][0] = m00;
            m[0][1] = m01;
            m[0][2] = m02;

            m[1][0] = m10;
            m[1][1] = m11;
            m[1][2] = m12;

            m[2][0] = m20;
            m[2][1] = m21;
            m[2][2] = m22;
        }

        mat3(const vec3<T>& v0, const vec3<T>& v1, const vec3<T>& v2) {
            m[0] = v0;
            m[1] = v1;
            m[2] = v2;
        }

        inline vec3<T>& operator [](int i) {
            return m[i];
        }

        inline const vec3<T>& operator [](int i) const {
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
    };

    template<typename T>
    inline mat3<T> transpose(const mat3<T>& m) {
        mat3 t = m;

        // Swap(t[0][0], t[0][0]);
        std::swap(t[0][1], t[1][0]);
        std::swap(t[0][2], t[2][0]);

        // Swap(t[1][0], t[0][1]);
        // Swap(t[1][1], t[1][1]);
        std::swap(t[1][2], t[2][1]);

        // Swap(t[2][0], t[0][2]);
        // Swap(t[2][1], t[1][2]);
        // Swap(t[2][2], t[2][2]);

        return t;
    }

    template<typename T>
    inline T det(const mat3<T>& m) {
        T d = m[0][0] * m[1][1] * m[2][2]
              + m[0][1] * m[1][2] * m[2][0]
              + m[0][2] * m[1][0] * m[2][1]
              - m[2][0] * m[1][1] * m[0][2]
              - m[2][1] * m[1][2] * m[0][0]
              - m[2][2] * m[1][0] * m[0][1];
        return d;
    }

    template<typename T>
    mat3<T> inverse(const mat3<T>& m) {
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

    template<typename T>
    struct mat4 {
        vec4<T> m[4];

        mat4() = default;

        mat4(
                T m00, T m01, T m02, T m03,
                T m10, T m11, T m12, T m13,
                T m20, T m21, T m22, T m23,
                T m30, T m31, T m32, T m33
        ) {
            m[0][0] = m00;
            m[0][1] = m01;
            m[0][2] = m02;
            m[0][3] = m03;

            m[1][0] = m10;
            m[1][1] = m11;
            m[1][2] = m12;
            m[1][3] = m13;

            m[2][0] = m20;
            m[2][1] = m21;
            m[2][2] = m22;
            m[2][3] = m23;

            m[3][0] = m30;
            m[3][1] = m31;
            m[3][2] = m32;
            m[3][3] = m33;
        }

        mat4(const vec4<T> &v0, const vec4<T> &v1, const vec4<T> &v2, const vec4<T> &v3) {
            m[0] = v0;
            m[1] = v1;
            m[2] = v2;
            m[3] = v3;
        }

        mat4(const vec3<T> &v0, const vec3<T> &v1, const vec3<T> &v2, const vec3<T> &v3) {
            m[0] = v0;
            m[1] = v1;
            m[2] = v2;
            m[3] = v3;
        }

        mat4(const quat &q) {
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

        inline vec4<T>& operator[](int i) {
            return m[i];
        }

        inline const vec4<T>& operator[](int i) const {
            return m[i];
        }

        inline friend mat4 operator*(const mat4 &m1, const mat4 &m2) {
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

        inline mat4 operator-() const {
            mat4 n;
            for (int r = 0; r < 4; r++) {
                for (int c = 0; c < 4; c++) {
                    n[r][c] = -m[r][c];
                }
            }
            return n;
        }

        inline friend mat4 operator/(const mat4& m1, T v) {
            mat4 m2 = m1;
            for (int r = 0; r < 4; r++) {
                for (int c = 0; c < 4; c++) {
                    m2[r][c] /= v;
                }
            }
            return m2;
        }

        inline friend mat4 operator/=(const mat4& m1, T v) {
            mat4 m2 = m1;
            for (int r = 0; r < 4; r++) {
                for (int c = 0; c < 4; c++) {
                    m2[r][c] /= v;
                }
            }
            return m2;
        }
    };

    template<typename T>
    inline mat4<T> transpose(const mat4<T>& m) {
        mat4 t = m;

        // Swap(t[0][0], t[0][0]);
        std::swap(t[0][1], t[1][0]);
        std::swap(t[0][2], t[2][0]);
        std::swap(t[0][3], t[3][0]);

        // Swap(t[1][0], t[0][1]);
        // Swap(t[1][1], t[1][1]);
        std::swap(t[1][2], t[2][1]);
        std::swap(t[1][3], t[3][1]);

        // Swap(t[2][0], t[0][2]);
        // Swap(t[2][1], t[1][2]);
        // Swap(t[2][2], t[2][2]);
        std::swap(t[2][3], t[3][2]);

        // Swap(t[3][0], t[0][3]);
        // Swap(t[3][1], t[1][3]);
        // Swap(t[3][2], t[2][3]);
        // Swap(t[3][3], t[3][3]);

        return t;
    }

    template<typename T>
    inline T det(const mat4<T>& m) {
        T d = m[0][0] * m[1][1] * m[2][2] * m[3][3]
              + m[0][1] * m[1][2] * m[2][3] * m[3][0]
              + m[0][2] * m[1][3] * m[2][0] * m[3][1]
              - m[3][0] * m[2][1] * m[1][2] * m[0][3]
              - m[3][1] * m[2][2] * m[1][3] * m[0][0]
              - m[3][2] * m[2][3] * m[1][0] * m[0][1];
        return d;
    }

    template<typename T>
    mat4<T> inverse(const mat4<T>& m) {
        T d = det(m);

        mat4<T> c;

        c[0][0] = det(mat3<T>{
                m[1][1], m[1][2], m[1][3],
                m[2][1], m[2][2], m[2][3],
                m[3][1], m[3][2], m[3][3]
        });

        c[0][1] = -det(mat3<T>{
                m[1][0], m[1][2], m[1][3],
                m[2][0], m[2][2], m[2][3],
                m[3][0], m[3][2], m[3][3]
        });

        c[0][2] = det(mat3<T>{
                m[1][0], m[1][1], m[1][3],
                m[2][0], m[2][1], m[2][3],
                m[3][0], m[3][1], m[3][3]
        });

        c[0][3] = -det(mat3<T>{
                m[1][0], m[1][1], m[1][2],
                m[2][0], m[2][1], m[2][2],
                m[3][0], m[3][1], m[3][2]
        });

        c[1][0] = -det(mat3<T>{
                m[0][1], m[0][2], m[0][3],
                m[2][1], m[2][2], m[2][3],
                m[3][1], m[3][2], m[3][3]
        });

        c[1][1] = det(mat3<T>{
                m[0][0], m[0][2], m[0][3],
                m[2][0], m[2][2], m[2][3],
                m[3][0], m[3][2], m[3][3]
        });

        c[1][2] = -det(mat3<T>{
                m[0][0], m[0][1], m[0][3],
                m[2][0], m[2][1], m[2][3],
                m[3][0], m[3][1], m[3][3]
        });

        c[1][3] = det(mat3<T>{
                m[0][0], m[0][1], m[0][2],
                m[2][0], m[2][1], m[2][2],
                m[3][0], m[3][1], m[3][2]
        });

        c[2][0] = det(mat3<T>{
                m[0][1], m[0][2], m[0][3],
                m[1][1], m[1][2], m[1][3],
                m[3][1], m[3][2], m[3][3]
        });

        c[2][1] = -det(mat3<T>{
                m[0][0], m[0][2], m[0][3],
                m[1][0], m[1][2], m[1][3],
                m[3][0], m[3][2], m[3][3]
        });

        c[2][2] = det(mat3<T>{
                m[0][0], m[0][1], m[0][3],
                m[1][0], m[1][1], m[1][3],
                m[3][0], m[3][1], m[3][3]
        });

        c[2][3] = -det(mat3<T>{
                m[0][0], m[0][1], m[0][2],
                m[1][0], m[1][1], m[1][2],
                m[3][0], m[3][1], m[3][2]
        });

        c[3][0] = -det(mat3<T>{
                m[0][1], m[0][2], m[0][3],
                m[1][1], m[1][2], m[1][3],
                m[2][1], m[2][2], m[2][3]
        });

        c[3][1] = det(mat3<T>{
                m[0][0], m[0][2], m[0][3],
                m[1][0], m[1][2], m[1][3],
                m[2][0], m[2][2], m[2][3]
        });

        c[3][2] = -det(mat3<T>{
                m[0][0], m[0][1], m[0][3],
                m[1][0], m[1][1], m[1][3],
                m[2][0], m[2][1], m[2][3]
        });

        c[3][3] = det(mat3<T>{
                m[0][0], m[0][1], m[0][2],
                m[1][0], m[1][1], m[1][2],
                m[2][0], m[2][1], m[2][2]
        });

        c /= d;

        return c;
    }

    template<typename T>
    mat4<T> inverse_fast(const mat4<T>& m) {
        // TODO(cheerwizard): not implemented!
        return {};
    }

    void translate(mat4<float>& m, const vec3<float>& translation) {
        m[0][3] += translation.x;
        m[1][3] += translation.y;
        m[2][3] += translation.z;
    }

    void scale(mat4<float>& m, const vec3<float>& scalar) {
        m[0][0] *= scalar.x;
        m[1][1] *= scalar.y;
        m[2][2] *= scalar.z;
    }

    void rotate(mat4<float>& m, const vec3<radians>& r, const vec3<float>& axis) {
        mat4<float> rx;
        {
            float sinx = std::sin(r.x);
            float cosx = std::cos(r.x);
            rx[1][1] = cosx;
            rx[1][2] = -sinx;
            rx[2][1] = sinx;
            rx[2][2] = cosx;
            rx[0][0] = axis.x;
        }

        mat4<float> ry;
        {
            float siny = std::sin(r.y);
            float cosy = std::cos(r.y);
            ry[0][0] = cosy;
            ry[0][2] = siny;
            ry[2][0] = -siny;
            ry[2][2] = cosy;
            ry[1][1] = axis.y;
        }

        mat4<float> rz;
        {
            float sinz = std::sin(r.z);
            float cosz = std::cos(r.z);
            rz[0][0] = cosz;
            rz[0][1] = -sinz;
            rz[1][0] = sinz;
            rz[1][1] = cosz;
            rz[2][2] = axis.z;
        }

        m = m * rz * ry * rx;
    }

    mat4<float> mat4_model(const vec3<float>& translation, const vec3<radians>& rotation, const vec3<float>& scalar) {
        mat4<float> m;
        translate(m, translation);
        rotate(m, rotation, { 1, 1, 1 });
        scale(m, scalar);
        return m;
    }

    mat4<float> mat4_model(const vec3<float>& translation, const quat& rotation, const vec3<float>& scalar) {
        mat4<float> m;
        translate(m, translation);
        m = m * mat4<float>(rotation);
        scale(m, scalar);
        return m;
    }

    mat4<float> mat4_rigid(const vec3<float>& translation, const vec3<radians>& rotation) {
        mat4<float> m;
        translate(m, translation);
        rotate(m, rotation, { 1, 1, 1 });
        return m;
    }

    inline mat4<float> mat4_rigid(const vec3<float>& translation, const quat& rotation) {
        mat4<float> m;
        translate(m, translation);
        m = m * mat4<float>(rotation);
        return m;
    }

    inline mat4<float> mat4_view(const vec3<float>& position, const vec3<float>& front, const vec3<float>& up) {
        vec3<float> right = normalize(cross(front, up));
        return inverse_fast(transpose(mat4<float>(
                right,
                cross(right, front),
                -front,
                position
        )));
    }

    inline mat4<float> mat4_ortho(float left, float right, float bottom, float top, float z_near, float z_far) {
        return mat4<float> {
                { 2.0f / (right - left), 0.0f, 0.0f, 0.0f },
                { 0.0f, 2.0f / (bottom - top), 0.0f, 0.0f },
                { 0.0f, 0.0f, 1.0f / (z_near - z_far), 0.0f },
                { -(right + left) / (right - left), -(bottom + top) / (bottom - top), z_near / (z_near - z_far), 1.0f }
        };
    }

    inline mat4<float> mat4_perspective(float aspect, degree fov, float z_near, float z_far) {
        float f = 1.0f / tan(radians(0.5f * fov));
        return mat4<float> {
                { f / aspect, 0.0f, 0.0f, 0.0f },
                { 0.0f, -f, 0.0f, 0.0f },
                { 0.0f, 0.0f, z_far / (z_near - z_far), -1.0f },
                { 0.0f, 0.0f, z_near * z_far / (z_near - z_far), 0.0f }
        };
    }

    inline mat4<float> mat4_normal(const mat4<float>& model) {
        mat4<float> m = transpose(inverse(model));
        return m;
    }

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

};