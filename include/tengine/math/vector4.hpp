#pragma once

#include "scalar.hpp"
#include "vector3.hpp"

namespace tengine::math
{

struct Vector4
{
    Scalar x{0.F};
    Scalar y{0.F};
    Scalar z{0.F};
    Scalar w{0.F};

    constexpr Vector4() noexcept = default;

    constexpr explicit Vector4(Scalar value) noexcept : x(value), y(value), z(value), w(value) {}

    constexpr Vector4(Scalar x, Scalar y, Scalar z, Scalar w) noexcept : x(x), y(y), z(z), w(w) {}

    constexpr Vector4(const Vector3& xyz, Scalar w) noexcept : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}

    // Component access
    [[nodiscard]] constexpr Vector3 xyz() const noexcept
    {
        return {x, y, z};
    }

    [[nodiscard]] constexpr Scalar& operator[](int index) noexcept
    {
        return index == 0 ? x : index == 1 ? y : index == 2 ? z : w;
    }

    [[nodiscard]] constexpr const Scalar& operator[](int index) const noexcept
    {
        return index == 0 ? x : index == 1 ? y : index == 2 ? z : w;
    }

    // Arithmatic
    [[nodiscard]] constexpr Vector4 operator+() const noexcept
    {
        return *this;
    }

    [[nodiscard]] constexpr Vector4 operator-() const noexcept
    {
        return {-x, -y, -z, -w};
    }

    constexpr Vector4& operator+=(const Vector4& rhs) noexcept
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }

    constexpr Vector4& operator-=(const Vector4& rhs) noexcept
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }

    constexpr Vector4& operator*=(Scalar rhs) noexcept
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        w *= rhs;
        return *this;
    }

    constexpr Vector4& operator/=(Scalar rhs) noexcept
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        w /= rhs;
        return *this;
    }

    // Magnitude
    [[nodiscard]] Scalar length() const noexcept;

    [[nodiscard]] constexpr Scalar length_squared() const noexcept
    {
        return square(x) + square(y) + square(z) + square(w);
    }

    [[nodiscard]] Vector4 normalized() const noexcept;

    void normalize() noexcept;

    [[nodiscard]] constexpr bool is_zero(Scalar epsilon = Epsilon) const noexcept
    {
        return length_squared() <= epsilon * epsilon;
    }

    // Common vectors
    [[nodiscard]] static constexpr Vector4 zero() noexcept
    {
        return {0.F, 0.F, 0.F, 0.F};
    }

    [[nodiscard]] static constexpr Vector4 one() noexcept
    {
        return {1.F, 1.F, 1.F, 1.F};
    }

    friend constexpr bool operator==(const Vector4&, const Vector4&) noexcept = default;
}; // struct Vector4

// Free operators
[[nodiscard]] constexpr Vector4 operator+(Vector4 lhs, const Vector4& rhs) noexcept
{
    return lhs += rhs;
}

[[nodiscard]] constexpr Vector4 operator-(Vector4 lhs, const Vector4& rhs) noexcept
{
    return lhs -= rhs;
}

[[nodiscard]] constexpr Vector4 operator*(Vector4 lhs, Scalar rhs) noexcept
{
    return lhs *= rhs;
}

[[nodiscard]] constexpr Vector4 operator*(Scalar lhs, Vector4 rhs) noexcept
{
    return rhs *= lhs;
}

[[nodiscard]] constexpr Vector4 operator/(Vector4 lhs, Scalar rhs) noexcept
{
    return lhs /= rhs;
}

// Vector operations
[[nodiscard]] constexpr Scalar dot(const Vector4& vec1, const Vector4& vec2) noexcept
{
    return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z + vec1.w * vec2.w;
}

using Vec4 = Vector4;

}; // namespace tengine::math
