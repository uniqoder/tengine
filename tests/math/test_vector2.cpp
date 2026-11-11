#include <catch2/catch_test_macros.hpp>

#include "tengine/math/scalar.hpp"
#include "tengine/math/vector2.hpp"

#include "test_helpers.hpp"

using namespace tengine::math;
using namespace tengine::test;

TEST_CASE("Vector2 construction and component access", "[math][vector2]")
{
    SECTION("default and component constructors")
    {
        const Vector2 defaulted;
        require_equal(defaulted, Vector2{0.F, 0.F});

        const Vector2 uniform(3.F);
        require_equal(uniform, Vector2{3.F, 3.F});

        const Vector2 vector{1.F, 2.F};
        REQUIRE(vector.x == 1.F);
        REQUIRE(vector.y == 2.F);
    }

    SECTION("subscript access")
    {
        Vector2 vector{1.F, 2.F};
        REQUIRE(vector[0] == 1.F);
        REQUIRE(vector[1] == 2.F);

        vector[0] = 3.F;
        vector[1] = 4.F;
        require_equal(vector, Vector2{3.F, 4.F});

        const Vector2 const_vector{5.F, 6.F};
        REQUIRE(const_vector[0] == 5.F);
        REQUIRE(const_vector[1] == 6.F);
    }

    SECTION("common vectors")
    {
        require_equal(Vector2::zero(), Vector2{0.F, 0.F});
        require_equal(Vector2::one(), Vector2{1.F, 1.F});
        require_equal(Vector2::right(), Vector2{1.F, 0.F});
        require_equal(Vector2::up(), Vector2{0.F, 1.F});
    }

    SECTION("equality")
    {
        REQUIRE(Vector2{1.F, 2.F} == Vector2{1.F, 2.F});
        REQUIRE_FALSE(Vector2{1.F, 2.F} == Vector2{2.F, 1.F});
    }
}

TEST_CASE("Vector2 arithmetic", "[math][vector2]")
{
    SECTION("unary operators")
    {
        const Vector2 vector{1.F, -2.F};
        REQUIRE(+vector == vector);
        require_equal(-vector, Vector2{-1.F, 2.F});
    }

    SECTION("compound assignment")
    {
        Vector2 vector{1.F, 2.F};

        vector += Vector2{3.F, 4.F};
        require_equal(vector, Vector2{4.F, 6.F});

        vector -= Vector2{1.F, 2.F};
        require_equal(vector, Vector2{3.F, 4.F});

        vector *= 2.F;
        require_equal(vector, Vector2{6.F, 8.F});

        vector /= 2.F;
        require_equal(vector, Vector2{3.F, 4.F});
    }

    SECTION("free operators")
    {
        require_equal(Vector2{1.F, 2.F} + Vector2{3.F, 4.F}, Vector2{4.F, 6.F});
        require_equal(Vector2{3.F, 4.F} - Vector2{1.F, 2.F}, Vector2{2.F, 2.F});
        require_equal(Vector2{1.F, 2.F} * 2.F, Vector2{2.F, 4.F});
        require_equal(2.F * Vector2{1.F, 2.F}, Vector2{2.F, 4.F});
        require_equal(Vector2{2.F, 4.F} / 2.F, Vector2{1.F, 2.F});
    }
}

TEST_CASE("Vector2 magnitude and normalization", "[math][vector2]")
{
    const Vector2 vector{3.F, 4.F};

    REQUIRE(vector.length_squared() == 25.F);
    REQUIRE(vector.length() == approx(5.F));
    require_equal(vector.normalized(), Vector2{0.6F, 0.8F});

    Vector2 mutable_vector{3.F, 4.F};
    mutable_vector.normalize();
    require_equal(mutable_vector, Vector2{0.6F, 0.8F});
    REQUIRE(mutable_vector.length() == approx(1.F));
}

TEST_CASE("Vector2 is_zero", "[math][vector2]")
{
    REQUIRE(Vector2::zero().is_zero());
    REQUIRE_FALSE(Vector2{1.F, 0.F}.is_zero());
    REQUIRE(Vector2{1.0e-4F, 0.F}.is_zero(1.0e-3F));
    REQUIRE_FALSE(Vector2{1.0e-4F, 0.F}.is_zero());
}

TEST_CASE("Vector2 vector operations", "[math][vector2]")
{
    SECTION("dot")
    {
        REQUIRE(dot(Vector2{1.F, 2.F}, Vector2{3.F, 4.F}) == 11.F);
        REQUIRE(dot(Vector2::right(), Vector2::up()) == 0.F);
    }

    SECTION("distance")
    {
        REQUIRE(distance(Vector2{0.F, 0.F}, Vector2{3.F, 4.F}) == approx(5.F));
        REQUIRE(distance(Vector2{1.F, 1.F}, Vector2{1.F, 1.F}) == 0.F);
    }

    SECTION("angle")
    {
        REQUIRE(angle(Vector2::right(), Vector2::up()) == approx(HalfPi));
        REQUIRE(angle(Vector2::right(), Vector2::right()) == approx(0.F));
        REQUIRE(angle(Vector2{1.F, 0.F}, Vector2{1.F, 1.F}) == approx(Pi / 4.F));
        REQUIRE(angle(Vector2{2.F, 0.F}, Vector2{3.F, 0.F}) == approx(0.F));
    }

    SECTION("lerp")
    {
        const Vector2 start{0.F, 0.F};
        const Vector2 end{10.F, 20.F};

        require_equal(lerp(start, end, 0.F), start);
        require_equal(lerp(start, end, 0.25F), Vector2{2.5F, 5.F});
        require_equal(lerp(start, end, 1.F), end);
    }
}
