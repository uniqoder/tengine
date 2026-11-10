#include "tengine/math/vector4.hpp"
#include "tengine/math/scalar.hpp"
#include <cassert>
#include <cmath>

namespace tengine::math
{

Scalar Vector4::length() const noexcept
{
    return std::sqrt(length_squared());
}

Vector4 Vector4::normalized() const noexcept
{
    assert(!is_zero());
    return *this / length();
}

void Vector4::normalize() noexcept
{
    assert(!is_zero());
    *this /= length();
}

Scalar distance(const Vector4& vec1, const Vector4& vec2) noexcept
{
    return std::sqrt(square(vec2.x - vec1.x) + square(vec2.y - vec1.y) + square(vec2.z - vec1.z) +
                     square(vec2.w - vec1.w));
}

Vector4 lerp(const Vector4& start, const Vector4& end, Scalar prog) noexcept
{
    return start + (end - start) * prog;
}

}; // namespace tengine::math
