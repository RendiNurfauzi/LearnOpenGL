#ifndef ENGINE_MATH_H
#define ENGINE_MATH_H

#undef near
#undef far
#pragma once
#include <cmath>

// Set ke 1 jika ingin menggunakan koordinat Vulkan (Y-Down, Depth 0 to 1)
#define ENGINE_USE_VULKAN 0 

namespace EngineMath {

    const float PI = 3.14159265359f;

    // =======================================================
    // Utilities & Conversions
    // =======================================================
    inline float radians(float deg) { return deg * PI / 180.0f; }
    inline float degrees(float rad) { return rad * 180.0f / PI; }

    // =======================================================
    // Vec1 (Scalar Wrapper - Berguna untuk Generic Programming)
    // =======================================================
    struct Vec1 {
        float x;
        Vec1() : x(0) {}
        Vec1(float x) : x(x) {}
        operator float() const { return x; } // Implicit conversion ke float
    };

    // =======================================================
    // Vec2 (Vector 2D - Untuk UV, UI, Screen Space)
    // =======================================================
    struct Vec2 {
        float x, y;

        Vec2() : x(0), y(0) {}
        Vec2(float x, float y) : x(x), y(y) {}
        Vec2(float v) : x(v), y(v) {}

        Vec2 operator-() const { return { -x, -y }; }
        Vec2 operator+(const Vec2& o) const { return { x + o.x, y + o.y }; }
        Vec2 operator-(const Vec2& o) const { return { x - o.x, y - o.y }; }
        Vec2 operator*(float s) const { return { x * s, y * s }; }
        Vec2 operator/(float s) const { return { x / s, y / s }; }

        float dot(const Vec2& o) const { return x * o.x + y * o.y; }
        float length() const { return std::sqrt(x * x + y * y); }
        Vec2 normalize() const {
            float len = length();
            return (len > 0) ? *this * (1.0f / len) : Vec2(0, 0);
        }
    };

    // =======================================================
    // Vec3 (Vector 3D)
    // =======================================================
    struct Vec3 {
        float x, y, z;

        Vec3() : x(0), y(0), z(0) {}
        Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
        Vec3(float v) : x(v), y(v), z(v) {}

        // Negasi & Compound
        Vec3 operator-() const { return { -x, -y, -z }; }
        Vec3& operator+=(const Vec3& o) { x += o.x; y += o.y; z += o.z; return *this; }

        // Basic Math Operators
        Vec3 operator+(const Vec3& o) const { return { x + o.x, y + o.y, z + o.z }; }
        Vec3 operator-(const Vec3& o) const { return { x - o.x, y - o.y, z - o.z }; }
        Vec3 operator*(const Vec3& o) const { return { x * o.x, y * o.y, z * o.z }; }
        Vec3 operator*(float s) const { return { x * s, y * s, z * s }; }
        Vec3 operator/(float s) const { return { x / s, y / s, z / s }; }

        // Vector Algebra
        float dot(const Vec3& o) const { return x * o.x + y * o.y + z * o.z; }
        Vec3 cross(const Vec3& o) const {
            return { y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x };
        }

        float length() const { return std::sqrt(x * x + y * y + z * z); }
        Vec3 normalize() const {
            float len = length();
            return (len > 0) ? *this * (1.0f / len) : Vec3(0, 0, 0);
        }

        static Vec3 lerp(const Vec3& a, const Vec3& b, float t) {
            return a + (b - a) * t;
        }
    };

    // =======================================================
    // Vec4 (Vector 4D - Untuk RGBA Color, Homogeneous Coord)
    // =======================================================
    struct Vec4 {
        float x, y, z, w;

        Vec4() : x(0), y(0), z(0), w(0) {}
        Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
        Vec4(const Vec3& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {} // Vec3 ke Vec4
        Vec4(float v) : x(v), y(v), z(v), w(v) {}

        Vec4 operator+(const Vec4& o) const { return { x + o.x, y + o.y, z + o.z, w + o.w }; }
        Vec4 operator-(const Vec4& o) const { return { x - o.x, y - o.y, z - o.z, w - o.w }; }
        Vec4 operator*(float s) const { return { x * s, y * s, z * s, w * s }; }

        float dot(const Vec4& o) const { return x * o.x + y * o.y + z * o.z + w * o.w; }

        float length() const { return std::sqrt(x * x + y * y + z * z + w * w); }
        Vec4 normalize() const {
            float len = length();
            return (len > 0) ? *this * (1.0f / len) : Vec4(0, 0, 0, 0);
        }

        // Helper untuk warna (RGBA)
        static Vec4 Color(float r, float g, float b, float a = 1.0f) {
            return { r, g, b, a };
        }
    };

    // =======================================================
    // Quaternion (Rotation)
    // =======================================================
    struct Quat {
        float x, y, z, w;

