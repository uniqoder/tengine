#pragma once

#include <array>

#include "scalar.hpp"
#include "vector3.hpp"
#include "vector4.hpp"

namespace std
{

typedef std::array<std::array<tengine::math::Scalar, 4>, 4> array4;

};

namespace tengine::math
{

struct Matrix4
{
  private:
    static constexpr int element_count = 4 * 4;
    std::array<Scalar, element_count> elements{};

  public:
    constexpr Matrix4() noexcept = default;

    constexpr explicit Matrix4(Scalar diagonal) noexcept
        : elements{diagonal, 0, 0, 0, 0, diagonal, 0, 0, 0, 0, diagonal, 0, 0, 0, 0, diagonal}
    {
    }

    constexpr Matrix4(const Vector4& diagonal) noexcept
        : elements{diagonal.x, 0, 0,          0, 0, diagonal.y, 0, 0,
                   0,          0, diagonal.z, 0, 0, 0,          0, diagonal.w}
    {
    }

    constexpr Matrix4(const std::array4& elements) noexcept
    {
        for (size_t i = 0; i < 4; i++)
        {
            for (size_t j = 0; j < 4; j++)
            {
                this->elements[4 * i + j] = elements[i][j];
            }
        }
    }

    // Element access
    [[nodiscard]] Scalar& operator()(int row, int column) noexcept;

    [[nodiscard]] const Scalar& operator()(int row, int column) const noexcept;

    // Factory methods //
    [[nodiscard]] static constexpr Matrix4 identity() noexcept
    {
        return Matrix4(1);
    }

    [[nodiscard]] static constexpr Matrix4 zero() noexcept
    {
        return Matrix4(0);
    }

    [[nodiscard]] static constexpr Matrix4 translation(const Vector3& offset) noexcept
    {
        return Matrix4(
            {{{0, 0, 0, offset.x}, {0, 0, 0, offset.y}, {0, 0, 0, offset.z}, {0, 0, 0, 1}}});
    }

    [[nodiscard]] static constexpr Matrix4 rotation_x(Scalar angle) noexcept
    {
        return Matrix4({{{1.F, 0, 0, 0},
                         {0, std::cos(angle), -std::sin(angle), 0},
                         {0, std::sin(angle), std::cos(angle), 0},
                         {0, 0, 0, 1.F}}});
    }

    [[nodiscard]] static constexpr Matrix4 rotation_y(Scalar angle) noexcept
    {
        return Matrix4({{{std::cos(angle), 0, std::sin(angle), 0},
                         {0, 1.F, 0, 0},
                         {-std::sin(angle), 0, std::cos(angle), 0},
                         {0, 0, 0, 1.F}}});
    }

    [[nodiscard]] static constexpr Matrix4 rotation_z(Scalar angle) noexcept
    {
        return Matrix4({{{std::cos(angle), -std::sin(angle), 0, 0},
                         {std::sin(angle), std::cos(angle), 0, 0},
                         {0, 0, 1.F, 0},
                         {0, 0, 0, 1.F}}});
    }

    [[nodiscard]] static constexpr Matrix4 scaling(const Vector3& scale)
    {
        return Vector4(scale.x, scale.y, scale.z, 1);
    }

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
