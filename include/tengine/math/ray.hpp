#pragma once

#include "scalar.hpp"
#include "vector3.hpp"

namespace tengine::math
{

struct Ray
{
    Vector3 origin;
    Vector3 direction = Vector3::forward();

    constexpr Ray() noexcept = default;

    constexpr Ray(const Vector3& origin, const Vector3& direction) noexcept
        : origin(origin), direction(direction)
    {
    }

    [[nodiscard]] Vector3 at(Scalar distance) noexcept;
}; // struct Ray

}; // namespace tengine::math
