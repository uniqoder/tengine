#pragma once

#include "scalar.hpp"
#include "vector3.hpp"

namespace tengine::math
{

struct Plane
{
    Vector3 normal = Vector3::up();
    Scalar distance{0.F};

    constexpr Plane() noexcept = default;

    constexpr Plane(const Vector3& normal, Scalar distance) noexcept
        : normal(normal), distance(distance)
    {
    }

    [[nodiscard]] static Plane from_point_normal(const Vector3& point,
                                                 const Vector3& normal) noexcept;

    [[nodiscard]] Scalar signed_distance(const Vector3& point) const noexcept;

    [[nodiscard]] Vector3 project(const Vector3& point) const noexcept;
}; // struct Plane

}; // namespace tengine::math
