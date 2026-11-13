#include "tengine/math/matrix4.hpp"
#include "tengine/math/scalar.hpp"
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
