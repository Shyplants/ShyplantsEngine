#pragma once

#include "Common/Types.h"
#include <algorithm>

constexpr int32 iabs(int32 v) noexcept {
    return v < 0 ? -v : v;
}

// -----------------------------------------------------------------------------
// IVec2 : Integer 2D Vector
// -----------------------------------------------------------------------------
struct IVec2
{
    int32 x = 0;
    int32 y = 0;

    constexpr IVec2() noexcept = default;
    constexpr IVec2(int32 x_, int32 y_) noexcept : x(x_), y(y_) {}

    // --- Basic arithmetic -----------------------------------------------------
    constexpr IVec2 operator+(const IVec2& rhs) const noexcept { return { x + rhs.x, y + rhs.y }; }
    constexpr IVec2 operator-(const IVec2& rhs) const noexcept { return { x - rhs.x, y - rhs.y }; }
    constexpr IVec2 operator*(int32 s) const noexcept { return { x * s, y * s }; }
    constexpr IVec2 operator/(int32 s) const noexcept { return { x / s, y / s }; }

    constexpr IVec2& operator+=(const IVec2& rhs) noexcept { x += rhs.x; y += rhs.y; return *this; }
    constexpr IVec2& operator-=(const IVec2& rhs) noexcept { x -= rhs.x; y -= rhs.y; return *this; }
    constexpr IVec2& operator*=(int32 s) noexcept { x *= s; y *= s; return *this; }
    constexpr IVec2& operator/=(int32 s) noexcept { x /= s; y /= s; return *this; }

    // --- Comparison ------------------------------------------------------------
    constexpr bool operator==(const IVec2& rhs) const noexcept { return x == rhs.x && y == rhs.y; }
    constexpr bool operator!=(const IVec2& rhs) const noexcept { return !(*this == rhs); }

    // --- Utility ---------------------------------------------------------------
    constexpr int32 ManhattanLength() const noexcept { return iabs(x) + iabs(y); }

    static constexpr IVec2 Zero() noexcept { return { 0, 0 }; }
    static constexpr IVec2 One() noexcept { return { 1, 1 }; }
    static constexpr IVec2 Up() noexcept { return { 0, 1 }; }
    static constexpr IVec2 Down() noexcept { return { 0, -1 }; }
    static constexpr IVec2 Left() noexcept { return { -1, 0 }; }
    static constexpr IVec2 Right() noexcept { return { 1, 0 }; }
};

// -----------------------------------------------------------------------------
// Vec2 : Float 2D Vector
// -----------------------------------------------------------------------------
struct Vec2
{
    float x = 0;
    float y = 0;

    constexpr Vec2() noexcept = default;
    constexpr Vec2(float x_, float y_) noexcept : x(x_), y(y_) {}

    // --- Basic arithmetic -----------------------------------------------------
    constexpr Vec2 operator+(const Vec2& rhs) const noexcept { return { x + rhs.x, y + rhs.y }; }
    constexpr Vec2 operator-(const Vec2& rhs) const noexcept { return { x - rhs.x, y - rhs.y }; }
    constexpr Vec2 operator*(float s) const noexcept { return { x * s, y * s }; }
    constexpr Vec2 operator/(float s) const noexcept { return { x / s, y / s }; }

    constexpr Vec2& operator+=(const Vec2& rhs) noexcept { x += rhs.x; y += rhs.y; return *this; }
    constexpr Vec2& operator-=(const Vec2& rhs) noexcept { x -= rhs.x; y -= rhs.y; return *this; }
    constexpr Vec2& operator*=(float s) noexcept { x *= s; y *= s; return *this; }
    constexpr Vec2& operator/=(float s) noexcept { x /= s; y /= s; return *this; }

    // --- Comparison ------------------------------------------------------------
    constexpr bool operator==(const Vec2& rhs) const noexcept { return x == rhs.x && y == rhs.y; }
    constexpr bool operator!=(const Vec2& rhs) const noexcept { return !(*this == rhs); }

    // --- Utility ---------------------------------------------------------------
    static constexpr Vec2 Zero() noexcept { return { 0, 0 }; }
    static constexpr Vec2 One() noexcept { return { 1, 1 }; }
};


// -----------------------------------------------------------------------------
// IVec3 : Integer 3D Vector
// -----------------------------------------------------------------------------
struct IVec3
{
    int32 x = 0;
    int32 y = 0;
    int32 z = 0;

    constexpr IVec3() noexcept = default;
    constexpr IVec3(int32 x_, int32 y_, int32 z_) noexcept : x(x_), y(y_), z(z_) {}

    // --- Basic arithmetic -----------------------------------------------------
    constexpr IVec3 operator+(const IVec3& rhs) const noexcept { return { x + rhs.x, y + rhs.y, z + rhs.z }; }
    constexpr IVec3 operator-(const IVec3& rhs) const noexcept { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
    constexpr IVec3 operator*(int32 s) const noexcept { return { x * s, y * s, z * s }; }
    constexpr IVec3 operator/(int32 s) const noexcept { return { x / s, y / s, z / s }; }

    constexpr IVec3& operator+=(const IVec3& rhs) noexcept { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
    constexpr IVec3& operator-=(const IVec3& rhs) noexcept { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
    constexpr IVec3& operator*=(int32 s) noexcept { x *= s; y *= s; z *= s; return *this; }
    constexpr IVec3& operator/=(int32 s) noexcept { x /= s; y /= s; z /= s; return *this; }

    // --- Comparison ------------------------------------------------------------
    constexpr bool operator==(const IVec3& rhs) const noexcept { return x == rhs.x && y == rhs.y && z == rhs.z; }
    constexpr bool operator!=(const IVec3& rhs) const noexcept { return !(*this == rhs); }

    // --- Utility ---------------------------------------------------------------
    constexpr int32 ManhattanLength() const noexcept { return iabs(x) + iabs(y) + iabs(z); }

    static constexpr IVec3 Zero() noexcept { return { 0, 0, 0 }; }
    static constexpr IVec3 One() noexcept { return { 1, 1, 1 }; }
};


// -----------------------------------------------------------------------------
// Global helpers (constexpr)
// -----------------------------------------------------------------------------

constexpr IVec2 Clamp(const IVec2& v, const IVec2& min, const IVec2& max)
{
    return {
        std::clamp(v.x, min.x, max.x),
        std::clamp(v.y, min.y, max.y)
    };
}

constexpr IVec3 Clamp(const IVec3& v, const IVec3& min, const IVec3& max)
{
    return {
        std::clamp(v.x, min.x, max.x),
        std::clamp(v.y, min.y, max.y),
        std::clamp(v.z, min.z, max.z)
    };
}
