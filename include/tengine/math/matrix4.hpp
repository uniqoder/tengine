#pragma once

#include <array>

#include "scalar.hpp"
#include "vector3.hpp"
#include "vector4.hpp"

namespace tengine::math
{

struct Matrix4
{
    static constexpr int element_count = 4 * 4;
    std::array<Scalar, element_count> elements{};

    constexpr Matrix4() noexcept = default;

    constexpr explicit Matrix4(Scalar diagonal) noexcept
        : elements{diagonal, 0, 0, 0, 0, diagonal, 0, 0, 0, 0, diagonal, 0, 0, 0, 0, diagonal}
    {
    }

    // Element access
    [[nodiscard]] Scalar& operator()(int row, int column) noexcept;

    [[nodiscard]] const Scalar& operator()(int row, int column) const noexcept;

    // Factory methods //
    [[nodiscard]] static constexpr Matrix4 identity() noexcept;

    [[nodiscard]] static constexpr Matrix4 zero() noexcept;

    [[nodiscard]] static Matrix4 translation(const Vector3& offset) noexcept;

    [[nodiscard]] static Matrix4 scale(const Vector3& factors) noexcept;

    [[nodiscard]] static Matrix4 rotation_x(Scalar angle) noexcept;

    [[nodiscard]] static Matrix4 rotation_y(Scalar angle) noexcept;

    [[nodiscard]] static Matrix4 rotation_z(Scalar angle) noexcept;

    // Projection
    [[nodiscard]] static Matrix4 perspective(Scalar vertical_fov, Scalar aspect_ratio,
                                             Scalar near_plane, Scalar far_plane) noexcept;

    [[nodiscard]] static Matrix4 orthographic(Scalar left, Scalar right, Scalar bottom, Scalar top,
                                              Scalar near_plane, Scalar far_plane) noexcept;

    // Camera
    [[nodiscard]] static Matrix4 look_at(const Vector3& eye, const Vector3& target,
                                         const Vector3& up) noexcept;

    // Matrix operations //
    [[nodiscard]] Matrix4 transposed() const noexcept;

    [[nodiscard]] Scalar determinant() const noexcept;

    [[nodiscard]] Matrix4 inverse() const noexcept;

    // Transformations
    [[nodiscard]] Vector3 transform_point(const Vector3& point) const noexcept;

    [[nodiscard]] Vector3 transform_vector(const Vector3& vector) const noexcept;

    // Arithmatic //
    [[nodiscard]] Matrix4 operator*(Scalar rhs) const noexcept;

    [[nodiscard]] Vector4 operator*(const Vector4& rhs) const noexcept;

    [[nodiscard]] Matrix4 operator*(const Matrix4& rhs) const noexcept;

    [[nodiscard]] Matrix4 operator+(const Matrix4& rhs) const noexcept;

    [[nodiscard]] Matrix4 operator-(const Matrix4& rhs) const noexcept;
}; // struct Matrix4

using Mat4 = Matrix4;

}; // namespace tengine::math
