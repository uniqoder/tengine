// These tests assume the same conventions as the individual shape tests:
// inclusive intersections, nearest positive ray parameters, and plane equation
// dot(normal, point) = distance.

#include <catch2/catch_test_macros.hpp>

#include "tengine/math/aabb.hpp"
#include "tengine/math/intersections.hpp"
#include "tengine/math/plane.hpp"
#include "tengine/math/ray.hpp"
#include "tengine/math/scalar.hpp"
#include "tengine/math/sphere.hpp"
#include "tengine/math/vector3.hpp"

#include "test_helpers.hpp"

using namespace tengine::math;
using namespace tengine::test;

TEST_CASE("Ray/Plane intersection", "[math][intersections]")
{
    const Plane plane = Plane::from_point_normal(Vector3{0.F, 5.F, 0.F}, Vector3::up());

    SECTION("hit")
    {
        Scalar distance = 0.F;
        const Ray ray(Vector3{0.F, 0.F, 0.F}, Vector3::up());

        REQUIRE(intersect(ray, plane, distance));
        REQUIRE(distance == approx(5.F));
    }

    SECTION("parallel")
    {
        Scalar distance = 0.F;
        const Ray ray(Vector3{0.F, 0.F, 0.F}, Vector3::forward());

        REQUIRE_FALSE(intersect(ray, plane, distance));
    }

    SECTION("plane behind the ray")
    {
        Scalar distance = 0.F;
        const Ray ray(Vector3{0.F, 6.F, 0.F}, Vector3::up());

        REQUIRE_FALSE(intersect(ray, plane, distance));
    }
}

TEST_CASE("Ray/Sphere intersection", "[math][intersections]")
{
    const Sphere sphere(Vector3{0.F, 0.F, 0.F}, 1.F);

    SECTION("hit")
    {
        Scalar distance = 0.F;
        const Ray ray(Vector3{0.F, 0.F, 5.F}, Vector3::forward());

        REQUIRE(intersect(ray, sphere, distance));
        REQUIRE(distance == approx(4.F));
    }

    SECTION("miss")
    {
        Scalar distance = 0.F;
        const Ray ray(Vector3{0.F, 2.F, 5.F}, Vector3::forward());

        REQUIRE_FALSE(intersect(ray, sphere, distance));
    }
}

TEST_CASE("Ray/AABB intersection", "[math][intersections]")
{
    const AABB box(Vector3{-1.F, -1.F, -1.F}, Vector3{1.F, 1.F, 1.F});

    SECTION("hit")
    {
        Scalar distance = 0.F;
        const Ray ray(Vector3{0.F, 0.F, 5.F}, Vector3::forward());

        REQUIRE(intersect(ray, box, distance));
        REQUIRE(distance == approx(4.F));
    }

    SECTION("miss")
    {
        Scalar distance = 0.F;
        const Ray ray(Vector3{2.F, 0.F, 5.F}, Vector3::forward());

        REQUIRE_FALSE(intersect(ray, box, distance));
    }

    SECTION("agrees with AABB member function")
    {
        const Ray ray(Vector3{0.F, 0.F, 5.F}, Vector3::forward());
        Scalar member_distance = 0.F;
        Scalar free_distance = 0.F;

        REQUIRE(box.intersects(ray, member_distance));
        REQUIRE(intersect(ray, box, free_distance));
        REQUIRE(free_distance == approx(member_distance));
    }
}

TEST_CASE("AABB/AABB intersection", "[math][intersections]")
{
    const AABB box(Vector3{-1.F, -1.F, -1.F}, Vector3{1.F, 1.F, 1.F});

    REQUIRE(intersect(box, AABB(Vector3{0.F, 0.F, 0.F}, Vector3{2.F, 2.F, 2.F})));
    REQUIRE(intersect(box, AABB(Vector3{1.F, 1.F, 1.F}, Vector3{2.F, 2.F, 2.F})));
    REQUIRE_FALSE(intersect(box, AABB(Vector3{1.1F, 0.F, 0.F}, Vector3{2.F, 2.F, 2.F})));
}

TEST_CASE("Sphere/Sphere intersection", "[math][intersections]")
{
    const Sphere sphere(Vector3{0.F, 0.F, 0.F}, 1.F);

    REQUIRE(intersect(sphere, Sphere(Vector3{1.F, 0.F, 0.F}, 1.F)));
    REQUIRE(intersect(sphere, Sphere(Vector3{2.F, 0.F, 0.F}, 1.F)));
    REQUIRE_FALSE(intersect(sphere, Sphere(Vector3{2.1F, 0.F, 0.F}, 1.F)));
}

TEST_CASE("Sphere/AABB intersection", "[math][intersections]")
{
    const Sphere sphere(Vector3{0.F, 0.F, 0.F}, 1.F);
    const AABB box(Vector3{-1.F, -1.F, -1.F}, Vector3{1.F, 1.F, 1.F});

    REQUIRE(intersect(sphere, box));
    REQUIRE(intersect(sphere, AABB(Vector3{1.F, 0.F, 0.F}, Vector3{2.F, 2.F, 2.F})));
    REQUIRE_FALSE(intersect(sphere, AABB(Vector3{2.F, 0.F, 0.F}, Vector3{3.F, 2.F, 2.F})));
    REQUIRE(intersect(sphere, box) == box.intersects(sphere));
}
