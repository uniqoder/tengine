#include <catch2/catch_test_macros.hpp>

#include "tengine/math/scalar.hpp"
#include "tengine/math/vector2.hpp"
#include "tengine/math/vector3.hpp"

#include "test_helpers.hpp"

using namespace tengine::math;
using namespace tengine::test;

TEST_CASE("Vector3 construction and component access", "[math][vector3]")
{
    SECTION("default and component constructors")
    {
        const Vector3 defaulted;
        require_equal(defaulted, Vector3{0.F, 0.F, 0.F});

        const Vector3 uniform(3.F);
        require_equal(uniform, Vector3{3.F, 3.F, 3.F});

        const Vector3 from_xy(Vector2{1.F, 2.F}, 3.F);
        require_equal(from_xy, Vector3{1.F, 2.F, 3.F});

        const Vector3 vector{1.F, 2.F, 3.F};
        REQUIRE(vector.x == 1.F);
        REQUIRE(vector.y == 2.F);
        REQUIRE(vector.z == 3.F);
    }

    SECTION("component projections")
    {
        const Vector3 vector{1.F, 2.F, 3.F};
        require_equal(vector.xy(), Vector2{1.F, 2.F});
    }

    SECTION("subscript access")
    {
        Vector3 vector{1.F, 2.F, 3.F};
        REQUIRE(vector[0] == 1.F);
        REQUIRE(vector[1] == 2.F);
        REQUIRE(vector[2] == 3.F);

        vector[0] = 4.F;
        vector[1] = 5.F;
        vector[2] = 6.F;
        require_equal(vector, Vector3{4.F, 5.F, 6.F});

        const Vector3 const_vector{7.F, 8.F, 9.F};
        REQUIRE(const_vector[0] == 7.F);
        REQUIRE(const_vector[1] == 8.F);
        REQUIRE(const_vector[2] == 9.F);
    }

    SECTION("common vectors")
    {
        require_equal(Vector3::zero(), Vector3{0.F, 0.F, 0.F});
        require_equal(Vector3::one(), Vector3{1.F, 1.F, 1.F});
        require_equal(Vector3::right(), Vector3{1.F, 0.F, 0.F});
        require_equal(Vector3::up(), Vector3{0.F, 1.F, 0.F});
        require_equal(Vector3::forward(), Vector3{0.F, 0.F, -1.F});
    }

    SECTION("equality")
    {
        REQUIRE(Vector3{1.F, 2.F, 3.F} == Vector3{1.F, 2.F, 3.F});
        REQUIRE_FALSE(Vector3{1.F, 2.F, 3.F} == Vector3{3.F, 2.F, 1.F});
    }
}

TEST_CASE("Vector3 arithmetic", "[math][vector3]")
{
    SECTION("unary operators")
    {
        const Vector3 vector{1.F, -2.F, 3.F};
        REQUIRE(+vector == vector);
        require_equal(-vector, Vector3{-1.F, 2.F, -3.F});
    }

    SECTION("compound assignment")
    {
        Vector3 vector{1.F, 2.F, 3.F};

        vector += Vector3{4.F, 5.F, 6.F};
        require_equal(vector, Vector3{5.F, 7.F, 9.F});

        vector -= Vector3{1.F, 2.F, 3.F};
        require_equal(vector, Vector3{4.F, 5.F, 6.F});

        vector *= 2.F;
        require_equal(vector, Vector3{8.F, 10.F, 12.F});

        vector /= 2.F;
        require_equal(vector, Vector3{4.F, 5.F, 6.F});
    }

    SECTION("free operators")
    {
        require_equal(Vector3{1.F, 2.F, 3.F} + Vector3{4.F, 5.F, 6.F}, Vector3{5.F, 7.F, 9.F});
        require_equal(Vector3{4.F, 5.F, 6.F} - Vector3{1.F, 2.F, 3.F}, Vector3{3.F, 3.F, 3.F});
        require_equal(Vector3{1.F, 2.F, 3.F} * 2.F, Vector3{2.F, 4.F, 6.F});
        require_equal(2.F * Vector3{1.F, 2.F, 3.F}, Vector3{2.F, 4.F, 6.F});
        require_equal(Vector3{2.F, 4.F, 6.F} / 2.F, Vector3{1.F, 2.F, 3.F});
    }
}

