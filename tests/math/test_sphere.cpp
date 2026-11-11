// These tests assume inclusive containment and intersection tests: points on
// the surface and shapes that touch count as intersections. Ray intersections
// report the nearest positive distance.

#include <catch2/catch_test_macros.hpp>

#include "tengine/math/aabb.hpp"
#include "tengine/math/ray.hpp"
#include "tengine/math/scalar.hpp"
#include "tengine/math/sphere.hpp"
#include "tengine/math/vector3.hpp"

#include "test_helpers.hpp"

using namespace tengine::math;
using namespace tengine::test;

TEST_CASE("Sphere construction", "[math][sphere]")
{
    const Sphere default_sphere;
    require_equal(default_sphere.center, Vector3::zero());
    REQUIRE(default_sphere.radius == 1.F);

    const Sphere sphere(Vector3{1.F, 2.F, 3.F}, 4.F);
    require_equal(sphere.center, Vector3{1.F, 2.F, 3.F});
    REQUIRE(sphere.radius == 4.F);
}

TEST_CASE("Sphere contains", "[math][sphere]")
{
    Sphere sphere(Vector3{1.F, 2.F, 3.F}, 2.F);

    REQUIRE(sphere.contains(Vector3{1.F, 2.F, 3.F}));
    REQUIRE(sphere.contains(Vector3{3.F, 2.F, 3.F}));
    REQUIRE(sphere.contains(Vector3{1.F, 4.F, 3.F}));
    REQUIRE_FALSE(sphere.contains(Vector3{4.F, 2.F, 3.F}));
}

TEST_CASE("Sphere intersects Sphere", "[math][sphere]")
{
    const Sphere sphere(Vector3{0.F, 0.F, 0.F}, 1.F);

    REQUIRE(sphere.intersects(Sphere(Vector3{1.F, 0.F, 0.F}, 1.F)));
    REQUIRE(sphere.intersects(Sphere(Vector3{2.F, 0.F, 0.F}, 1.F)));
    REQUIRE(sphere.intersects(Sphere(Vector3{0.F, 0.F, 0.F}, 3.F)));

    REQUIRE_FALSE(sphere.intersects(Sphere(Vector3{2.1F, 0.F, 0.F}, 1.F)));
}

TEST_CASE("Sphere intersects AABB", "[math][sphere]")
{
    const Sphere sphere(Vector3{0.F, 0.F, 0.F}, 1.F);
    const AABB box(Vector3{-1.F, -1.F, -1.F}, Vector3{1.F, 1.F, 1.F});

    REQUIRE(sphere.intersects(box));
    REQUIRE(sphere.intersects(AABB(Vector3{1.F, 0.F, 0.F}, Vector3{2.F, 2.F, 2.F})));
    REQUIRE(sphere.intersects(AABB(Vector3{-0.5F, -0.5F, -0.5F}, Vector3{0.5F, 0.5F, 0.5F})));

    REQUIRE_FALSE(sphere.intersects(AABB(Vector3{2.F, 0.F, 0.F}, Vector3{3.F, 2.F, 2.F})));
}

TEST_CASE("Sphere intersects Ray", "[math][sphere]")
{
    const Sphere sphere(Vector3{0.F, 0.F, 0.F}, 1.F);

    SECTION("hit from the front")
    {
        Scalar distance = 0.F;
        const Ray ray(Vector3{0.F, 0.F, 5.F}, Vector3::forward());

        REQUIRE(sphere.intersects(ray, distance));
        REQUIRE(distance == approx(4.F));
    }

    SECTION("tangent hit")
    {
        Scalar distance = 0.F;
        const Ray ray(Vector3{1.F, 0.F, 5.F}, Vector3::forward());

        REQUIRE(sphere.intersects(ray, distance));
        REQUIRE(distance == approx(5.F));
    }

    SECTION("pointing away")
    {
        Scalar distance = 0.F;
        const Ray ray(Vector3{0.F, 0.F, 5.F}, Vector3{0.F, 0.F, 1.F});

        REQUIRE_FALSE(sphere.intersects(ray, distance));
    }

    SECTION("miss")
    {
        Scalar distance = 0.F;
        const Ray ray(Vector3{0.F, 2.F, 5.F}, Vector3::forward());

        REQUIRE_FALSE(sphere.intersects(ray, distance));
    }
}
