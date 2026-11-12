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

Scalar distance(const Vector3& a, const Vector3& b) noexcept
{
    return std::sqrt(square(b.x - a.x) + square(b.y - a.y) + square(b.z - a.z));
}

Scalar angle(const Vector3& a, const Vector3& b) noexcept
{
    const Scalar cosine = dot(a, b) / (a.length() * b.length());
    return std::acos(clamp(cosine, -1.F, 1.F));
}

Vector3 lerp(const Vector3& a, const Vector3& b, Scalar t) noexcept
{
    return a + (b - a) * t;
}

Vector3 cross(const Vector3& a, const Vector3& b) noexcept
{
    return {
        a.y * b.z - b.y * a.z,
        a.z * b.x - b.z * a.x,
        a.x * b.y - b.x * a.y,
    };
}

Vector3 reflect(const Vector3& incident, const Vector3& normal) noexcept
{
    const Vector3 normal_hat = normal.normalized();
    return incident - 2.F * dot(incident, normal_hat) * normal_hat;
}

}; // namespace tengine::math
