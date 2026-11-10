#include "tengine/math/vector2.hpp"
#include "tengine/math/scalar.hpp"
#include <cmath>

namespace tengine::math
{

Scalar Vector2::length() const noexcept
{
    return std::sqrt(length_squared());
}

Vector2 Vector2::normalized() const noexcept
{
    return *this / length();
}

void Vector2::normalize() noexcept
{
    *this /= length();
}

Scalar distance(const Vector2& vec1, const Vector2& vec2) noexcept
{
    return std::sqrt(square(vec2.x - vec1.x) + square(vec2.y - vec1.y));
}

Scalar angle(const Vector2& vec1, const Vector2& vec2) noexcept
{
    return std::acos(dot(vec1, vec2) / vec1.length() * vec2.length());
}

Vector2 lerp(const Vector2& start, const Vector2& end, Scalar prog) noexcept
{
    return start + (end - start) * prog;
}

}; // namespace tengine::math
