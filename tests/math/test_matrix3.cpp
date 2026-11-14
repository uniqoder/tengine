#include <catch2/catch_test_macros.hpp>

#include "tengine/math/matrix3.hpp"
#include "tengine/math/scalar.hpp"
#include "tengine/math/vector3.hpp"

#include "test_helpers.hpp"

using namespace tengine::math;
using namespace tengine::test;

TEST_CASE("Matrix3 construction", "[math][matrix3]")
{
    SECTION("default constructor produces a zero matrix")
    {
        const Matrix3 matrix;
        require_equal(matrix, Matrix3::zero());
    }

    SECTION("diagonal scalar constructor")
    {
        const Matrix3 matrix(2.F);

        Matrix3 expected({{{2.F, 0.F, 0.F}, {0.F, 2.F, 0.F}, {0.F, 0.F, 2.F}}});
        require_equal(matrix, expected);
    }

    SECTION("diagonal vector constructor")
    {
        const Matrix3 matrix(Vector3{2.F, 3.F, 4.F});

        Matrix3 expected({{{2.F, 0.F, 0.F}, {0.F, 3.F, 0.F}, {0.F, 0.F, 4.F}}});
        require_equal(matrix, expected);
    }

    SECTION("identity and zero")
    {
        Matrix3 expected_identity({{{1.F, 0.F, 0.F}, {0.F, 1.F, 0.F}, {0.F, 0.F, 1.F}}});

        require_equal(Matrix3::identity(), expected_identity);
        require_equal(Matrix3::zero(), Matrix3(0.F));
    }
}

TEST_CASE("Matrix3 element access", "[math][matrix3]")
{
    Matrix3 matrix;
    matrix(0, 0) = 1.F;
    matrix(0, 1) = 2.F;
    matrix(0, 2) = 3.F;
    matrix(1, 0) = 4.F;
    matrix(1, 1) = 5.F;
    matrix(1, 2) = 6.F;
    matrix(2, 0) = 7.F;
    matrix(2, 1) = 8.F;
    matrix(2, 2) = 9.F;

    REQUIRE(matrix(0, 0) == 1.F);
    REQUIRE(matrix(0, 1) == 2.F);
    REQUIRE(matrix(0, 2) == 3.F);
    REQUIRE(matrix(1, 0) == 4.F);
    REQUIRE(matrix(1, 1) == 5.F);
    REQUIRE(matrix(1, 2) == 6.F);
    REQUIRE(matrix(2, 0) == 7.F);
    REQUIRE(matrix(2, 1) == 8.F);
    REQUIRE(matrix(2, 2) == 9.F);

    const Matrix3& const_matrix = matrix;
    REQUIRE(const_matrix(0, 0) == 1.F);
    REQUIRE(const_matrix(1, 2) == 6.F);
    REQUIRE(const_matrix(2, 1) == 8.F);
}

TEST_CASE("Matrix3 rotation factories", "[math][matrix3]")
{
    SECTION("zero angle produces identity")
    {
        require_equal(Matrix3::rotation_x(0.F), Matrix3::identity());
        require_equal(Matrix3::rotation_y(0.F), Matrix3::identity());
        require_equal(Matrix3::rotation_z(0.F), Matrix3::identity());
    }

    SECTION("rotation matrices at ninety degrees")
    {
        Matrix3 expected_x({{{1.F, 0.F, 0.F}, {0.F, 0.F, -1.F}, {0.F, 1.F, 0.F}}});
        require_equal(Matrix3::rotation_x(HalfPi), expected_x);

        Matrix3 expected_y({{{0.F, 0.F, 1.F}, {0.F, 1.F, 0.F}, {-1.F, 0.F, 0.F}}});
        require_equal(Matrix3::rotation_y(HalfPi), expected_y);

        Matrix3 expected_z({{{0.F, -1.F, 0.F}, {1.F, 0.F, 0.F}, {0.F, 0.F, 1.F}}});
        require_equal(Matrix3::rotation_z(HalfPi), expected_z);
    }

    SECTION("rotation of basis vectors")
    {
        require_equal(Matrix3::rotation_z(HalfPi) * Vector3::right(), Vector3::up());
        require_equal(Matrix3::rotation_x(HalfPi) * Vector3::up(), Vector3{0.F, 0.F, 1.F});
        require_equal(Matrix3::rotation_y(HalfPi) * Vector3::forward(), Vector3{-1.F, 0.F, 0.F});
        require_equal(Matrix3::rotation_z(Pi) * Vector3::right(), Vector3{-1.F, 0.F, 0.F});
    }
}

TEST_CASE("Matrix3 scale factory", "[math][matrix3]")
{
    const Matrix3 scale = Matrix3::scaling(Vector3{2.F, 3.F, 4.F});

    Matrix3 expected({{{2.F, 0.F, 0.F}, {0.F, 3.F, 0.F}, {0.F, 0.F, 4.F}}});
    require_equal(scale, expected);

    require_equal(scale * Vector3{1.F, 1.F, 1.F}, Vector3{2.F, 3.F, 4.F});
    require_equal(scale * Vector3{1.F, 2.F, 3.F}, Vector3{2.F, 6.F, 12.F});
}

