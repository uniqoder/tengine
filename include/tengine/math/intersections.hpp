#pragma once

#include "aabb.hpp"
#include "plane.hpp"
#include "ray.hpp"
#include "scalar.hpp"
#include "sphere.hpp"

namespace tengine::math
{

// Ray intersections
[[nodiscard]] bool intersect(const Ray& ray, const Plane& plane, Scalar& hit_distance) noexcept;

[[nodiscard]] bool intersect(const Ray& ray, const Sphere& sphere, Scalar& hit_distance) noexcept;

[[nodiscard]] bool intersect(const Ray& ray, const AABB& box, Scalar& hit_distance) noexcept;

// Shape intersections
[[nodiscard]] bool intersect(const AABB& box1, const AABB& box2) noexcept;

[[nodiscard]] bool intersect(const Sphere& sphere1, const Sphere& sphere2) noexcept;

[[nodiscard]] bool intersect(const Sphere& sphere, const AABB& box) noexcept;

}; // namespace tengine::math
