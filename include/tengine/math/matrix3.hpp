#pragma once

#include <array>
#include <cassert>
#include <cmath>

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

    constexpr explicit Matrix3(const Vector3& diagonal) noexcept
        : elements{diagonal.x, 0, 0, 0, diagonal.y, 0, 0, 0, diagonal.z}
    {
    }

    // Element access
    [[nodiscard]] Scalar& operator()(int row, int column) noexcept;

    [[nodiscard]] const Scalar& operator()(int row, int column) const noexcept;

    // Factory methods
    [[nodiscard]] static constexpr Matrix3 identity() noexcept
    {
        return Matrix3(1);
    }

    [[nodiscard]] static constexpr Matrix3 zero() noexcept
    {
        return Matrix3(0);
    }

    [[nodiscard]] static constexpr Matrix3 rot_x_mat(Scalar angle) noexcept
    {
        Matrix3 rotation;
        rotation.elements = {
            1, 0, 0, 0, std::cos(angle), -std::sin(angle), 0, std::sin(angle), std::cos(angle)};
        return rotation;
    }

    [[nodiscard]] static constexpr Matrix3 rot_y_mat(Scalar angle) noexcept
    {
        Matrix3 rotation;
        rotation.elements = {std::cos(angle),  0, std::sin(angle), 0, 1, 0,
                             -std::sin(angle), 0, std::cos(angle)};
        return rotation;
    }

    [[nodiscard]] static constexpr Matrix3 rot_z_mat(Scalar angle) noexcept
    {
        Matrix3 rotation;
        rotation.elements = {
            std::cos(angle), -std::sin(angle), 0, std::sin(angle), std::cos(angle), 0, 0, 0, 1};
        return rotation;
    }

    [[nodiscard]] static constexpr Matrix3 scale_mat(const Vector3& scale)
    {
        return Matrix3(scale);
    }

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
