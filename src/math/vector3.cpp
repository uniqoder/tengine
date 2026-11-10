#include "tengine/math/vector3.hpp"
#include "tengine/math/scalar.hpp"
#include <cassert>
#include <cmath>

namespace tengine::math
{

Scalar Vector3::length() const noexcept
{
    return std::sqrt(length_squared());
}

Vector3 Vector3::normalized() const noexcept
{
    assert(!is_zero());
    return *this / length();
}

void Vector3::normalize() noexcept
{
    assert(!is_zero());
    *this /= length();
}

Scalar distance(const Vector3& vec1, const Vector3& vec2) noexcept
{
    return std::sqrt(square(vec2.x - vec1.x) + square(vec2.y - vec1.y) + square(vec2.z - vec1.z));
}

Scalar angle(const Vector3& vec1, const Vector3& vec2) noexcept
{
    return std::acos(dot(vec1, vec2) / vec1.length() * vec2.length());
}

Vector3 lerp(const Vector3& start, const Vector3& end, Scalar prog) noexcept
{
    return start + (end - start) * prog;
}

Vector3 cross(const Vector3& vec1, const Vector3& vec2) noexcept
{
    return {
        vec1.y * vec2.z - vec2.y * vec1.z,
        vec1.z * vec2.x - vec2.z * vec1.x,
        vec1.x * vec2.y - vec2.x * vec1.y,
    };
}

Vector3 reflect(const Vector3& incident, const Vector3& normal) noexcept
{
    const Vector3 normal_hat = normal.normalized();
    return dot(incident, normal_hat) * normal_hat;
}

}; // namespace tengine::math
