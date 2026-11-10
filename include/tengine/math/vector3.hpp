#pragma once

#include "scalar.hpp"
#include "vector2.hpp"

namespace tengine::math
{

struct Vector3
{
    Scalar x{0.F};
    Scalar y{0.F};
    Scalar z{0.F};

    constexpr Vector3() noexcept = default;

    constexpr explicit Vector3(Scalar value) noexcept : x(value), y(value), z(value) {}

    constexpr Vector3(const Vector2& xy, Scalar z) noexcept : x(xy.x), y(xy.y), z(z) {}

    constexpr Vector3(Scalar x, Scalar y, Scalar z) noexcept : x(x), y(y), z(z) {}

    // Component access
    [[nodiscard]] constexpr Vector2 xy() const noexcept
    {
        return {x, y};
    }

    [[nodiscard]] constexpr Scalar& operator[](int index) noexcept
    {
        return index == 0 ? x : index == 1 ? y : z;
    }

    [[nodiscard]] constexpr const Scalar& operator[](int index) const noexcept
    {
        return index == 0 ? x : index == 1 ? y : z;
    }

    // Arithmatic
    [[nodiscard]] constexpr Vector3 operator+() const noexcept
    {
        return *this;
    }

    [[nodiscard]] constexpr Vector3 operator-() const noexcept
    {
        return {-x, -y, -z};
    }

    [[nodiscard]] constexpr Vector3& operator+=(const Vector3& rhs) noexcept
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    [[nodiscard]] constexpr Vector3& operator-=(const Vector3& rhs) noexcept
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    [[nodiscard]] constexpr Vector3& operator*=(Scalar rhs) noexcept
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    [[nodiscard]] constexpr Vector3& operator/=(Scalar rhs) noexcept
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        return *this;
    }

    // Magnitude
    [[nodiscard]] Scalar length() const noexcept;

    [[nodiscard]] Scalar length_squared() const noexcept
    {
        return square(x) + square(y) + square(z);
    }

    [[nodiscard]] Vector3 normalized() const noexcept;

    void normalize() noexcept;

    [[nodiscard]] constexpr bool is_zero(Scalar epsilon = Epsilon) const noexcept
    {
        return length_squared() <= epsilon * epsilon;
    }

    // Common vectors
    [[nodiscard]] static constexpr Vector3 zero() noexcept
    {
        return {0.F, 0.F, 0.F};
    }

    [[nodiscard]] static constexpr Vector3 one() noexcept
    {
        return {1.F, 1.F, 1.F};
    }

    [[nodiscard]] static constexpr Vector3 right() noexcept
    {
        return {1.F, 0.F, 0.F};
    }

    [[nodiscard]] static constexpr Vector3 up() noexcept
    {
        return {0.F, 1.F, 0.F};
    }

    [[nodiscard]] static constexpr Vector3 forward() noexcept
    {
        return {0.F, 0.F, -1.F};
    }

    friend constexpr bool operator==(const Vector3&, const Vector3&) noexcept = default;
}; // struct Vector3

// Free operators
[[nodiscard]] constexpr Vector3 operator+(Vector3 lhs, Vector3& rhs) noexcept
{
    return lhs += rhs;
}

[[nodiscard]] constexpr Vector3 operator-(Vector3 lhs, Vector3& rhs) noexcept
{
    return lhs -= rhs;
}

[[nodiscard]] constexpr Vector3 operator*(Vector3 lhs, Scalar rhs) noexcept
{
    return lhs *= rhs;
}

[[nodiscard]] constexpr Vector3 operator*(Scalar lhs, Vector3 rhs) noexcept
{
    return rhs *= lhs;
}

[[nodiscard]] constexpr Vector3 operator/(Vector3 lhs, Scalar rhs) noexcept
{
    return lhs /= rhs;
}

// Vector operations
[[nodiscard]] constexpr Scalar dot(const Vector3& vec1, const Vector3& vec2) noexcept
{
    return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

[[nodiscard]] constexpr Vector3 cross(const Vector3& vec1, const Vector3& vec2) noexcept;

[[nodiscard]] constexpr Scalar distance(const Vector3& vec1, const Vector3& vec2) noexcept;

[[nodiscard]] constexpr Scalar angle(const Vector3& vec1, const Vector3& vec2) noexcept;

[[nodiscard]] constexpr Scalar lerp(const Vector3& start, const Vector3& end, Scalar prog) noexcept;

[[nodiscard]] constexpr Vector3 reflect(const Vector3& incident, const Vector3& normal) noexcept;

using Vec3 = Vector3;

}; // namespace tengine::math
