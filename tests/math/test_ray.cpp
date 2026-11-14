#include <catch2/catch_test_macros.hpp>

#include "tengine/math/ray.hpp"
#include "tengine/math/vector3.hpp"

#include "test_helpers.hpp"

using namespace tengine::math;
using namespace tengine::test;

TEST_CASE("Ray construction", "[math][ray]")
{
    const Ray default_ray;
    require_equal(default_ray.origin, Vector3::zero());
    require_equal(default_ray.direction, Vector3::forward());

    const Ray ray(Vector3{1.F, 2.F, 3.F}, Vector3{0.F, 1.F, 0.F});
    require_equal(ray.origin, Vector3{1.F, 2.F, 3.F});
    require_equal(ray.direction, Vector3{0.F, 1.F, 0.F});
}

TEST_CASE("Ray at", "[math][ray]")
{
    Ray ray(Vector3{1.F, 2.F, 3.F}, Vector3{0.F, 0.F, -1.F});

    require_equal(ray.at(0.F), Vector3{1.F, 2.F, 3.F});
    require_equal(ray.at(5.F), Vector3{1.F, 2.F, -2.F});
    require_equal(ray.at(-1.F), Vector3{1.F, 2.F, 4.F});
}
