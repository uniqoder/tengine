// These tests assume right-handed rotations, column vectors, and the same
// rotation conventions as Matrix3::rot_x_mat / rot_y_mat / rot_z_mat.
// Quaternions are stored as (w, x, y, z) and represent unit rotations. All
// angles are in radians. from_euler takes angles around the X, Y and Z axes;
// the exact composition order is not asserted, only round-tripping and
// single-axis behavior.

#include <catch2/catch_test_macros.hpp>

#include <cmath>

#include "tengine/math/matrix3.hpp"
#include "tengine/math/matrix4.hpp"
#include "tengine/math/quaternion.hpp"
#include "tengine/math/scalar.hpp"
#include "tengine/math/vector3.hpp"
#include "tengine/math/vector4.hpp"

#include "test_helpers.hpp"

using namespace tengine::math;
using namespace tengine::test;

TEST_CASE("Quaternion construction", "[math][quaternion]")
{
    SECTION("default constructor")
    {
        const Quaternion quaternion;
        require_equal(quaternion, Quaternion(0.F, 0.F, 0.F, 0.F));
    }

    SECTION("component constructor")
    {
        const Quaternion quaternion(1.F, 2.F, 3.F, 4.F);
        REQUIRE(quaternion.w == 1.F);
        REQUIRE(quaternion.x == 2.F);
        REQUIRE(quaternion.y == 3.F);
        REQUIRE(quaternion.z == 4.F);
    }

    SECTION("identity")
    {
        require_equal(Quaternion::identity(), Quaternion(1.F, 0.F, 0.F, 0.F));
    }
}

TEST_CASE("Quaternion from_axis_angle", "[math][quaternion]")
{
    const Scalar root_half = std::sqrt(0.5F);

    SECTION("zero angle")
    {
        require_equal(Quaternion::from_axis_angle(Vector3::up(), 0.F), Quaternion::identity());
    }

    SECTION("ninety degrees around Y")
    {
        require_equal(Quaternion::from_axis_angle(Vector3::up(), HalfPi),
                      Quaternion(root_half, 0.F, root_half, 0.F));
    }

    SECTION("one hundred eighty degrees around Y")
    {
        require_equal(Quaternion::from_axis_angle(Vector3::up(), Pi),
                      Quaternion(0.F, 0.F, 1.F, 0.F));
    }

    SECTION("axis is normalized")
    {
        require_same_rotation(Quaternion::from_axis_angle(Vector3{0.F, 2.F, 0.F}, HalfPi),
                              Quaternion::from_axis_angle(Vector3::up(), HalfPi));
    }
}

TEST_CASE("Quaternion from_euler", "[math][quaternion]")
{
    SECTION("zero angles produce identity")
    {
        require_same_rotation(Quaternion::from_euler(Vector3{0.F, 0.F, 0.F}),
                              Quaternion::identity());
    }

    SECTION("single axis rotations")
    {
        require_same_rotation(Quaternion::from_euler(Vector3{0.4F, 0.F, 0.F}),
                              Quaternion::from_axis_angle(Vector3::right(), 0.4F));
        require_same_rotation(Quaternion::from_euler(Vector3{0.F, -0.3F, 0.F}),
                              Quaternion::from_axis_angle(Vector3::up(), -0.3F));
        require_same_rotation(Quaternion::from_euler(Vector3{0.F, 0.F, 0.7F}),
                              Quaternion::from_axis_angle(Vector3{0.F, 0.F, 1.F}, 0.7F));
    }

    SECTION("round trip through to_euler")
    {
        const Quaternion quaternion = Quaternion::from_euler(Vector3{0.3F, -0.2F, 0.5F});
        require_same_rotation(Quaternion::from_euler(quaternion.to_euler()), quaternion);
    }
}

TEST_CASE("Quaternion from_rotation_matrix", "[math][quaternion]")
{
    const Matrix3 rotation =
        Matrix3::rot_z_mat(0.3F) * Matrix3::rot_y_mat(-0.4F) * Matrix3::rot_x_mat(0.5F);

    const Quaternion quaternion = Quaternion::from_rotation_matrix(rotation);

    require_equal(quaternion.to_matrix3(), rotation);

    const Vector3 vector{1.F, 2.F, 3.F};
    require_equal(quaternion.rotate(vector), rotation * vector);
}

TEST_CASE("Quaternion magnitude and normalization", "[math][quaternion]")
{
    const Quaternion quaternion(1.F, 2.F, 3.F, 4.F);
    const Scalar length = std::sqrt(30.F);

    REQUIRE(quaternion.length_squared() == approx(30.F));
    REQUIRE(quaternion.length() == approx(length));
    require_equal(quaternion.normalized(),
                  Quaternion(1.F / length, 2.F / length, 3.F / length, 4.F / length));

    Quaternion mutable_quaternion(1.F, 2.F, 3.F, 4.F);
    mutable_quaternion.normalize();
    require_equal(mutable_quaternion, quaternion.normalized());
    REQUIRE(mutable_quaternion.length() == approx(1.F));
}

