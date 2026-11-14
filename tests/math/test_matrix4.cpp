// These tests assume a right-handed coordinate system, row-major storage,
// column vectors (v' = M * v), and the same rotation conventions as Matrix3.
// All angles are in radians. Projection matrices use the OpenGL convention:
// the view direction is -Z and the near/far planes map to NDC z = -1 / +1
// respectively.

#include <catch2/catch_test_macros.hpp>

#include "tengine/math/matrix4.hpp"
#include "tengine/math/scalar.hpp"
#include "tengine/math/vector3.hpp"
#include "tengine/math/vector4.hpp"

#include "test_helpers.hpp"

using namespace tengine::math;
using namespace tengine::test;

TEST_CASE("Matrix4 construction", "[math][matrix4]")
{
    SECTION("default constructor produces a zero matrix")
    {
        const Matrix4 matrix;
        require_equal(matrix, Matrix4(0.F));
    }

    SECTION("diagonal constructor")
    {
        const Matrix4 matrix(2.F);

        Matrix4 expected({{{2.F, 0.F, 0.F, 0.F},
                           {0.F, 2.F, 0.F, 0.F},
                           {0.F, 0.F, 2.F, 0.F},
                           {0.F, 0.F, 0.F, 2.F}}});
        require_equal(matrix, expected);
    }

    SECTION("identity and zero")
    {
        Matrix4 expected_identity({{{1.F, 0.F, 0.F, 0.F},
                                    {0.F, 1.F, 0.F, 0.F},
                                    {0.F, 0.F, 1.F, 0.F},
                                    {0.F, 0.F, 0.F, 1.F}}});

        require_equal(Matrix4::identity(), expected_identity);
        require_equal(Matrix4::zero(), Matrix4(0.F));
    }
}

TEST_CASE("Matrix4 element access", "[math][matrix4]")
{
    Matrix4 matrix;
    for (int row = 0; row < 4; ++row)
    {
        for (int column = 0; column < 4; ++column)
        {
            matrix(row, column) = static_cast<Scalar>(row * 4 + column);
        }
    }

    for (int row = 0; row < 4; ++row)
    {
        for (int column = 0; column < 4; ++column)
        {
            REQUIRE(matrix(row, column) == static_cast<Scalar>(row * 4 + column));
        }
    }

    const Matrix4& const_matrix = matrix;
    REQUIRE(const_matrix(0, 0) == 0.F);
    REQUIRE(const_matrix(1, 2) == 6.F);
    REQUIRE(const_matrix(3, 3) == 15.F);
}

TEST_CASE("Matrix4 translation", "[math][matrix4]")
{
    const Matrix4 translation = Matrix4::translation(Vector3{1.F, 2.F, 3.F});

    Matrix4 expected = Matrix4::identity();
    expected(0, 3) = 1.F;
    expected(1, 3) = 2.F;
    expected(2, 3) = 3.F;
    require_equal(translation, expected);

    require_equal(translation.transform_point(Vector3{1.F, 1.F, 1.F}), Vector3{2.F, 3.F, 4.F});
    require_equal(translation.transform_vector(Vector3{1.F, 1.F, 1.F}), Vector3{1.F, 1.F, 1.F});
    require_equal(translation * Vector4{1.F, 2.F, 3.F, 1.F}, Vector4{2.F, 4.F, 6.F, 1.F});
}

TEST_CASE("Matrix4 scale", "[math][matrix4]")
{
    const Matrix4 scale = Matrix4::scaling(Vector3{2.F, 3.F, 4.F});

    Matrix4 expected = Matrix4::identity();
    expected(0, 0) = 2.F;
    expected(1, 1) = 3.F;
    expected(2, 2) = 4.F;
    require_equal(scale, expected);

    require_equal(scale.transform_point(Vector3{1.F, 1.F, 1.F}), Vector3{2.F, 3.F, 4.F});
    require_equal(scale.transform_vector(Vector3{1.F, 1.F, 1.F}), Vector3{2.F, 3.F, 4.F});
}

TEST_CASE("Matrix4 rotation factories", "[math][matrix4]")
{
    SECTION("zero angle produces identity")
    {
        require_equal(Matrix4::rotation_x(0.F), Matrix4::identity());
        require_equal(Matrix4::rotation_y(0.F), Matrix4::identity());
        require_equal(Matrix4::rotation_z(0.F), Matrix4::identity());
    }

    SECTION("rotation of basis vectors")
    {
        require_equal(Matrix4::rotation_x(HalfPi) * Vector4{0.F, 1.F, 0.F, 0.F},
                      Vector4{0.F, 0.F, 1.F, 0.F});
        require_equal(Matrix4::rotation_y(HalfPi) * Vector4{1.F, 0.F, 0.F, 0.F},
                      Vector4{0.F, 0.F, -1.F, 0.F});
        require_equal(Matrix4::rotation_z(HalfPi) * Vector4{1.F, 0.F, 0.F, 0.F},
                      Vector4{0.F, 1.F, 0.F, 0.F});
    }

    SECTION("rotation matrix layout")
    {
        const Matrix4 rotation = Matrix4::rotation_z(HalfPi);

        REQUIRE(rotation(0, 0) == approx(0.F));
        REQUIRE(rotation(0, 1) == approx(-1.F));
        REQUIRE(rotation(1, 0) == approx(1.F));
        REQUIRE(rotation(1, 1) == approx(0.F));
        REQUIRE(rotation(2, 2) == approx(1.F));
        REQUIRE(rotation(3, 3) == approx(1.F));
    }
}

