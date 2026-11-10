#pragma once

#include <array>
#include <cstddef>

#include "scalar.hpp"
#include "vector3.hpp"

namespace tengine::math
{

struct Matrix3
{
    static constexpr int element_count = 3 * 3;
    std::array<Scalar, element_count> elements{};

    constexpr Matrix3() noexcept = default;

    constexpr explicit Matrix3(Scalar diagonal) noexcept
        : elements{diagonal, 0, 0, 0, diagonal, 0, 0, 0, diagonal}
    {
    }

    // Element access
    [[nodiscard]] Scalar& operator()(int row, int column) noexcept;

    [[nodiscard]] const Scalar& operator()(int row, int column) const noexcept;

    // Factory methods
    [[nodiscard]] static constexpr Matrix3 identity() noexcept;

    [[nodiscard]] static constexpr Matrix3 zero() noexcept;

    // Matrix operations
    [[nodiscard]] Matrix3 transposed() const noexcept;

    [[nodiscard]] Scalar determinant() const noexcept;

    [[nodiscard]] Matrix3 inverse() const noexcept;

    // Arithmatic
    [[nodiscard]] Matrix3 operator*(Scalar rhs) const noexcept;

    [[nodiscard]] Vector3 operator*(const Vector3& rhs) const noexcept;

    [[nodiscard]] Matrix3 operator*(const Matrix3& rhs) const noexcept;

    [[nodiscard]] Matrix3 operator+(const Matrix3& rhs) const noexcept;

    [[nodiscard]] Matrix3 operator-(const Matrix3& rhs) const noexcept;
}; // struct Matrix3

using Mat3 = Matrix3;

}; // namespace tengine::math