TEST_CASE("Matrix3 transpose", "[math][matrix3]")
{
    Matrix3 matrix({{{1.F, 2.F, 3.F}, {4.F, 5.F, 6.F}, {7.F, 8.F, 9.F}}});

    const Matrix3 transposed = matrix.transposed();

    for (int row = 0; row < 3; ++row)
    {
        for (int column = 0; column < 3; ++column)
        {
            REQUIRE(transposed(row, column) == approx(matrix(column, row)));
        }
    }

    require_equal(transposed.transposed(), matrix);
    require_equal(Matrix3::identity().transposed(), Matrix3::identity());
}

TEST_CASE("Matrix3 determinant", "[math][matrix3]")
{
    REQUIRE(Matrix3::identity().determinant() == approx(1.F));
    REQUIRE(Matrix3::zero().determinant() == approx(0.F));

    Matrix3 diagonal({{{2.F, 0.F, 0.F}, {0.F, 3.F, 0.F}, {0.F, 0.F, 4.F}}});
    REQUIRE(diagonal.determinant() == approx(24.F));

    Matrix3 matrix({{{1.F, 2.F, 3.F}, {0.F, 1.F, 4.F}, {5.F, 6.F, 0.F}}});
    REQUIRE(matrix.determinant() == approx(1.F));

    Matrix3 singular({{{1.F, 2.F, 3.F}, {4.F, 5.F, 6.F}, {7.F, 8.F, 9.F}}});
    REQUIRE(singular.determinant() == approx(0.F));
}

TEST_CASE("Matrix3 inverse", "[math][matrix3]")
{
    SECTION("identity")
    {
        require_equal(Matrix3::identity().inverse(), Matrix3::identity());
    }

    SECTION("diagonal")
    {
        Matrix3 diagonal({{{2.F, 0.F, 0.F}, {0.F, 4.F, 0.F}, {0.F, 0.F, 8.F}}});

        Matrix3 expected({{{0.5F, 0.F, 0.F}, {0.F, 0.25F, 0.F}, {0.F, 0.F, 0.125F}}});
        require_equal(diagonal.inverse(), expected);
    }

    SECTION("general matrix")
    {
        Matrix3 matrix({{{1.F, 2.F, 3.F}, {0.F, 1.F, 4.F}, {5.F, 6.F, 0.F}}});

        const Matrix3 inverse = matrix.inverse();

        require_equal(matrix * inverse, Matrix3::identity());
        require_equal(inverse * matrix, Matrix3::identity());
    }

    SECTION("rotation")
    {
        const Matrix3 rotation = Matrix3::rotation_z(Pi / 3.F);

        require_equal(rotation.inverse(), rotation.transposed());
        require_equal(rotation.inverse(), Matrix3::rotation_z(-Pi / 3.F));
    }
}

TEST_CASE("Matrix3 arithmetic", "[math][matrix3]")
{
    const Matrix3 matrix = []
    {
        Matrix3 result({{{1.F, 2.F, 3.F}, {4.F, 5.F, 6.F}, {7.F, 8.F, 9.F}}});
        return result;
    }();

    SECTION("scalar multiplication")
    {
        Matrix3 expected({{{2.F, 4.F, 6.F}, {8.F, 10.F, 12.F}, {14.F, 16.F, 18.F}}});
        require_equal(matrix * 2.F, expected);
    }

    SECTION("vector multiplication")
    {
        require_equal(matrix * Vector3{1.F, 2.F, 3.F}, Vector3{14.F, 32.F, 50.F});
    }

    SECTION("matrix multiplication")
    {
        Matrix3 rhs({{{9.F, 8.F, 7.F}, {6.F, 5.F, 4.F}, {3.F, 2.F, 1.F}}});

        Matrix3 expected({{{30.F, 24.F, 18.F}, {84.F, 69.F, 54.F}, {138.F, 114.F, 90.F}}});
        require_equal(matrix * rhs, expected);

        require_equal(matrix * Matrix3::identity(), matrix);
        require_equal(Matrix3::identity() * matrix, matrix);
    }

    SECTION("addition and subtraction")
    {
        const Matrix3 other = Matrix3::identity();

        Matrix3 expected_sum({{{2.F, 2.F, 3.F}, {4.F, 6.F, 6.F}, {7.F, 8.F, 10.F}}});
        require_equal(matrix + other, expected_sum);

        Matrix3 expected_difference({{{0.F, 2.F, 3.F}, {4.F, 4.F, 6.F}, {7.F, 8.F, 8.F}}});
        require_equal(matrix - other, expected_difference);
    }
}
