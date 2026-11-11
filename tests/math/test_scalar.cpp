#include <catch2/catch_test_macros.hpp>

#include <cmath>

#include "tengine/math/scalar.hpp"

#include "test_helpers.hpp"

using namespace tengine::math;
using namespace tengine::test;

TEST_CASE("Scalar constants", "[math][scalar]")
{
    REQUIRE(Pi == approx(3.14159265358979323846F));
    REQUIRE(TwoPi == approx(2.F * Pi));
    REQUIRE(HalfPi == approx(Pi / 2.F));
    REQUIRE(Epsilon > 0.F);
    REQUIRE(std::isinf(Infinity));
}

TEST_CASE("Scalar angle conversions", "[math][scalar]")
{
    REQUIRE(radians(0.F) == 0.F);
    REQUIRE(radians(90.F) == approx(HalfPi));
    REQUIRE(radians(180.F) == approx(Pi));
    REQUIRE(radians(-90.F) == approx(-HalfPi));

    REQUIRE(degrees(0.F) == 0.F);
    REQUIRE(degrees(HalfPi) == approx(90.F));
    REQUIRE(degrees(Pi) == approx(180.F));

    REQUIRE(degrees(radians(37.F)) == approx(37.F));
    REQUIRE(radians(degrees(1.25F)) == approx(1.25F));
}

TEST_CASE("Scalar basic operations", "[math][scalar]")
{
    SECTION("square")
    {
        REQUIRE(square(3.F) == 9.F);
        REQUIRE(square(-3.F) == 9.F);
        REQUIRE(square(0.F) == 0.F);
    }

    SECTION("absolute")
    {
        REQUIRE(absolute(3.5F) == 3.5F);
        REQUIRE(absolute(-3.5F) == 3.5F);
        REQUIRE(absolute(0.F) == 0.F);
    }

    SECTION("clamp")
    {
        REQUIRE(clamp(-1.F, 0.F, 1.F) == 0.F);
        REQUIRE(clamp(0.F, 0.F, 1.F) == 0.F);
        REQUIRE(clamp(0.5F, 0.F, 1.F) == 0.5F);
        REQUIRE(clamp(1.F, 0.F, 1.F) == 1.F);
        REQUIRE(clamp(2.F, 0.F, 1.F) == 1.F);
    }

    SECTION("saturate")
    {
        REQUIRE(saturate(-0.5F) == 0.F);
        REQUIRE(saturate(0.25F) == 0.25F);
        REQUIRE(saturate(1.5F) == 1.F);
    }

    SECTION("lerp")
    {
        REQUIRE(lerp(10.F, 20.F, 0.F) == 10.F);
        REQUIRE(lerp(10.F, 20.F, 0.5F) == 15.F);
        REQUIRE(lerp(10.F, 20.F, 1.F) == 20.F);
        REQUIRE(lerp(10.F, 20.F, 2.F) == 30.F);
    }
}

TEST_CASE("Scalar nearly_equal", "[math][scalar]")
{
    REQUIRE(nearly_equal(1.F, 1.F));
    REQUIRE(nearly_equal(1.F, 1.F + 0.5F * Epsilon));
    REQUIRE_FALSE(nearly_equal(1.F, 1.F + 2.F * Epsilon));

    REQUIRE(nearly_equal(1.F, 1.1F, 0.2F));
    REQUIRE_FALSE(nearly_equal(1.F, 1.1F, 0.05F));
}

TEST_CASE("Scalar inverse_sqrt", "[math][scalar]")
{
    REQUIRE(inverse_sqrt(4.F) == approx(0.5F));
    REQUIRE(inverse_sqrt(16.F) == approx(0.25F));
    REQUIRE(inverse_sqrt(0.25F) == approx(2.F));
}
