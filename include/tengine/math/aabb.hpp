#pragma once

#include "scalar.hpp"
#include "vector3.hpp"

namespace tengine::math
{

struct Sphere;
struct Ray;

struct AABB
{
    Vector3 min;
    Vector3 max;

    constexpr AABB() noexcept = default;

    constexpr AABB(const Vector3& min, const Vector3& max) noexcept : min(min), max(max) {}

    // Properties
    [[nodiscard]] Vector3 center() const noexcept;

    [[nodiscard]] Vector3 size() const noexcept;

    [[nodiscard]] Vector3 extents() const noexcept;

    // Containment
    [[nodiscard]] bool contains(const Vector3& point) const noexcept;

    // Intersection
    [[nodiscard]] bool intersects(const AABB& other) const noexcept;

    [[nodiscard]] bool intersects(const Sphere& sphere) const noexcept;

    [[nodiscard]] bool intersects(const Ray& ray, Scalar& hit_distance) const noexcept;

    // Modification
    [[nodiscard]] AABB expanded(Scalar amount) const noexcept;
}; // struct AABB

}; // namespace tengine::math
