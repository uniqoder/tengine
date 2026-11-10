#pragma once

#include "matrix4.hpp"
#include "quaternion.hpp"
#include "vector3.hpp"

namespace tengine::math
{

struct Transform
{
    Vector3 position;
    Quaternion rotation;
    Vector3 scale = Vector3::one();

    // Matrix generation
    [[nodiscard]] Matrix4 matrix() const noexcept;

    // Coordinate transformation
    [[nodiscard]] Vector3 transform_point(const Vector3& point) const noexcept;

    [[nodiscard]] Vector3 transform_direction(const Vector3& direction) const noexcept;

    // Direction vectors
    [[nodiscard]] Vector3 right() const noexcept;

    [[nodiscard]] Vector3 up() const noexcept;

    [[nodiscard]] Vector3 forward() const noexcept;
}; // struct Transform

}; // namespace tengine::math
