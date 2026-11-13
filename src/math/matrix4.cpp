#include "tengine/math/matrix4.hpp"
#include "tengine/math/scalar.hpp"
#include "tengine/math/vector3.hpp"
#include "tengine/math/vector4.hpp"
#include <cassert>
#include <cmath>
#include <cstdlib>

namespace tengine::math
{

// Access
Scalar& Matrix4::operator()(int row, int column) noexcept
{
    const int dim = 4;
    assert(row >= 0 && row < dim);
    assert(column >= 0 && column < dim);

    const size_t idx = size_t(row) * size_t(dim) + size_t(column);
    return elements[idx];
}

const Scalar& Matrix4::operator()(int row, int column) const noexcept
{
    const int dim = 4;
    assert(row >= 0 && row < dim);
    assert(column >= 0 && column < dim);

    const size_t idx = size_t(row) * size_t(dim) + size_t(column);
    return elements[idx];
}

// Projection

Matrix4 Matrix4::perspective(Scalar vertical_fov, Scalar aspect_ratio, Scalar near_plane,
                             Scalar far_plane) noexcept
{
    const Scalar tan_half_fov = std::tan(vertical_fov / Scalar(2));
    const Scalar fn_sum = far_plane + near_plane;
    const Scalar fn_diff = far_plane - near_plane;
    const Scalar fn_prod = far_plane * near_plane;
    return std::array4({{1 / (aspect_ratio * tan_half_fov), 0, 0, 0},
                        {0, 1 / tan_half_fov, 0, 0},
                        {0, 0, -fn_sum / fn_diff, -2 * fn_prod / fn_diff},
                        {0, 0, -1, 0}});
}

Matrix4 Matrix4::orthographic(Scalar left, Scalar right, Scalar bottom, Scalar top,
                              Scalar near_plane, Scalar far_plane) noexcept
{
    const Scalar rl_diff = right - left;
    const Scalar rl_sum = right + left;
    const Scalar tb_diff = top - bottom;
    const Scalar tb_sum = top + bottom;
    const Scalar fn_diff = far_plane - near_plane;
    const Scalar fn_sum = far_plane + near_plane;

    return std::array4({{2 / rl_diff, 0, 0, -rl_sum / rl_diff},
                        {0, 2 / tb_diff, 0, -tb_sum / tb_diff},
                        {0, 0, -2 / fn_diff, -fn_sum / fn_diff},
                        {0, 0, 0, 1}});
}

// Matrix operations
Matrix4 Matrix4::transposed() const noexcept
{
    Matrix4 trans;

    const int dim = 4;
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            trans(i, j) = (*this)(j, i);
            trans(j, i) = (*this)(i, j);
        }
    }

    return trans;
}

Matrix4 Matrix4::inverse() const noexcept
{
    Matrix4 A = *this;
    Matrix4 I = identity();

    int dim = 4;
    for (int i = 0; i < dim; i++)
    {
        // Find row with largest absolute value at pivot
        int pivotRow = i;
        for (int k = i + 1; k < dim; k++)
        {
            if (std::abs(A(k, i)) > std::abs(A(pivotRow, i)))
            {
                pivotRow = k;
            }
        }

        // Check matrix is not singular
        assert(!nearly_equal(std::abs(A(pivotRow, i)), 0));

        // Swap current row with pivot row
        if (pivotRow != i)
        {
            for (int j = 0; j < dim; j++)
            {
                // Swap elements of A
                Scalar tmp = A(i, j);
                A(i, j) = A(pivotRow, j);
                A(pivotRow, j) = tmp;

                // Swap elements of I
                tmp = I(i, j);
                I(i, j) = I(pivotRow, j);
                I(pivotRow, j) = tmp;
            }
        }

        // Scale pivot row to make pivot element 1
        Scalar pivot = A(i, i);
        for (int j = 0; j < dim; j++)
        {
            A(i, j) /= pivot;
            I(i, j) /= pivot;
        }

        // Eliminate all other values in column i
        for (int k = 0; k < dim; k++)
        {
            if (k == i)
            {
                continue;
            }

            Scalar factor = A(k, i);
            for (int j = 0; j < dim; j++)
            {
                A(k, j) -= factor * A(i, j);
                I(k, j) -= factor * I(i, j);
            }
        }
    }

    return I;
}

// Transformations
Vector3 Matrix4::transform_point(const Vector3& point) const noexcept
{
    Matrix4 point_transformer = *this;
    point_transformer(3, 3) = 1.F;

    Vector4 v4_point{point, 1.F};

    Vector4 transformed_point = point_transformer * v4_point;
    return transformed_point.xyz();
}

Vector3 Matrix4::transform_vector(const Vector3& vector) const noexcept
{
    Matrix4 vector_transformer = *this;
    vector_transformer(3, 3) = 0;

    Vector4 v4_vector{vector, 1.F};

    Vector4 transformed_vector = vector_transformer * v4_vector;
    return transformed_vector.xyz();
}

// Arithmatic
Matrix4 Matrix4::operator*(Scalar rhs) const noexcept
{
    Matrix4 result = *this;
    for (Scalar& element : result.elements)
    {
        element *= rhs;
    }

    return result;
}
Vector4 Matrix4::operator*(const Vector4& rhs) const noexcept
{
    Vector4 result;
    const int dim = 4;
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            result[i] += (*this)(i, j) * rhs[j];
        }
    }

    return result;
}

Matrix4 Matrix4::operator*(const Matrix4& rhs) const noexcept
{
    Matrix4 result;
    const int dim = 4;
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            for (int k = 0; k < dim; k++)
            {
                result(j, i) += (*this)(j, k) * rhs(k, i);
            }
        }
    }

    return result;
}

Matrix4 Matrix4::operator+(const Matrix4& rhs) const noexcept
{
    Matrix4 result = *this;
    for (size_t i = 0; i < element_count; i++)
    {
        result.elements[i] += rhs.elements[i];
    }

    return result;
}

Matrix4 Matrix4::operator-(const Matrix4& rhs) const noexcept
{
    Matrix4 result = *this;
    for (size_t i = 0; i < element_count; i++)
    {
        result.elements[i] -= rhs.elements[i];
    }

    return result;
}

}; // namespace tengine::math