TEST_CASE("Matrix4 composition", "[math][matrix4]")
{
    const Matrix4 model = Matrix4::translation(Vector3{5.F, 0.F, 0.F}) *
                          Matrix4::rotation_z(HalfPi) * Matrix4::scaling(Vector3{2.F, 2.F, 2.F});

    require_equal(model.transform_point(Vector3{1.F, 0.F, 0.F}), Vector3{5.F, 2.F, 0.F});
    require_equal(model.transform_vector(Vector3{1.F, 0.F, 0.F}), Vector3{0.F, 2.F, 0.F});
}

TEST_CASE("Matrix4 perspective projection", "[math][matrix4]")
{
    const Matrix4 projection = Matrix4::perspective(HalfPi, 2.F, 1.F, 3.F);

    // f = 1 / tan(fov / 2) = 1, with near = 1 and far = 3.
    REQUIRE(projection(0, 0) == approx(0.5F));
    REQUIRE(projection(1, 1) == approx(1.F));
    REQUIRE(projection(2, 2) == approx(-2.F));
    REQUIRE(projection(2, 3) == approx(-3.F));
    REQUIRE(projection(3, 2) == approx(-1.F));
    REQUIRE(projection(3, 3) == approx(0.F));

    const Vector4 near_clip = projection * Vector4{0.F, 0.F, -1.F, 1.F};
    REQUIRE(near_clip.z / near_clip.w == approx(-1.F));

    const Vector4 far_clip = projection * Vector4{0.F, 0.F, -3.F, 1.F};
    REQUIRE(far_clip.z / far_clip.w == approx(1.F));

    const Vector4 edge_clip = projection * Vector4{2.F, 1.F, -1.F, 1.F};
    REQUIRE(edge_clip.x / edge_clip.w == approx(1.F));
    REQUIRE(edge_clip.y / edge_clip.w == approx(1.F));

    // transform_point is expected to perform the perspective divide.
    require_equal(projection.transform_point(Vector3{0.F, 0.F, -2.F}), Vector3{0.F, 0.F, 0.5F});
}

TEST_CASE("Matrix4 orthographic projection", "[math][matrix4]")
{
    const Matrix4 projection = Matrix4::orthographic(1.F, 3.F, 2.F, 6.F, 1.F, 5.F);

    REQUIRE(projection(0, 0) == approx(1.F));
    REQUIRE(projection(0, 3) == approx(-2.F));
    REQUIRE(projection(1, 1) == approx(0.5F));
    REQUIRE(projection(1, 3) == approx(-2.F));
    REQUIRE(projection(2, 2) == approx(-0.5F));
    REQUIRE(projection(2, 3) == approx(-1.5F));
    REQUIRE(projection(3, 3) == approx(1.F));

    require_equal(projection.transform_point(Vector3{1.F, 2.F, -1.F}), Vector3{-1.F, -1.F, -1.F});
    require_equal(projection.transform_point(Vector3{3.F, 6.F, -5.F}), Vector3{1.F, 1.F, 1.F});
}

TEST_CASE("Matrix4 look_at", "[math][matrix4]")
{
    SECTION("identity view")
    {
        require_equal(Matrix4::look_at(Vector3{0.F, 0.F, 0.F}, Vector3{0.F, 0.F, -1.F},
                                       Vector3{0.F, 1.F, 0.F}),
                      Matrix4::identity());
    }

    SECTION("view from positive Z")
    {
        const Matrix4 view = Matrix4::look_at(Vector3{0.F, 0.F, 5.F}, Vector3{0.F, 0.F, 0.F},
                                              Vector3{0.F, 1.F, 0.F});

        require_equal(view.transform_point(Vector3{0.F, 0.F, 5.F}), Vector3{0.F, 0.F, 0.F});
        require_equal(view.transform_point(Vector3{0.F, 0.F, 0.F}), Vector3{0.F, 0.F, -5.F});
        require_equal(view.transform_point(Vector3{1.F, 0.F, 5.F}), Vector3{1.F, 0.F, 0.F});
        require_equal(view.transform_point(Vector3{0.F, 1.F, 5.F}), Vector3{0.F, 1.F, 0.F});
    }
}

