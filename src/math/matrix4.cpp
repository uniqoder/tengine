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

Scalar Matrix4::determinant() const noexcept
{
    const auto& m = elements;

    // Precompute the sub-determinants of rows 3 and 4
    const Scalar s0 = m[8] * m[13] - m[9] * m[12];
    const Scalar s1 = m[8] * m[14] - m[10] * m[12];
    const Scalar s2 = m[8] * m[15] - m[11] * m[12];
    const Scalar s3 = m[9] * m[14] - m[10] * m[13];
    const Scalar s4 = m[9] * m[15] - m[11] * m[13];
    const Scalar s5 = m[10] * m[15] - m[11] * m[14];

    // Combine with rows 1 and 2 using Laplace expansion
    return (
        m[0] * (m[5] * s5 - m[6] * s4 + m[7] * s3) - m[1] * (m[4] * s5 - m[6] * s2 + m[7] * s1) +
        m[2] * (m[4] * s4 - m[5] * s2 + m[7] * s0) - m[3] * (m[4] * s3 - m[5] * s1 + m[6] * s0));
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
    Vector4 coords{point, 1.F};

    Vector4 transformed_coords = *this * coords;
    return transformed_coords.xyz();
}

Vector3 Matrix4::transform_vector(const Vector3& vector) const noexcept
{
    Vector4 coords{vector, 0};

    Vector4 transformed_coords = *this * coords;
    return transformed_coords.xyz();
}

// Camera
Matrix4 Matrix4::look_at(const Vector3& eye, const Vector3& target, const Vector3& up) noexcept
{
    const Vector3 F = (target - eye).normalized();
    const Vector3 R = cross(F, up).normalized();
    const Vector3 U = cross(R, F);

    return std::array4({{R.x, R.y, R.z, dot(-R, eye)},
                        {U.x, U.y, U.z, dot(-U, eye)},
                        {-F.x, -F.y, -F.z, dot(F, eye)},
                        {0, 0, 0, 1}});
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
