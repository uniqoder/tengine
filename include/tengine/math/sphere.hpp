#pragma once

#include "scalar.hpp"
#include "vector3.hpp"

namespace tengine::math
{

struct AABB;
struct Ray;

struct Sphere
{
    Vector3 center;
    Scalar radius{1.F};

    constexpr Sphere() noexcept = default;

    constexpr Sphere(const Vector3& center, Scalar radius) noexcept : center(center), radius(radius)
    {
    }

    // Containment
    [[nodiscard]] bool contains(const Vector3& point) noexcept;

    // Intersection
    [[nodiscard]] bool intersects(const Sphere& other) const noexcept;

    [[nodiscard]] bool intersects(const AABB& box) const noexcept;

    [[nodiscard]] bool intersects(const Ray& ray, Scalar& hit_distance) const noexcept;
}; // struct Sphere

}; // namespace tengine::math
