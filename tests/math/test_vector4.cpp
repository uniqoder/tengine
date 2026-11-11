#include <catch2/catch_test_macros.hpp>

#include "tengine/math/scalar.hpp"
#include "tengine/math/vector3.hpp"
#include "tengine/math/vector4.hpp"

#include "test_helpers.hpp"

using namespace tengine::math;
using namespace tengine::test;

TEST_CASE("Vector4 construction and component access", "[math][vector4]")
{
    SECTION("default and component constructors")
    {
        const Vector4 defaulted;
        require_equal(defaulted, Vector4{0.F, 0.F, 0.F, 0.F});

        const Vector4 uniform(3.F);
        require_equal(uniform, Vector4{3.F, 3.F, 3.F, 3.F});

        const Vector4 from_xyz(Vector3{1.F, 2.F, 3.F}, 4.F);
        require_equal(from_xyz, Vector4{1.F, 2.F, 3.F, 4.F});

        const Vector4 vector{1.F, 2.F, 3.F, 4.F};
        REQUIRE(vector.x == 1.F);
        REQUIRE(vector.y == 2.F);
        REQUIRE(vector.z == 3.F);
        REQUIRE(vector.w == 4.F);
    }

    SECTION("component projections")
    {
        const Vector4 vector{1.F, 2.F, 3.F, 4.F};
        require_equal(vector.xyz(), Vector3{1.F, 2.F, 3.F});
    }

    SECTION("subscript access")
    {
        Vector4 vector{1.F, 2.F, 3.F, 4.F};
        REQUIRE(vector[0] == 1.F);
        REQUIRE(vector[1] == 2.F);
        REQUIRE(vector[2] == 3.F);
        REQUIRE(vector[3] == 4.F);

        vector[0] = 5.F;
        vector[1] = 6.F;
        vector[2] = 7.F;
        vector[3] = 8.F;
        require_equal(vector, Vector4{5.F, 6.F, 7.F, 8.F});

        const Vector4 const_vector{9.F, 10.F, 11.F, 12.F};
        REQUIRE(const_vector[0] == 9.F);
        REQUIRE(const_vector[1] == 10.F);
        REQUIRE(const_vector[2] == 11.F);
        REQUIRE(const_vector[3] == 12.F);
    }

    SECTION("common vectors")
    {
        require_equal(Vector4::zero(), Vector4{0.F, 0.F, 0.F, 0.F});
        require_equal(Vector4::one(), Vector4{1.F, 1.F, 1.F, 1.F});
    }

    SECTION("equality")
    {
        REQUIRE(Vector4{1.F, 2.F, 3.F, 4.F} == Vector4{1.F, 2.F, 3.F, 4.F});
        REQUIRE_FALSE(Vector4{1.F, 2.F, 3.F, 4.F} == Vector4{4.F, 3.F, 2.F, 1.F});
    }
}

TEST_CASE("Vector4 arithmetic", "[math][vector4]")
{
    SECTION("unary operators")
    {
        const Vector4 vector{1.F, -2.F, 3.F, -4.F};
        REQUIRE(+vector == vector);
        require_equal(-vector, Vector4{-1.F, 2.F, -3.F, 4.F});
    }

    SECTION("compound assignment")
    {
        Vector4 vector{1.F, 2.F, 3.F, 4.F};

        vector += Vector4{4.F, 5.F, 6.F, 7.F};
        require_equal(vector, Vector4{5.F, 7.F, 9.F, 11.F});

        vector -= Vector4{1.F, 2.F, 3.F, 4.F};
        require_equal(vector, Vector4{4.F, 5.F, 6.F, 7.F});

        vector *= 2.F;
        require_equal(vector, Vector4{8.F, 10.F, 12.F, 14.F});

        vector /= 2.F;
        require_equal(vector, Vector4{4.F, 5.F, 6.F, 7.F});
    }

    SECTION("free operators")
    {
        require_equal(Vector4{1.F, 2.F, 3.F, 4.F} + Vector4{4.F, 5.F, 6.F, 7.F},
                      Vector4{5.F, 7.F, 9.F, 11.F});
        require_equal(Vector4{4.F, 5.F, 6.F, 7.F} - Vector4{1.F, 2.F, 3.F, 4.F},
                      Vector4{3.F, 3.F, 3.F, 3.F});
        require_equal(Vector4{1.F, 2.F, 3.F, 4.F} * 2.F, Vector4{2.F, 4.F, 6.F, 8.F});
        require_equal(2.F * Vector4{1.F, 2.F, 3.F, 4.F}, Vector4{2.F, 4.F, 6.F, 8.F});
        require_equal(Vector4{2.F, 4.F, 6.F, 8.F} / 2.F, Vector4{1.F, 2.F, 3.F, 4.F});
    }
}

TEST_CASE("Vector4 magnitude and normalization", "[math][vector4]")
{
    const Vector4 vector{1.F, 2.F, 2.F, 4.F};

    REQUIRE(vector.length_squared() == 25.F);
    REQUIRE(vector.length() == approx(5.F));

    const Vector4 normalized = vector.normalized();
    require_equal(normalized, Vector4{0.2F, 0.4F, 0.4F, 0.8F});
    REQUIRE(normalized.length() == approx(1.F));

    Vector4 mutable_vector{1.F, 2.F, 2.F, 4.F};
    mutable_vector.normalize();
    require_equal(mutable_vector, normalized);
}

TEST_CASE("Vector4 is_zero", "[math][vector4]")
{
    REQUIRE(Vector4::zero().is_zero());
    REQUIRE_FALSE(Vector4{0.F, 0.F, 0.F, 1.F}.is_zero());
    REQUIRE(Vector4{0.F, 0.F, 1.0e-4F, 0.F}.is_zero(1.0e-3F));
    REQUIRE_FALSE(Vector4{0.F, 0.F, 1.0e-4F, 0.F}.is_zero());
}

TEST_CASE("Vector4 dot", "[math][vector4]")
{
    REQUIRE(dot(Vector4{1.F, 2.F, 3.F, 4.F}, Vector4{5.F, 6.F, 7.F, 8.F}) == 70.F);
    REQUIRE(dot(Vector4{1.F, 0.F, 0.F, 0.F}, Vector4{0.F, 1.F, 0.F, 0.F}) == 0.F);
}
