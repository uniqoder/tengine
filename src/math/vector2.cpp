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
    Vector2 diff = vec2 - vec1;
    return std::atan2(diff.y, diff.x);
}

Vector2 lerp(const Vector2& start, const Vector2& end, Scalar prog) noexcept
{
    Vector2 result = start;
    result.x = lerp(start.x, end.x, prog);
    result.y = lerp(start.y, end.y, prog);

    return result;
}

}; // namespace tengine::math
