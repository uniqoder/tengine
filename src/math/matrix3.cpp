#include "tengine/math/matrix3.hpp"
#include "tengine/math/scalar.hpp"
#include <cassert>
#include <cmath>
#include <cstdlib>

namespace tengine::math
{

// Access
Scalar& Matrix3::operator()(int row, int column) noexcept
{
    const int dim = 3;
    assert(row < dim);
    assert(column < dim);

    const size_t idx = size_t(row) * size_t(dim) + size_t(column);
    return elements[idx];
}

const Scalar& Matrix3::operator()(int row, int column) const noexcept
{
    const int dim = 3;
    assert(row < dim);
    assert(column < dim);

    const size_t idx = size_t(row) * size_t(dim) + size_t(column);
    return elements[idx];
}

// Matrix operations
Matrix3 Matrix3::transposed() const noexcept
{
    Matrix3 trans;

    const int dim = 3;
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

Scalar Matrix3::determinant() const noexcept
{
    const auto& self = *this;
    return (self(0, 0) * (self(1, 1) * self(2, 2) - self(1, 2) * self(2, 1)) -
            self(0, 1) * (self(1, 0) * self(2, 2) - self(1, 2) * self(2, 0)) +
            self(0, 2) * (self(1, 0) * self(2, 1) - self(1, 1) * self(2, 0)));
}

Matrix3 Matrix3::inverse() const noexcept
{
    Matrix3 A = *this;
    Matrix3 I = identity();

    int dim = 3;
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

// Arithmatic

Matrix3 Matrix3::operator*(Scalar rhs) const noexcept
{
    Matrix3 result = *this;
    for (Scalar& element : result.elements)
    {
        element *= rhs;
    }

    return result;
}
Vector3 Matrix3::operator*(const Vector3& rhs) const noexcept
{
    Vector3 result;
    const int dim = 3;
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            result[i] += (*this)(i, j) * rhs[j];
        }
    }

    return result;
}

Matrix3 Matrix3::operator*(const Matrix3& rhs) const noexcept
{
    Matrix3 result;
    const int dim = 3;
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

Matrix3 Matrix3::operator+(const Matrix3& rhs) const noexcept
{
    Matrix3 result = *this;
    for (size_t i = 0; i < element_count; i++)
    {
        result.elements[i] += rhs.elements[i];
    }

    return result;
}

Matrix3 Matrix3::operator-(const Matrix3& rhs) const noexcept
{
    Matrix3 result = *this;
    for (size_t i = 0; i < element_count; i++)
    {
        result.elements[i] -= rhs.elements[i];
    }

    return result;
}

}; // namespace tengine::math
