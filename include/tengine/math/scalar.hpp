#pragma once

#include <cmath>
#include <limits>

namespace tengine::math
{

using Scalar = float;

// Constants
inline constexpr Scalar Pi = 3.14159265358979323846f;
inline constexpr Scalar TwoPi = 2.F * Pi;
inline constexpr Scalar HalfPi = 0.5F * Pi;
inline constexpr Scalar Epsilon = 1.0e-6F;
inline constexpr Scalar Infinity = std::numeric_limits<Scalar>::infinity();

// Angle conversion
[[nodiscard]] constexpr Scalar radians(Scalar degrees) noexcept
{
    const Scalar half = 180.F;
    return degrees * (Pi / half);
}

[[nodiscard]] constexpr Scalar degrees(Scalar radians) noexcept
{
    const Scalar half = 180.F;
    return radians * (half / Pi);
}

// Basic operations
[[nodiscard]] constexpr Scalar square(Scalar value) noexcept
{
    return value * value;
}

[[nodiscard]] constexpr Scalar absolute(Scalar value) noexcept
{
    return value < 0.F ? -value : value;
}

[[nodiscard]] constexpr Scalar clamp(Scalar value, Scalar min, Scalar max) noexcept
{
    return value < min ? min : value > max ? max : value;
}

[[nodiscard]] constexpr Scalar saturate(Scalar value) noexcept
{
    return clamp(value, 0.F, 1.F);
}

[[nodiscard]] constexpr Scalar lerp(Scalar start, Scalar end, Scalar prog) noexcept
{
    return start + (end - start) * prog;
}

[[nodiscard]] constexpr bool nearly_equal(Scalar val1, Scalar val2,
                                          Scalar epsilon = Epsilon) noexcept
{
    return absolute(val1 - val2) <= epsilon;
}

// Other utilities
[[nodiscard]] constexpr Scalar inverse_sqrt(Scalar value) noexcept
{
    return 1.F / std::sqrt(value);
}

}; // namespace tengine::math
