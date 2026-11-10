#include "tengine/math/vector3.hpp"
#include "tengine/math/scalar.hpp"
#include <cmath>

namespace tengine::math
{

Scalar Vector3::length() const noexcept
{
    return std::sqrt(length_squared());
}

Vector3 Vector3::normalized() const noexcept
{
    return *this / length();
}

void Vector3::normalize() noexcept
{
    *this /= length();
}

Scalar distance(const Vector3& vec1, const Vector3& vec2) noexcept
{
    return std::sqrt(square(vec2.x - vec1.x) + square(vec2.y - vec1.y) + square(vec2.z - vec1.z));
}

Scalar angle(const Vector3& vec1, const Vector3& vec2) noexcept
{
    Vector3 diff = vec2 - vec1;
    return std::atan2(diff.y, diff.x);
}

Vector3 lerp(const Vector3& start, const Vector3& end, Scalar prog) noexcept
{
    Vector3 result = start;
    result.x = lerp(start.x, end.x, prog);
    result.y = lerp(start.y, end.y, prog);
    result.z = lerp(start.z, end.z, prog);

    return result;
}

Vector3 cross(const Vector3& vec1, const Vector3& vec2) noexcept
{
    return {
        vec1.y * vec2.z - vec2.y * vec1.z,
        vec1.x * vec2.z - vec2.x * vec1.z,
        vec1.x * vec2.y - vec2.x * vec1.y,
    };
}

Vector3 reflect(const Vector3& incident, const Vector3& normal) noexcept
{
    return cross(incident, normal) / normal.length();
}

}; // namespace tengine::math
