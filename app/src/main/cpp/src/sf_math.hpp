#pragma once

#include <sf.hpp>
#include <cmath>
#include <random>

#define SF_PI 3.14159265359f
#define SF_RADIANS(degree) ((degree) * SF_PI / 180.0f)
#define SF_E 2.71828f

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

    struct degree_t final {
        float a;

        explicit degree_t(float a = 0) : a(a) {}

        operator float() const { return a; }
    };

    struct radians_t final {
        float a;

        explicit radians_t(float a = 0) : a(a) {}
        explicit radians_t(const degree_t& d) : a(SF_RADIANS(d)) {}

        operator float() const { return a; }
    };

    template<typename T>
    struct vec2_t {
        T x, y;

        vec2_t() = default;

        vec2_t(T x, T y) : x(x), y(y) {}

        vec2_t(const vec2_t<T>& v) = default;

        inline T& operator [](int i) {
            return *(&x + i);
        }

        inline const T& operator [](int i) const {
            return *(&x + i);
        }

        inline friend vec2_t operator +(const vec2_t& v1, const vec2_t& v2) {
            return { v1.x + v2.x, v1.y + v2.y };
        }

        inline friend vec2_t operator -(const vec2_t& v1, const vec2_t& v2) {
            return { v1.x - v2.x, v1.y - v2.y };
        }

        inline friend vec2_t operator *(const vec2_t& v1, const vec2_t& v2) {
            return { v1.x * v2.x, v1.y * v2.y };
        }

        inline friend vec2_t operator /(const vec2_t& v1, const vec2_t& v2) {
            return { v1.x / v2.x, v1.y / v2.y };
        }

        inline friend vec2_t operator +(const vec2_t& v1, const T& s) {
            return { v1.x + s, v1.y + s };
        }

        inline friend vec2_t operator -(const vec2_t& v1, const T& s) {
            return { v1.x - s, v1.y - s };
        }

        inline friend vec2_t operator *(const vec2_t& v1, const T& s) {
            return { v1.x * s, v1.y * s };
        }

        inline friend vec2_t operator /(const vec2_t& v1, const T& s) {
            return { v1.x / s, v1.y / s };
        }

        inline friend vec2_t operator +(const T& s, const vec2_t& v2) {
            return { s + v2.x, s + v2.y };
        }

        inline friend vec2_t operator -(const T& s, const vec2_t& v2) {
            return { s - v2.x, s - v2.y };
        }

        inline friend vec2_t operator *(const T& s, const vec2_t& v2) {
            return { s * v2.x, s * v2.y };
        }

        inline friend vec2_t operator /(const T& s, const vec2_t& v2) {
            return { s / v2.x, s / v2.y };
        }

        inline friend vec2_t operator ^(const vec2_t& v, const T& p) {
            return { Pow(v.x, p), Pow(v.y, p) };
        }

        inline friend vec2_t operator -(const vec2_t& v) {
            return { -v.x, -v.y };
        }
    };

    template<typename T>
    inline T length(const vec2_t<T>& v) {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }

    template<typename T>
    inline vec2_t<T> normalize(const vec2_t<T>& v) {
        T l = length(v);
        return { v.x / l, v.y / l };
    }

    template<typename T>
    inline T dot(const vec2_t<T>& v1, const vec2_t<T>& v2) {
        return v1.x * v2.x + v1.y * v2.y;
    }

    template<typename T>
    inline T cross(const vec2_t<T>& v1, const vec2_t<T>& v2) {
        return v1.x * v2.y - v1.y * v2.x;
    }

    template<typename T>
    struct vec3_t {
        T x, y, z;

        vec3_t() = default;

        vec3_t(T x, T y, T z) : x(x), y(y), z(z) {}

        vec3_t(const vec2_t<T>& v) : x(v.x), y(v.y) {}

        vec3_t(const vec3_t<T>& v) = default;

        inline T& operator [](int i) {
            return *(&x + i);
        }

        inline const T& operator [](int i) const {
            return *(&x + i);
        }

        inline friend vec3_t operator +(const vec3_t& v1, const vec3_t& v2) {
            return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
        }

        inline friend vec3_t operator -(const vec3_t& v1, const vec3_t& v2) {
            return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
        }

        inline friend vec3_t operator *(const vec3_t& v1, const vec3_t& v2) {
            return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
        }

        inline friend vec3_t operator /(const vec3_t& v1, const vec3_t& v2) {
            return { v1.x / v2.x, v1.y / v2.y, v1.z / v2.z };
        }

        inline friend vec3_t operator +(const vec3_t& v1, const T& s) {
            return { v1.x + s, v1.y + s, v1.z + s };
        }

        inline friend vec3_t operator -(const vec3_t& v1, const T& s) {
            return { v1.x - s, v1.y - s, v1.z - s };
        }

        inline friend vec3_t operator *(const vec3_t& v1, const T& s) {
            return { v1.x * s, v1.y * s, v1.z * s };
        }

        inline friend vec3_t operator /(const vec3_t& v1, const T& s) {
            return { v1.x / s, v1.y / s, v1.z / s };
        }

        inline friend vec3_t operator +(const T& s, const vec3_t& v2) {
            return { s + v2.x, s + v2.y, s + v2.z };
        }

        inline friend vec3_t operator -(const T& s, const vec3_t& v2) {
            return { s - v2.x, s - v2.y, s - v2.z };
        }

        inline friend vec3_t operator *(const T& s, const vec3_t& v2) {
            return { s * v2.x, s * v2.y, s * v2.z };
        }

        inline friend vec3_t operator /(const T& s, const vec3_t& v2) {
            return { s / v2.x, s / v2.y, s / v2.z };
        }

        inline friend vec3_t operator ^(const vec3_t& v, const T& p) {
            return { Pow(v.x, p), Pow(v.y, p), Pow(v.z, p) };
        }

        inline friend vec3_t operator -(const vec3_t& v) {
            return { -v.x, -v.y, -v.z };
        }

        inline vec2_t<T> xy() const {
            return { x, y };
        }
    };

    template<typename T>
    inline T length(const vec3_t<T>& v) {
        return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    }

    template<typename T>
    inline vec3_t<T> normalize(const vec3_t<T>& v) {
        T l = length(v);
        return { v.x / l, v.y / l, v.z / l };
    }

    template<typename T>
    inline T dot(const vec3_t<T>& v1, const vec3_t<T>& v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    template<typename T>
    inline vec3_t<T> cross(const vec3_t<T>& v1, const vec3_t<T>& v2) {
        return {
                v1.y * v2.z - v1.z * v2.y,
                v1.z * v2.x - v1.x * v2.z,
                v1.x * v2.y - v1.y * v2.x,
        };
    }

    template<typename T>
    struct vec4_t {
        T x, y, z, w;

        vec4_t() = default;

        vec4_t(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

        vec4_t(const vec2_t<T>& v) : x(v.x), y(v.y) {}

        vec4_t(const vec3_t<T>& v) : x(v.x), y(v.y), z(v.z) {}

        vec4_t(const vec4_t<T>& v) = default;

        inline T& operator [](int i) {
            return *(&x + i);
        }

        inline const T& operator [](int i) const {
            return *(&x + i);
        }

        inline friend vec4_t operator +(const vec4_t& v1, const vec4_t& v2) {
            return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w };
        }

        inline friend vec4_t operator -(const vec4_t v1, const vec4_t& v2) {
            return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w };
        }

        inline friend vec4_t operator *(const vec4_t& v1, const vec4_t& v2) {
            return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w };
        }

        inline friend vec4_t operator /(const vec4_t& v1, const vec4_t& v2) {
            return { v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w };
        }

        inline friend vec4_t operator +(const vec4_t& v1, const T& s) {
            return { v1.x + s, v1.y + s, v1.z + s, v1.w + s };
        }

        inline friend vec4_t operator -(const vec4_t& v1, const T& s) {
            return { v1.x - s, v1.y - s, v1.z - s, v1.w - s };
        }

        inline friend vec4_t operator *(const vec4_t& v1, const T& s) {
            return { v1.x * s, v1.y * s, v1.z * s, v1.w * s };
        }

        inline friend vec4_t operator /(const vec4_t& v1, const T& s) {
            return { v1.x / s, v1.y / s, v1.z / s, v1.w / s };
        }

        inline friend vec4_t operator +(const T& s, const vec4_t& v2) {
            return { s + v2.x, s + v2.y, s + v2.z, s + v2.w };
        }

        inline friend vec4_t operator -(const T& s, const vec4_t& v2) {
            return { s - v2.x, s - v2.y, s - v2.z, s - v2.w };
        }

        inline friend vec4_t operator *(const T& s, const vec4_t& v2) {
            return { s * v2.x, s * v2.y, s * v2.z, s * v2.w };
        }

        inline friend vec4_t operator /(const T& s, const vec4_t& v2) {
            return { s / v2.x, s / v2.y, s / v2.z, s / v2.w };
        }

        inline friend vec4_t operator ^(const vec4_t& v, const T& p) {
            return { Pow(v.x, p), Pow(v.y, p), Pow(v.z, p), Pow(v.w, p) };
        }

        inline friend vec4_t operator -(const vec4_t& v) {
            return { -v.x, -v.y, -v.z, -v.w };
        }

        inline vec2_t<T> xy() const {
            return { x, y };
        }

        inline vec3_t<T> xyz() const {
            return { x, y, z };
        }
    };

    template<typename T>
    inline T length(const vec4_t<T>& v) {
        return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
    }

    template<typename T>
    inline vec4_t<T> normalize(const vec4_t<T>& v) {
        T l = length(v);
        return { v.x / l, v.y / l, v.z / l, v.w / l };
    }

    template<typename T>
    inline T dot(const vec4_t<T>& v1, const vec4_t<T>& v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }

    // TODO(cheerwizard): do I really need a wedged/geometry products?
    template<typename T>
    inline vec4_t<T> cross(const vec4_t<T>& v1, const vec4_t<T>& v2) {
        // TODO(cheerwizard): not implemented!
        return {};
    }

    struct quat_t {
        float x = 0;
        float y = 0;
        float z = 0;
        float w = 1;

        quat_t() = default;

        quat_t(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        quat_t(float nx, float ny, float nz, const radians_t& r = radians_t(0)) {
            x = nx * std::sin(r * 0.5f);
            y = ny * std::sin(r * 0.5f);
            z = nz * std::sin(r * 0.5f);
            w = std::cos(r * 0.5f);
        }

        quat_t(const vec3_t<float>& n, const radians_t& r = radians_t(0)) : quat_t(n.x, n.y, n.z , r) {}

        inline friend quat_t operator *(const quat_t& q1, const quat_t& q2) {
            quat_t q3;
            q3.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
            q3.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
            q3.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
            q3.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
            return q3;
        }

        inline friend quat_t operator-(const quat_t& q) {
            return { -q.x, -q.y, -q.z, q.w };
        }

        inline vec2_t<float> xy() const {
            return { x, y };
        }

        inline vec3_t<float> xyz() const {
            return { x, y, z };
        }
    };

    inline float length(const quat_t& q) {
        return std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z);
    }

    inline quat_t normalize(const quat_t& q) {
        float l = length(q);
        return { q.x / l, q.y / l, q.z / l, q.w };
    }

    inline quat_t rotate(const quat_t& q, const vec3_t<float>& n) {
        return q * quat_t(n) * -q;
    }

    inline quat_t slerp(const quat_t& q1, const quat_t& q2, float t) {
        quat_t q3;
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
    struct mat2_t {
        vec2_t<T> m[2];

        mat2_t() = default;

        mat2_t(
                T m00, T m01,
                T m10, T m11
        ) {
            m[0][0] = m00;
            m[0][1] = m01;

            m[1][0] = m10;
            m[1][1] = m11;
        }

        mat2_t(const vec2_t<T>& v0, const vec2_t<T>& v1) {
            m[0] = v0;
            m[1] = v1;
        }

        inline vec2_t<T>& operator [](int i) {
            return m[i];
        }

        inline const vec2_t<T>& operator [](int i) const {
            return m[i];
        }

        inline friend mat2_t operator *(const mat2_t& m1, const mat2_t& m2) {
            mat2_t m3;
            for (int r = 0 ; r < 2 ; r++) {
                for (int c = 0 ; c < 2 ; c++) {
                    for (int i = 0 ; i < 2 ; i++) {
                        m3[r][c] += m1[r][i] * m2[i][c];
                    }
                }
            }
            return m3;
        }

        inline mat2_t operator -() const {
            mat2_t n;
            for (int r = 0 ; r < 2 ; r++) {
                for (int c = 0 ; c < 2 ; c++) {
                    n[r][c] = -m[r][c];
                }
            }
            return n;
        }
    };

    template<typename T>
    inline mat2_t<T> transpose(const mat2_t<T>& m) {
        mat2_t t = m;

        // Swap(t[0][0], t[0][0]);
        std::swap(t[0][1], t[1][0]);

        // Swap(t[1][0], t[0][1]);
        // Swap(t[1][1], t[1][1]);

        return t;
    }

    template<typename T>
    inline T det(const mat2_t<T>& m) {
        return m[0][0] * m[1][1] - m[1][0] * m[0][1];
    }

    template<typename T>
    mat2_t<T> inverse(const mat2_t<T>& m) {
        mat2_t<T> c;
        T d = det(m);
        c[0][0] = m[1][1];
        c[0][1] =-m[1][0];
        c[1][0] =-m[0][1];
        c[1][1] = m[0][0];
        c /= d;
        return c;
    }

    template<typename T>
    struct mat3_t {
        vec3_t<T> m[3];

        mat3_t() = default;

        mat3_t(
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

        mat3_t(const vec3_t<T>& v0, const vec3_t<T>& v1, const vec3_t<T>& v2) {
            m[0] = v0;
            m[1] = v1;
            m[2] = v2;
        }

        inline vec3_t<T>& operator [](int i) {
            return m[i];
        }

        inline const vec3_t<T>& operator [](int i) const {
            return m[i];
        }

        inline friend mat3_t operator *(const mat3_t& m1, const mat3_t& m2) {
            mat3_t m3;
            for (int r = 0; r < 3; r++) {
                for (int c = 0; c < 3; c++) {
                    for (int i = 0; i < 3; i++) {
                        m3[r][c] += m1[r][i] * m2[i][c];
                    }
                }
            }
            return m3;
        }

        inline mat3_t operator -() const {
            mat3_t n;
            for (int r = 0 ; r < 3 ; r++) {
                for (int c = 0 ; c < 3 ; c++) {
                    n[r][c] = -m[r][c];
                }
            }
            return n;
        }
    };

    template<typename T>
    inline mat3_t<T> transpose(const mat3_t<T>& m) {
        mat3_t t = m;

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
    inline T det(const mat3_t<T>& m) {
        T d = m[0][0] * m[1][1] * m[2][2]
              + m[0][1] * m[1][2] * m[2][0]
              + m[0][2] * m[1][0] * m[2][1]
              - m[2][0] * m[1][1] * m[0][2]
              - m[2][1] * m[1][2] * m[0][0]
              - m[2][2] * m[1][0] * m[0][1];
        return d;
    }

    template<typename T>
    mat3_t<T> inverse(const mat3_t<T>& m) {
        T d = det(m);

        mat3_t<T> c;

        c[0][0] = det(mat2_t<T> {
                m[1][1], m[1][2],
                m[2][1], m[2][2]
        });

        c[0][1] = -det(mat2_t<T> {
                m[1][0], m[1][2],
                m[2][0], m[2][2]
        });

        c[0][2] = det(mat2_t<T> {
                m[1][0], m[1][1],
                m[2][0], m[2][1]
        });

        c[1][0] = -det(mat2_t<T> {
                m[0][0], m[0][1],
                m[2][0], m[2][1]
        });

        c[1][1] = det(mat2_t<T> {
                m[0][0], m[0][2],
                m[2][0], m[2][2]
        });

        c[1][2] = -det(mat2_t<T> {
                m[0][0], m[0][1],
                m[2][0], m[2][1]
        });

        c[2][0] = det(mat2_t<T> {
                m[0][1], m[0][2],
                m[1][1], m[1][2]
        });

        c[2][1] = -det(mat2_t<T> {
                m[0][0], m[0][2],
                m[1][0], m[1][2]
        });

        c[2][2] = det(mat2_t<T> {
                m[0][0], m[0][1],
                m[1][0], m[1][1]
        });

        c /= d;

        return c;
    }

    template<typename T>
    struct mat4_t {
        vec4_t<T> m[4];

        mat4_t() = default;

        mat4_t(
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

        mat4_t(const vec4_t<T> &v0, const vec4_t<T> &v1, const vec4_t<T> &v2, const vec4_t<T> &v3) {
            m[0] = v0;
            m[1] = v1;
            m[2] = v2;
            m[3] = v3;
        }

        mat4_t(const vec3_t<T> &v0, const vec3_t<T> &v1, const vec3_t<T> &v2, const vec3_t<T> &v3) {
            m[0] = v0;
            m[1] = v1;
            m[2] = v2;
            m[3] = v3;
        }

        mat4_t(const quat_t &q) {
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

        inline vec4_t<T>& operator[](int i) {
            return m[i];
        }

        inline const vec4_t<T>& operator[](int i) const {
            return m[i];
        }

        inline friend mat4_t operator*(const mat4_t &m1, const mat4_t &m2) {
            mat4_t m3;
            for (int r = 0; r < 4; r++) {
                for (int c = 0; c < 4; c++) {
                    for (int i = 0; i < 4; i++) {
                        m3[r][c] += m1[r][i] * m2[i][c];
                    }
                }
            }
            return m3;
        }

        inline mat4_t operator-() const {
            mat4_t n;
            for (int r = 0; r < 4; r++) {
                for (int c = 0; c < 4; c++) {
                    n[r][c] = -m[r][c];
                }
            }
            return n;
        }

        inline friend mat4_t operator/(const mat4_t& m1, T v) {
            mat4_t m2 = m1;
            for (int r = 0; r < 4; r++) {
                for (int c = 0; c < 4; c++) {
                    m2[r][c] /= v;
                }
            }
            return m2;
        }

        inline friend mat4_t operator/=(const mat4_t& m1, T v) {
            mat4_t m2 = m1;
            for (int r = 0; r < 4; r++) {
                for (int c = 0; c < 4; c++) {
                    m2[r][c] /= v;
                }
            }
            return m2;
        }
    };

    template<typename T>
    inline mat4_t<T> transpose(const mat4_t<T>& m) {
        mat4_t t = m;

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
    inline T det(const mat4_t<T>& m) {
        T d = m[0][0] * m[1][1] * m[2][2] * m[3][3]
              + m[0][1] * m[1][2] * m[2][3] * m[3][0]
              + m[0][2] * m[1][3] * m[2][0] * m[3][1]
              - m[3][0] * m[2][1] * m[1][2] * m[0][3]
              - m[3][1] * m[2][2] * m[1][3] * m[0][0]
              - m[3][2] * m[2][3] * m[1][0] * m[0][1];
        return d;
    }

    template<typename T>
    mat4_t<T> inverse(const mat4_t<T>& m) {
        T d = det(m);

        mat4_t<T> c;

        c[0][0] = det(mat3_t<T>{
                m[1][1], m[1][2], m[1][3],
                m[2][1], m[2][2], m[2][3],
                m[3][1], m[3][2], m[3][3]
        });

        c[0][1] = -det(mat3_t<T>{
                m[1][0], m[1][2], m[1][3],
                m[2][0], m[2][2], m[2][3],
                m[3][0], m[3][2], m[3][3]
        });

        c[0][2] = det(mat3_t<T>{
                m[1][0], m[1][1], m[1][3],
                m[2][0], m[2][1], m[2][3],
                m[3][0], m[3][1], m[3][3]
        });

        c[0][3] = -det(mat3_t<T>{
                m[1][0], m[1][1], m[1][2],
                m[2][0], m[2][1], m[2][2],
                m[3][0], m[3][1], m[3][2]
        });

        c[1][0] = -det(mat3_t<T>{
                m[0][1], m[0][2], m[0][3],
                m[2][1], m[2][2], m[2][3],
                m[3][1], m[3][2], m[3][3]
        });

        c[1][1] = det(mat3_t<T>{
                m[0][0], m[0][2], m[0][3],
                m[2][0], m[2][2], m[2][3],
                m[3][0], m[3][2], m[3][3]
        });

        c[1][2] = -det(mat3_t<T>{
                m[0][0], m[0][1], m[0][3],
                m[2][0], m[2][1], m[2][3],
                m[3][0], m[3][1], m[3][3]
        });

        c[1][3] = det(mat3_t<T>{
                m[0][0], m[0][1], m[0][2],
                m[2][0], m[2][1], m[2][2],
                m[3][0], m[3][1], m[3][2]
        });

        c[2][0] = det(mat3_t<T>{
                m[0][1], m[0][2], m[0][3],
                m[1][1], m[1][2], m[1][3],
                m[3][1], m[3][2], m[3][3]
        });

        c[2][1] = -det(mat3_t<T>{
                m[0][0], m[0][2], m[0][3],
                m[1][0], m[1][2], m[1][3],
                m[3][0], m[3][2], m[3][3]
        });

        c[2][2] = det(mat3_t<T>{
                m[0][0], m[0][1], m[0][3],
                m[1][0], m[1][1], m[1][3],
                m[3][0], m[3][1], m[3][3]
        });

        c[2][3] = -det(mat3_t<T>{
                m[0][0], m[0][1], m[0][2],
                m[1][0], m[1][1], m[1][2],
                m[3][0], m[3][1], m[3][2]
        });

        c[3][0] = -det(mat3_t<T>{
                m[0][1], m[0][2], m[0][3],
                m[1][1], m[1][2], m[1][3],
                m[2][1], m[2][2], m[2][3]
        });

        c[3][1] = det(mat3_t<T>{
                m[0][0], m[0][2], m[0][3],
                m[1][0], m[1][2], m[1][3],
                m[2][0], m[2][2], m[2][3]
        });

        c[3][2] = -det(mat3_t<T>{
                m[0][0], m[0][1], m[0][3],
                m[1][0], m[1][1], m[1][3],
                m[2][0], m[2][1], m[2][3]
        });

        c[3][3] = det(mat3_t<T>{
                m[0][0], m[0][1], m[0][2],
                m[1][0], m[1][1], m[1][2],
                m[2][0], m[2][1], m[2][2]
        });

        c /= d;

        return c;
    }

    template<typename T>
    mat4_t<T> inverse_fast(const mat4_t<T>& m) {
        // TODO(cheerwizard): not implemented!
        return {};
    }

    void translate(mat4_t<float>& m, const vec3_t<float>& translation) {
        m[0][3] += translation.x;
        m[1][3] += translation.y;
        m[2][3] += translation.z;
    }

    void scale(mat4_t<float>& m, const vec3_t<float>& scalar) {
        m[0][0] *= scalar.x;
        m[1][1] *= scalar.y;
        m[2][2] *= scalar.z;
    }

    void rotate(mat4_t<float>& m, const vec3_t<radians_t>& r, const vec3_t<float>& axis) {
        mat4_t<float> rx;
        {
            float sinx = std::sin(r.x);
            float cosx = std::cos(r.x);
            rx[1][1] = cosx;
            rx[1][2] = -sinx;
            rx[2][1] = sinx;
            rx[2][2] = cosx;
            rx[0][0] = axis.x;
        }

        mat4_t<float> ry;
        {
            float siny = std::sin(r.y);
            float cosy = std::cos(r.y);
            ry[0][0] = cosy;
            ry[0][2] = siny;
            ry[2][0] = -siny;
            ry[2][2] = cosy;
            ry[1][1] = axis.y;
        }

        mat4_t<float> rz;
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

    mat4_t<float> mat4_model(const vec3_t<float>& translation, const vec3_t<radians_t>& rotation, const vec3_t<float>& scalar) {
        mat4_t<float> m;
        translate(m, translation);
        rotate(m, rotation, { 1, 1, 1 });
        scale(m, scalar);
        return m;
    }

    mat4_t<float> mat4_model(const vec3_t<float>& translation, const quat_t& rotation, const vec3_t<float>& scalar) {
        mat4_t<float> m;
        translate(m, translation);
        m = m * mat4_t<float>(rotation);
        scale(m, scalar);
        return m;
    }

    mat4_t<float> mat4_rigid(const vec3_t<float>& translation, const vec3_t<radians_t>& rotation) {
        mat4_t<float> m;
        translate(m, translation);
        rotate(m, rotation, { 1, 1, 1 });
        return m;
    }

    inline mat4_t<float> mat4_rigid(const vec3_t<float>& translation, const quat_t& rotation) {
        mat4_t<float> m;
        translate(m, translation);
        m = m * mat4_t<float>(rotation);
        return m;
    }

    inline mat4_t<float> mat4_view(const vec3_t<float>& position, const vec3_t<float>& front, const vec3_t<float>& up) {
        vec3_t<float> right = normalize(cross(front, up));
        return inverse_fast(transpose(mat4_t<float>(
                right,
                cross(right, front),
                -front,
                position
        )));
    }

    inline mat4_t<float> mat4_ortho(float left, float right, float bottom, float top, float z_near, float z_far) {
        return mat4_t<float> {
                { 2.0f / (right - left), 0.0f, 0.0f, 0.0f },
                { 0.0f, 2.0f / (bottom - top), 0.0f, 0.0f },
                { 0.0f, 0.0f, 1.0f / (z_near - z_far), 0.0f },
                { -(right + left) / (right - left), -(bottom + top) / (bottom - top), z_near / (z_near - z_far), 1.0f }
        };
    }

    inline mat4_t<float> mat4_perspective(float aspect, degree_t fov, float z_near, float z_far) {
        float f = 1.0f / tan(radians_t(0.5f * fov));
        return mat4_t<float> {
                { f / aspect, 0.0f, 0.0f, 0.0f },
                { 0.0f, -f, 0.0f, 0.0f },
                { 0.0f, 0.0f, z_far / (z_near - z_far), -1.0f },
                { 0.0f, 0.0f, z_near * z_far / (z_near - z_far), 0.0f }
        };
    }

    inline mat4_t<float> mat4_normal(const mat4_t<float>& model) {
        mat4_t<float> m = transpose(inverse(model));
        return m;
    }

    typedef vec2_t<float> float2_t;
    typedef vec3_t<float> float3_t;
    typedef vec4_t<float> float4_t;
    typedef vec2_t<degree_t> degree2_t;
    typedef vec3_t<degree_t> degree3_t;
    typedef vec4_t<degree_t> degree4_t;
    typedef vec2_t<radians_t> radians2_t;
    typedef vec3_t<radians_t> radians3_t;
    typedef vec4_t<radians_t> radians4_t;
    using float2x2_t = mat2_t<float>;
    using float3x3_t = mat3_t<float>;
    using float4x4_t = mat4_t<float>;

    typedef vec2_t<int> int2_t;
    typedef vec3_t<int> int3_t;
    typedef vec4_t<int> int4_t;
    using int2x2_t = mat2_t<int>;
    using int3x3_t = mat3_t<int>;
    using int4x4_t = mat4_t<int>;

    typedef vec2_t<double> double2_t;
    typedef vec3_t<double> double3_t;
    typedef vec4_t<double> double4_t;
    using double2x2_t = mat2_t<double>;
    using double3x3_t = mat3_t<double>;
    using double4x4_t = mat4_t<double>;

}