        Quat() : x(0), y(0), z(0), w(1) {}
        Quat(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        Quat normalize() const {
            float len = std::sqrt(x * x + y * y + z * z + w * w);
            return { x / len, y / len, z / len, w / len };
        }

        Quat operator*(const Quat& q) const {
            return {
                w * q.x + x * q.w + y * q.z - z * q.y,
                w * q.y - x * q.z + y * q.w + z * q.x,
                w * q.z + x * q.y - y * q.x + z * q.w,
                w * q.w - x * q.x - y * q.y - z * q.z
            };
        }

        // Gameplay: Euler (Degrees) to Quaternion
        static Quat fromEuler(const Vec3& eulerDegrees) {
            float ex = radians(eulerDegrees.x) * 0.5f;
            float ey = radians(eulerDegrees.y) * 0.5f;
            float ez = radians(eulerDegrees.z) * 0.5f;

            float cx = std::cos(ex); float sx = std::sin(ex);
            float cy = std::cos(ey); float sy = std::sin(ey);
            float cz = std::cos(ez); float sz = std::sin(ez);

            return {
                sx * cy * cz - cx * sy * sz,
                cx * sy * cz + sx * cy * sz,
                cx * cy * sz - sx * sy * cz,
                cx * cy * cz + sx * sy * sz
            };
        }

        // Quaternion to Euler (Degrees)
        Vec3 toEuler() const {
            Vec3 res;
            // Roll (x)
            float sinr_cosp = 2 * (w * x + y * z);
            float cosr_cosp = 1 - 2 * (x * x + y * y);
            res.x = degrees(std::atan2(sinr_cosp, cosr_cosp));
            // Pitch (y)
            float sinp = 2 * (w * y - z * x);
            if (std::abs(sinp) >= 1) res.y = degrees(std::copysign(PI / 2, sinp));
            else res.y = degrees(std::asin(sinp));
            // Yaw (z)
            float siny_cosp = 2 * (w * z + x * y);
            float cosy_cosp = 1 - 2 * (y * y + z * z);
            res.z = degrees(std::atan2(siny_cosp, cosy_cosp));
            return res;
        }
    };

    // =======================================================
    // Mat4 (Column Major)
    // =======================================================
    struct Mat4 {
        float m[4][4] = { 0 };

        static Mat4 identity() {
            Mat4 mat;
            for (int i = 0; i < 4; i++) mat.m[i][i] = 1.0f;
            return mat;
        }

        // Fungsi konversi internal agar rotate bisa menggunakannya
        static Mat4 fromQuat(const Quat& q) {
            Mat4 mat = identity();
            float xx = q.x * q.x, yy = q.y * q.y, zz = q.z * q.z;
            float xy = q.x * q.y, xz = q.x * q.z, yz = q.y * q.z;
            float wx = q.w * q.x, wy = q.w * q.y, wz = q.w * q.z;
            mat.m[0][0] = 1.0f - 2.0f * (yy + zz); mat.m[0][1] = 2.0f * (xy + wz); mat.m[0][2] = 2.0f * (xz - wy);
            mat.m[1][0] = 2.0f * (xy - wz); mat.m[1][1] = 1.0f - 2.0f * (xx + zz); mat.m[1][2] = 2.0f * (yz + wx);
            mat.m[2][0] = 2.0f * (xz + wy); mat.m[2][1] = 2.0f * (yz - wx); mat.m[2][2] = 1.0f - 2.0f * (xx + yy);
            return mat;
        }

        // Tambahkan ini di dalam struct Mat4 { ... }
        static Mat4 rotate(const Mat4& matrix, const Quat& q) {
            return matrix * fromQuat(q);
        }

        // Transformation Matrix
        static Mat4 translate(const Mat4& matrix, const Vec3& v) {
            Mat4 res = matrix;
            // Langsung hitung posisi baru tanpa perkalian matriks penuh
            res.m[3][0] = matrix.m[0][0] * v.x + matrix.m[1][0] * v.y + matrix.m[2][0] * v.z + matrix.m[3][0];
            res.m[3][1] = matrix.m[0][1] * v.x + matrix.m[1][1] * v.y + matrix.m[2][1] * v.z + matrix.m[3][1];
            res.m[3][2] = matrix.m[0][2] * v.x + matrix.m[1][2] * v.y + matrix.m[2][2] * v.z + matrix.m[3][2];
            return res;
        }

        static Mat4 scale(const Vec3& v) {
            Mat4 mat = identity();
            mat.m[0][0] = v.x; mat.m[1][1] = v.y; mat.m[2][2] = v.z;
            return mat;
        }

        // Projections (Versatile Vulkan/OpenGL)
        static Mat4 perspective(float fovRad, float aspect, float zNear, float zFar) {
            Mat4 mat;
            float t = std::tan(fovRad * 0.5f);
            mat.m[0][0] = 1.0f / (aspect * t);
            mat.m[1][1] = 1.0f / t;
            mat.m[2][3] = -1.0f;

#if ENGINE_USE_VULKAN
            mat.m[1][1] *= -1.0f;
            mat.m[2][2] = zFar / (zNear - zFar);
            mat.m[3][2] = -(zFar * zNear) / (zFar - zNear);
#else
            mat.m[2][2] = -(zFar + zNear) / (zFar - zNear);
            mat.m[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);
#endif
            return mat;
        }

        static Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
            Vec3 f = (center - eye).normalize();
            Vec3 s = f.cross(up).normalize();
            Vec3 u = s.cross(f);
            Mat4 mat = identity();
            mat.m[0][0] = s.x; mat.m[1][0] = s.y; mat.m[2][0] = s.z;
            mat.m[0][1] = u.x; mat.m[1][1] = u.y; mat.m[2][1] = u.z;
            mat.m[0][2] = -f.x; mat.m[1][2] = -f.y; mat.m[2][2] = -f.z;
            mat.m[3][0] = -s.dot(eye);
            mat.m[3][1] = -u.dot(eye);
            mat.m[3][2] = f.dot(eye);
            return mat;
        }

        // Matrix Multiplication
        Mat4 operator*(const Mat4& o) const {
            Mat4 r;
            for (int c = 0; c < 4; c++)
                for (int row = 0; row < 4; row++)
                    r.m[c][row] = m[0][row] * o.m[c][0] +
                    m[1][row] * o.m[c][1] +
                    m[2][row] * o.m[c][2] +
                    m[3][row] * o.m[c][3];
            return r;
        }
    };

    // Pointer helper untuk API Grafis
    inline const float* value_ptr(const Mat4& mat) {
        return &mat.m[0][0];
    }

} // namespace EngineMath

#endif