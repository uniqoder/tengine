// These tests assume inclusive containment and intersection tests: touching
// boundaries count as intersections.

#include <catch2/catch_test_macros.hpp>

#include <cmath>

#include "tengine/math/aabb.hpp"
#include "tengine/math/ray.hpp"
#include "tengine/math/scalar.hpp"
#include "tengine/math/sphere.hpp"
#include "tengine/math/vector3.hpp"

#include "test_helpers.hpp"

using namespace tengine::math;
using namespace tengine::test;

TEST_CASE("AABB properties", "[math][aabb]")
{
    const AABB box(Vector3{-1.F, -2.F, -3.F}, Vector3{3.F, 4.F, 5.F});

    require_equal(box.center(), Vector3{1.F, 1.F, 1.F});
    require_equal(box.size(), Vector3{4.F, 6.F, 8.F});
    require_equal(box.extents(), Vector3{2.F, 3.F, 4.F});
}

TEST_CASE("AABB contains", "[math][aabb]")
{
    const AABB box(Vector3{-1.F, -2.F, -3.F}, Vector3{3.F, 4.F, 5.F});

    REQUIRE(box.contains(Vector3{1.F, 1.F, 1.F}));
    REQUIRE(box.contains(Vector3{-1.F, -2.F, -3.F}));
    REQUIRE(box.contains(Vector3{3.F, 4.F, 5.F}));
    REQUIRE(box.contains(Vector3{3.F, 0.F, 0.F}));

    REQUIRE_FALSE(box.contains(Vector3{-2.F, 0.F, 0.F}));
    REQUIRE_FALSE(box.contains(Vector3{4.F, 0.F, 0.F}));
    REQUIRE_FALSE(box.contains(Vector3{0.F, -3.F, 0.F}));
    REQUIRE_FALSE(box.contains(Vector3{0.F, 5.F, 0.F}));
    REQUIRE_FALSE(box.contains(Vector3{0.F, 0.F, -4.F}));
    REQUIRE_FALSE(box.contains(Vector3{0.F, 0.F, 6.F}));
}

TEST_CASE("AABB intersects AABB", "[math][aabb]")
{
    const AABB box(Vector3{-1.F, -1.F, -1.F}, Vector3{1.F, 1.F, 1.F});

    REQUIRE(box.intersects(AABB(Vector3{-2.F, -2.F, -2.F}, Vector3{2.F, 2.F, 2.F})));
    REQUIRE(box.intersects(AABB(Vector3{0.F, 0.F, 0.F}, Vector3{2.F, 2.F, 2.F})));
    REQUIRE(box.intersects(AABB(Vector3{1.F, 1.F, 1.F}, Vector3{2.F, 2.F, 2.F})));

    REQUIRE_FALSE(box.intersects(AABB(Vector3{1.1F, 0.F, 0.F}, Vector3{2.F, 2.F, 2.F})));
    REQUIRE_FALSE(box.intersects(AABB(Vector3{0.F, 1.1F, 0.F}, Vector3{2.F, 2.F, 2.F})));
    REQUIRE_FALSE(box.intersects(AABB(Vector3{0.F, 0.F, 1.1F}, Vector3{2.F, 2.F, 2.F})));
}

TEST_CASE("AABB intersects Sphere", "[math][aabb]")
{
    const AABB box(Vector3{-1.F, -1.F, -1.F}, Vector3{1.F, 1.F, 1.F});

    REQUIRE(box.intersects(Sphere(Vector3{0.F, 0.F, 0.F}, 0.5F)));
    REQUIRE(box.intersects(Sphere(Vector3{2.F, 0.F, 0.F}, 1.F)));
    REQUIRE(box.intersects(Sphere(Vector3{0.F, 0.F, 0.F}, 2.F)));

    REQUIRE_FALSE(box.intersects(Sphere(Vector3{3.F, 0.F, 0.F}, 1.F)));
    REQUIRE_FALSE(box.intersects(Sphere(Vector3{0.F, 3.F, 0.F}, 1.F)));
    REQUIRE_FALSE(box.intersects(Sphere(Vector3{0.F, 0.F, 3.F}, 1.F)));
}

TEST_CASE("AABB intersects Ray", "[math][aabb]")
{
    const AABB box(Vector3{-1.F, -1.F, -1.F}, Vector3{1.F, 1.F, 1.F});

    SECTION("hit from the front")
    {
        Scalar distance = 0.F;
        const Ray ray(Vector3{0.F, 0.F, 5.F}, Vector3::forward());

        REQUIRE(box.intersects(ray, distance));
        REQUIRE(distance == approx(4.F));
    }

    SECTION("hit from an angle")
    {
        Scalar distance = 0.F;
        const Ray ray(Vector3{5.F, 0.F, 5.F}, Vector3{-1.F, 0.F, -1.F}.normalized());

        REQUIRE(box.intersects(ray, distance));
        REQUIRE(distance == approx(std::sqrt(32.F)));
    }

    SECTION("pointing away")
    {
        Scalar distance = 0.F;
        const Ray ray(Vector3{0.F, 0.F, 5.F}, Vector3{0.F, 0.F, 1.F});

        REQUIRE_FALSE(box.intersects(ray, distance));
    }

    SECTION("parallel to a slab but outside it")
    {
        Scalar distance = 0.F;
        const Ray ray(Vector3{2.F, 0.F, 5.F}, Vector3::forward());

        REQUIRE_FALSE(box.intersects(ray, distance));
    }
}

TEST_CASE("AABB expanded", "[math][aabb]")
{
    const AABB box(Vector3{-1.F, -2.F, -3.F}, Vector3{3.F, 4.F, 5.F});

    require_equal(box.expanded(1.F).min, Vector3{-2.F, -3.F, -4.F});
    require_equal(box.expanded(1.F).max, Vector3{4.F, 5.F, 6.F});
    require_equal(box.expanded(0.F).min, box.min);
    require_equal(box.expanded(0.F).max, box.max);
}
