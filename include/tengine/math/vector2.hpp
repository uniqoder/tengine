#pragma once

#include "scalar.hpp"

namespace tengine::math
{

struct Vector2
{
    Scalar x{0.F};
    Scalar y{0.F};

    constexpr Vector2() noexcept = default;

    constexpr explicit Vector2(Scalar value) noexcept : x(value), y(value) {}

    constexpr Vector2(Scalar x, Scalar y) noexcept : x(x), y(y) {}

    // Component access
    [[nodiscard]] constexpr Scalar& operator[](int index) noexcept
    {
        return index == 0 ? x : y;
    }

    [[nodiscard]] constexpr const Scalar& operator[](int index) const noexcept
    {
        return index == 0 ? x : y;
    }

    // Arithmatic
    [[nodiscard]] constexpr Vector2 operator+() const noexcept
    {
        return *this;
    }

    [[nodiscard]] constexpr Vector2 operator-() const noexcept
    {
        return {-x, -y};
    }

    constexpr Vector2& operator+=(const Vector2& rhs) noexcept
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    constexpr Vector2& operator-=(const Vector2& rhs) noexcept
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    constexpr Vector2& operator*=(Scalar rhs) noexcept
    {
        x *= rhs;
        y *= rhs;
        return *this;
    }

    constexpr Vector2& operator/=(Scalar rhs) noexcept
    {
        x /= rhs;
        y /= rhs;
        return *this;
    }

    // Magnitude
    [[nodiscard]] Scalar length() const noexcept;

    [[nodiscard]] constexpr Scalar length_squared() const noexcept
    {
        return square(x) + square(y);
    }

    [[nodiscard]] Vector2 normalized() const noexcept;

    void normalize() noexcept;

    [[nodiscard]] constexpr bool is_zero(Scalar epsilon = Epsilon) const noexcept
    {
        return nearly_equal(length_squared(), 0, epsilon * epsilon);
    }

    // Common vectors
    [[nodiscard]] static constexpr Vector2 zero() noexcept
    {
        return {0.F, 0.F};
    }

    [[nodiscard]] static constexpr Vector2 one() noexcept
    {
        return {1.F, 1.F};
    }

    [[nodiscard]] static constexpr Vector2 right() noexcept
    {
        return {1.F, 0.F};
    }

    [[nodiscard]] static constexpr Vector2 up() noexcept
    {
        return {0.F, 1.F};
    }

    friend constexpr bool operator==(const Vector2&, const Vector2&) noexcept = default;
}; // struct Vector2

// Free operators
[[nodiscard]] constexpr Vector2 operator+(Vector2 lhs, const Vector2& rhs) noexcept
{
    return lhs += rhs;
}

[[nodiscard]] constexpr Vector2 operator-(Vector2 lhs, const Vector2& rhs) noexcept
{
    return lhs -= rhs;
}

[[nodiscard]] constexpr Vector2 operator*(Vector2 lhs, Scalar rhs) noexcept
{
    return lhs *= rhs;
}

[[nodiscard]] constexpr Vector2 operator*(Scalar lhs, Vector2 rhs) noexcept
{
    return rhs *= lhs;
}

[[nodiscard]] constexpr Vector2 operator/(Vector2 lhs, Scalar rhs) noexcept
{
    return lhs /= rhs;
}

// Vector operations
[[nodiscard]] constexpr Scalar dot(const Vector2& a, const Vector2& b) noexcept
{
    return a.x * b.x + a.y * b.y;
}

[[nodiscard]] Scalar distance(const Vector2& a, const Vector2& b) noexcept;

[[nodiscard]] Scalar angle(const Vector2& a, const Vector2& b) noexcept;

[[nodiscard]] Vector2 lerp(const Vector2& a, const Vector2& b, Scalar t) noexcept;

using Vec2 = Vector2;

}; // namespace tengine::math
