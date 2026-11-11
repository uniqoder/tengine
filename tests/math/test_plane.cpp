// These tests assume the plane equation dot(normal, point) = distance, with a
// normalized normal. Signed distance is positive on the side the normal points
// toward.

#include <catch2/catch_test_macros.hpp>

#include "tengine/math/plane.hpp"
#include "tengine/math/scalar.hpp"
#include "tengine/math/vector3.hpp"

#include "test_helpers.hpp"

using namespace tengine::math;
using namespace tengine::test;

TEST_CASE("Plane construction", "[math][plane]")
{
    const Plane default_plane;
    require_equal(default_plane.normal, Vector3::up());
    REQUIRE(default_plane.distance == 0.F);

    const Plane plane(Vector3{0.F, 2.F, 0.F}, 4.F);
    require_equal(plane.normal, Vector3{0.F, 2.F, 0.F});
    REQUIRE(plane.distance == 4.F);
}

TEST_CASE("Plane from_point_normal", "[math][plane]")
{
    const Plane plane = Plane::from_point_normal(Vector3{0.F, 5.F, 0.F}, Vector3{0.F, 2.F, 0.F});

    require_equal(plane.normal, Vector3::up());
    REQUIRE(plane.normal.length() == approx(1.F));
    REQUIRE(plane.distance == approx(5.F));
}

TEST_CASE("Plane signed_distance", "[math][plane]")
{
    const Plane plane = Plane::from_point_normal(Vector3{0.F, 5.F, 0.F}, Vector3::up());

    REQUIRE(plane.signed_distance(Vector3{0.F, 7.F, 0.F}) == approx(2.F));
    REQUIRE(plane.signed_distance(Vector3{0.F, 5.F, 0.F}) == approx(0.F));
    REQUIRE(plane.signed_distance(Vector3{0.F, 3.F, 0.F}) == approx(-2.F));
    REQUIRE(plane.signed_distance(Vector3{10.F, 5.F, -20.F}) == approx(0.F));
}

TEST_CASE("Plane project", "[math][plane]")
{
    const Plane plane = Plane::from_point_normal(Vector3{0.F, 5.F, 0.F}, Vector3::up());

    require_equal(plane.project(Vector3{0.F, 7.F, 0.F}), Vector3{0.F, 5.F, 0.F});
    require_equal(plane.project(Vector3{1.F, 7.F, 2.F}), Vector3{1.F, 5.F, 2.F});
    require_equal(plane.project(Vector3{0.F, 5.F, 0.F}), Vector3{0.F, 5.F, 0.F});
}