TEST_CASE("Quaternion conjugate and inverse", "[math][quaternion]")
{
    const Quaternion quaternion(1.F, 2.F, 3.F, 4.F);

    require_equal(quaternion.conjugate(), Quaternion(1.F, -2.F, -3.F, -4.F));

    require_equal(quaternion * quaternion.inverse(), Quaternion::identity());
    require_equal(quaternion.inverse() * quaternion, Quaternion::identity());

    const Quaternion unit = Quaternion::from_axis_angle(Vector3::up(), 0.6F);
    require_equal(unit.inverse(), unit.conjugate());
}

TEST_CASE("Quaternion rotate", "[math][quaternion]")
{
    const Quaternion rotation = Quaternion::from_axis_angle(Vector3::up(), HalfPi);

    require_equal(rotation.rotate(Vector3::right()), Vector3::forward());
    require_equal(rotation.rotate(Vector3::up()), Vector3::up());
    require_equal(rotation.rotate(Vector3::forward()), Vector3{-1.F, 0.F, 0.F});
}

TEST_CASE("Quaternion matrix conversion", "[math][quaternion]")
{
    const Quaternion rotation = Quaternion::from_axis_angle(Vector3::up(), HalfPi);

    require_equal(rotation.to_matrix3(), Matrix3::rot_y_mat(HalfPi));
    require_equal(rotation.to_matrix4(), Matrix4::rotation_y(HalfPi));

    const Vector3 vector{1.F, 2.F, 3.F};
    require_equal(rotation.to_matrix3() * vector, rotation.rotate(vector));
    require_equal(rotation.to_matrix4() * Vector4{vector, 1.F},
                  Vector4{rotation.rotate(vector), 1.F});
}

TEST_CASE("Quaternion to_euler", "[math][quaternion]")
{
    require_equal(Quaternion::identity().to_euler(), Vector3::zero());

    const Vector3 euler = Quaternion::from_axis_angle(Vector3::right(), 0.4F).to_euler();
    REQUIRE(euler.x == approx(0.4F));
    REQUIRE(euler.y == approx(0.F));
    REQUIRE(euler.z == approx(0.F));
}

TEST_CASE("Quaternion arithmetic", "[math][quaternion]")
{
    SECTION("multiplication by identity")
    {
        const Quaternion quaternion = Quaternion::from_axis_angle(Vector3::up(), 0.3F);
        require_equal(quaternion * Quaternion::identity(), quaternion);
        require_equal(Quaternion::identity() * quaternion, quaternion);
    }

    SECTION("Hamilton product")
    {
        const Quaternion i(0.F, 1.F, 0.F, 0.F);
        const Quaternion j(0.F, 0.F, 1.F, 0.F);
        const Quaternion k(0.F, 0.F, 0.F, 1.F);

        require_equal(i * j, k);
        require_equal(j * k, i);
        require_equal(k * i, j);
    }

    SECTION("scalar multiplication")
    {
        const Quaternion quaternion(1.F, 2.F, 3.F, 4.F);
        require_equal(quaternion * 2.F, Quaternion(2.F, 4.F, 6.F, 8.F));
    }

    SECTION("addition")
    {
        const Quaternion lhs(1.F, 2.F, 3.F, 4.F);
        const Quaternion rhs(5.F, 6.F, 7.F, 8.F);
        require_equal(lhs + rhs, Quaternion(6.F, 8.F, 10.F, 12.F));
    }
}

TEST_CASE("Quaternion dot", "[math][quaternion]")
{
    REQUIRE(dot(Quaternion(1.F, 2.F, 3.F, 4.F), Quaternion(5.F, 6.F, 7.F, 8.F)) == 70.F);
}

TEST_CASE("Quaternion slerp", "[math][quaternion]")
{
    const Quaternion start = Quaternion::identity();
    const Quaternion end = Quaternion::from_axis_angle(Vector3::up(), Pi);

    require_same_rotation(slerp(start, end, 0.F), start);
    require_same_rotation(slerp(start, end, 0.5F),
                          Quaternion::from_axis_angle(Vector3::up(), HalfPi));
    require_same_rotation(slerp(start, end, 1.F), end);

    const Quaternion quarter = Quaternion::from_axis_angle(Vector3::up(), HalfPi);
    const Quaternion halfway = slerp(start, quarter, 0.5F);
    require_equal(halfway.rotate(Vector3::right()),
                  Vector3{std::sqrt(0.5F), 0.F, -std::sqrt(0.5F)});
}
