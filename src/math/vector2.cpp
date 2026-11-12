#include "tengine/math/vector2.hpp"
#include "tengine/math/scalar.hpp"
#include <cassert>
#include <cmath>

namespace tengine::math
{

Scalar Vector2::length() const noexcept
{
    return std::sqrt(length_squared());
}

Vector2 Vector2::normalized() const noexcept
{
    assert(!is_zero());
    return *this / length();
}

void Vector2::normalize() noexcept
{
    assert(!is_zero());
    *this /= length();
}

Scalar distance(const Vector2& a, const Vector2& b) noexcept
{
    return std::sqrt(square(b.x - a.x) + square(b.y - a.y));
}

Scalar angle(const Vector2& a, const Vector2& b) noexcept
{
    const Scalar cosine = dot(a, b) / (a.length() * b.length());
    return std::acos(clamp(cosine, -1.F, 1.F));
}

Vector2 lerp(const Vector2& a, const Vector2& b, Scalar t) noexcept
{
    return a + (b - a) * t;
}

}; // namespace tengine::math
