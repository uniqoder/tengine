// These tests assume a right-handed coordinate system, column vectors, and
// the rotation conventions of Matrix3/Quaternion. A Transform's matrix is
// expected to be translation * rotation * scale.

#include <catch2/catch_test_macros.hpp>

#include "tengine/math/matrix4.hpp"
#include "tengine/math/quaternion.hpp"
#include "tengine/math/scalar.hpp"
#include "tengine/math/transform.hpp"
#include "tengine/math/vector3.hpp"

#include "test_helpers.hpp"

using namespace tengine::math;
using namespace tengine::test;

TEST_CASE("Transform identity", "[math][transform]")
{
    Transform transform;
    transform.rotation = Quaternion::identity();

    require_equal(transform.matrix(), Matrix4::identity());
    require_equal(transform.transform_point(Vector3{1.F, 2.F, 3.F}), Vector3{1.F, 2.F, 3.F});
    require_equal(transform.transform_direction(Vector3{1.F, 2.F, 3.F}), Vector3{1.F, 2.F, 3.F});
    require_equal(transform.right(), Vector3::right());
    require_equal(transform.up(), Vector3::up());
    require_equal(transform.forward(), Vector3::forward());
}

TEST_CASE("Transform matrix composition", "[math][transform]")
{
    Transform transform;
    transform.position = Vector3{1.F, 2.F, 3.F};
    transform.rotation = Quaternion::from_axis_angle(Vector3::up(), HalfPi);
    transform.scale = Vector3{2.F, 3.F, 4.F};

    const Matrix4 expected = Matrix4::translation(transform.position) *
                             transform.rotation.to_matrix4() * Matrix4::scaling(transform.scale);

    require_equal(transform.matrix(), expected);

    const Vector3 point{1.F, 1.F, 1.F};
    require_equal(transform.transform_point(point), transform.matrix().transform_point(point));
}

TEST_CASE("Transform point and direction", "[math][transform]")
{
    Transform transform;
    transform.position = Vector3{5.F, 0.F, 0.F};
    transform.rotation = Quaternion::from_axis_angle(Vector3::up(), HalfPi);
    transform.scale = Vector3{2.F, 2.F, 2.F};

    require_equal(transform.transform_point(Vector3{1.F, 0.F, 0.F}), Vector3{5.F, 0.F, -2.F});
    require_equal(transform.transform_direction(Vector3::right()), Vector3::forward());
    require_equal(transform.transform_direction(Vector3::up()), Vector3::up());
    require_equal(transform.transform_direction(Vector3::forward()), Vector3{-1.F, 0.F, 0.F});
}

TEST_CASE("Transform direction vectors", "[math][transform]")
{
    Transform transform;
    transform.position = Vector3{10.F, 20.F, 30.F};
    transform.rotation = Quaternion::from_axis_angle(Vector3::up(), HalfPi);
    transform.scale = Vector3{2.F, 3.F, 4.F};

    require_equal(transform.right(), Vector3::forward());
    require_equal(transform.up(), Vector3::up());
    require_equal(transform.forward(), Vector3{-1.F, 0.F, 0.F});
}