TEST_CASE("Vector3 magnitude and normalization", "[math][vector3]")
{
    const Vector3 vector{1.F, 2.F, 2.F};

    REQUIRE(vector.length_squared() == 9.F);
    REQUIRE(vector.length() == approx(3.F));

    const Vector3 normalized = vector.normalized();
    require_equal(normalized, Vector3{1.F / 3.F, 2.F / 3.F, 2.F / 3.F});
    REQUIRE(normalized.length() == approx(1.F));

    Vector3 mutable_vector{1.F, 2.F, 2.F};
    mutable_vector.normalize();
    require_equal(mutable_vector, normalized);
}

TEST_CASE("Vector3 is_zero", "[math][vector3]")
{
    REQUIRE(Vector3::zero().is_zero());
    REQUIRE_FALSE(Vector3{0.F, 0.F, 1.F}.is_zero());
    REQUIRE(Vector3{0.F, 1.0e-4F, 0.F}.is_zero(1.0e-3F));
    REQUIRE_FALSE(Vector3{0.F, 1.0e-4F, 0.F}.is_zero());
}

TEST_CASE("Vector3 vector operations", "[math][vector3]")
{
    SECTION("dot")
    {
        REQUIRE(dot(Vector3{1.F, 2.F, 3.F}, Vector3{4.F, 5.F, 6.F}) == 32.F);
        REQUIRE(dot(Vector3::right(), Vector3::up()) == 0.F);
    }

    SECTION("cross")
    {
        require_equal(cross(Vector3::right(), Vector3::up()), Vector3{0.F, 0.F, 1.F});
        require_equal(cross(Vector3::up(), Vector3::right()), Vector3{0.F, 0.F, -1.F});
        require_equal(cross(Vector3{1.F, 2.F, 3.F}, Vector3{4.F, 5.F, 6.F}),
                      Vector3{-3.F, 6.F, -3.F});
        REQUIRE(cross(Vector3{1.F, 2.F, 3.F}, Vector3{1.F, 2.F, 3.F}).is_zero());
    }

    SECTION("distance")
    {
        REQUIRE(distance(Vector3{0.F, 0.F, 0.F}, Vector3{1.F, 2.F, 2.F}) == approx(3.F));
        REQUIRE(distance(Vector3{1.F, 1.F, 1.F}, Vector3{1.F, 1.F, 1.F}) == 0.F);
    }

    SECTION("angle")
    {
        REQUIRE(angle(Vector3::right(), Vector3::up()) == approx(HalfPi));
        REQUIRE(angle(Vector3::right(), Vector3::right()) == approx(0.F));
        REQUIRE(angle(Vector3{1.F, 0.F, 0.F}, Vector3{1.F, 1.F, 0.F}) == approx(Pi / 4.F));
        REQUIRE(angle(Vector3{2.F, 0.F, 0.F}, Vector3{3.F, 0.F, 0.F}) == approx(0.F));
    }

    SECTION("lerp")
    {
        const Vector3 start{0.F, 0.F, 0.F};
        const Vector3 end{10.F, 20.F, 30.F};

        require_equal(lerp(start, end, 0.F), start);
        require_equal(lerp(start, end, 0.25F), Vector3{2.5F, 5.F, 7.5F});
        require_equal(lerp(start, end, 1.F), end);
    }

    SECTION("reflect")
    {
        require_equal(reflect(Vector3{1.F, -1.F, 0.F}, Vector3{0.F, 1.F, 0.F}),
                      Vector3{1.F, 1.F, 0.F});
        require_equal(reflect(Vector3{1.F, 0.F, 0.F}, Vector3{1.F, 0.F, 0.F}),
                      Vector3{-1.F, 0.F, 0.F});
        require_equal(reflect(Vector3{1.F, -1.F, 0.F}, Vector3{0.F, 2.F, 0.F}),
                      Vector3{1.F, 1.F, 0.F});
    }
}
