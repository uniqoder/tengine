#pragma once

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "tengine/math/matrix3.hpp"
#include "tengine/math/matrix4.hpp"
#include "tengine/math/quaternion.hpp"
#include "tengine/math/scalar.hpp"
#include "tengine/math/vector2.hpp"
#include "tengine/math/vector3.hpp"
#include "tengine/math/vector4.hpp"

namespace tengine::test
{

inline Catch::Approx approx(math::Scalar value)
{
    return Catch::Approx(value).margin(1.0e-5).epsilon(1.0e-5);
}

inline void require_equal(const math::Vector2& actual, const math::Vector2& expected)
{
    REQUIRE(actual.x == approx(expected.x));
    REQUIRE(actual.y == approx(expected.y));
}

inline void require_equal(const math::Vector3& actual, const math::Vector3& expected)
{
    REQUIRE(actual.x == approx(expected.x));
    REQUIRE(actual.y == approx(expected.y));
    REQUIRE(actual.z == approx(expected.z));
}

inline void require_equal(const math::Vector4& actual, const math::Vector4& expected)
{
    REQUIRE(actual.x == approx(expected.x));
    REQUIRE(actual.y == approx(expected.y));
    REQUIRE(actual.z == approx(expected.z));
    REQUIRE(actual.w == approx(expected.w));
}

inline void require_equal(const math::Matrix3& actual, const math::Matrix3& expected)
{
    for (int row = 0; row < 3; ++row)
    {
        for (int column = 0; column < 3; ++column)
        {
            REQUIRE(actual(row, column) == approx(expected(row, column)));
        }
    }
}

inline void require_equal(const math::Matrix4& actual, const math::Matrix4& expected)
{
    for (int row = 0; row < 4; ++row)
    {
        for (int column = 0; column < 4; ++column)
        {
            REQUIRE(actual(row, column) == approx(expected(row, column)));
        }
    }
}

inline void require_equal(const math::Quaternion& actual, const math::Quaternion& expected)
{
    REQUIRE(actual.w == approx(expected.w));
    REQUIRE(actual.x == approx(expected.x));
    REQUIRE(actual.y == approx(expected.y));
    REQUIRE(actual.z == approx(expected.z));
}

// Quaternions q and -q describe the same rotation, so compare their rotations
// instead of their components.
inline void require_same_rotation(const math::Quaternion& actual, const math::Quaternion& expected)
{
    const math::Scalar cosine = math::dot(actual.normalized(), expected.normalized());
    REQUIRE(math::absolute(cosine) == approx(1.F));
}

} // namespace tengine::test