TEST_CASE("Matrix4 transpose", "[math][matrix4]")
{
    Matrix4 matrix;
    for (int row = 0; row < 4; ++row)
    {
        for (int column = 0; column < 4; ++column)
        {
            matrix(row, column) = static_cast<Scalar>(row * 4 + column);
        }
    }

    const Matrix4 transposed = matrix.transposed();

    for (int row = 0; row < 4; ++row)
    {
        for (int column = 0; column < 4; ++column)
        {
            REQUIRE(transposed(row, column) == approx(matrix(column, row)));
        }
    }

    require_equal(transposed.transposed(), matrix);
}

TEST_CASE("Matrix4 determinant", "[math][matrix4]")
{
    REQUIRE(Matrix4::identity().determinant() == approx(1.F));
    REQUIRE(Matrix4::zero().determinant() == approx(0.F));
    REQUIRE(Matrix4::translation(Vector3{1.F, 2.F, 3.F}).determinant() == approx(1.F));
    REQUIRE(Matrix4::scaling(Vector3{2.F, 3.F, 4.F}).determinant() == approx(24.F));
    REQUIRE(Matrix4::rotation_z(Pi / 3.F).determinant() == approx(1.F));
}

TEST_CASE("Matrix4 inverse", "[math][matrix4]")
{
    SECTION("translation")
    {
        const Matrix4 translation = Matrix4::translation(Vector3{1.F, 2.F, 3.F});
        require_equal(translation.inverse(), Matrix4::translation(Vector3{-1.F, -2.F, -3.F}));
    }

    SECTION("scale")
    {
        const Matrix4 scale = Matrix4::scaling(Vector3{2.F, 4.F, 8.F});
        require_equal(scale.inverse(), Matrix4::scaling(Vector3{0.5F, 0.25F, 0.125F}));
    }

    SECTION("rotation")
    {
        const Matrix4 rotation = Matrix4::rotation_y(Pi / 3.F);
        require_equal(rotation.inverse(), rotation.transposed());
    }

    SECTION("composed transform")
    {
        const Matrix4 model = Matrix4::translation(Vector3{1.F, 2.F, 3.F}) *
                              Matrix4::rotation_z(Pi / 3.F) *
                              Matrix4::scaling(Vector3{2.F, 3.F, 4.F});

        require_equal(model * model.inverse(), Matrix4::identity());
        require_equal(model.inverse() * model, Matrix4::identity());
    }
}

TEST_CASE("Matrix4 arithmetic", "[math][matrix4]")
{
    SECTION("scalar multiplication")
    {
        const Matrix4 matrix = Matrix4::translation(Vector3{1.F, 2.F, 3.F});
        const Matrix4 scaled = matrix * 2.F;

        REQUIRE(scaled(0, 0) == approx(2.F));
        REQUIRE(scaled(1, 1) == approx(2.F));
        REQUIRE(scaled(2, 2) == approx(2.F));
        REQUIRE(scaled(3, 3) == approx(2.F));
        REQUIRE(scaled(0, 3) == approx(2.F));
        REQUIRE(scaled(1, 3) == approx(4.F));
        REQUIRE(scaled(2, 3) == approx(6.F));
    }

    SECTION("vector multiplication")
    {
        const Matrix4 translation = Matrix4::translation(Vector3{1.F, 2.F, 3.F});
        require_equal(translation * Vector4{1.F, 2.F, 3.F, 1.F}, Vector4{2.F, 4.F, 6.F, 1.F});

        const Matrix4 scale = Matrix4::scaling(Vector3{2.F, 3.F, 4.F});
        require_equal(scale * Vector4{1.F, 2.F, 3.F, 1.F}, Vector4{2.F, 6.F, 12.F, 1.F});
    }

    SECTION("matrix multiplication order")
    {
        const Matrix4 translation = Matrix4::translation(Vector3{1.F, 0.F, 0.F});
        const Matrix4 scale = Matrix4::scaling(Vector3{2.F, 2.F, 2.F});

        require_equal((translation * scale).transform_point(Vector3{1.F, 0.F, 0.F}),
                      Vector3{3.F, 0.F, 0.F});
        require_equal((scale * translation).transform_point(Vector3{1.F, 0.F, 0.F}),
                      Vector3{4.F, 0.F, 0.F});

        require_equal(translation * Matrix4::identity(), translation);
        require_equal(Matrix4::identity() * translation, translation);
    }

    SECTION("addition and subtraction")
    {
        const Matrix4 translation = Matrix4::translation(Vector3{1.F, 2.F, 3.F});
        const Matrix4 sum = translation + Matrix4::identity();
        const Matrix4 difference = translation - Matrix4::identity();

        REQUIRE(sum(0, 0) == approx(2.F));
        REQUIRE(sum(0, 3) == approx(1.F));
        REQUIRE(sum(1, 3) == approx(2.F));
        REQUIRE(sum(2, 3) == approx(3.F));

        REQUIRE(difference(0, 0) == approx(0.F));
        REQUIRE(difference(0, 3) == approx(1.F));
        REQUIRE(difference(1, 3) == approx(2.F));
        REQUIRE(difference(2, 3) == approx(3.F));
    }
}
