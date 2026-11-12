#pragma once

#include "matrix3.hpp"
#include "matrix4.hpp"
#include "scalar.hpp"
#include "vector3.hpp"

namespace tengine::math
{

struct Quaternion
{
    Scalar w{0.F};
    Scalar x{0.F};
    Scalar y{0.F};
    Scalar z{0.F};

    [[nodiscard]] constexpr Quaternion() noexcept = default;

    [[nodiscard]] constexpr explicit Quaternion(Scalar w, Scalar x, Scalar y, Scalar z) noexcept
        : w(w), x(x), y(y), z(z)
    {
    }

    // Construction
    [[nodiscard]] static constexpr Quaternion identity() noexcept;

    [[nodiscard]] static constexpr Quaternion from_axis_angle(const Vector3& axis,
                                                              Scalar angle) noexcept;

    [[nodiscard]] static constexpr Quaternion from_euler(const Vector3& euler_angles) noexcept;

    [[nodiscard]] static constexpr Quaternion from_rotation_matrix(const Matrix3& matrix) noexcept;

    // Properties
    [[nodiscard]] Scalar length() const noexcept;

    [[nodiscard]] Scalar length_squared() const noexcept;

    [[nodiscard]] Quaternion normalized() const noexcept;

    void normalize() noexcept;

    [[nodiscard]] Quaternion conjugate() const noexcept;

    [[nodiscard]] Quaternion inverse() const noexcept;

    // Transformations
    [[nodiscard]] Vector3 rotate(const Vector3& vector) const noexcept;

    [[nodiscard]] Matrix3 to_matrix3() const noexcept;

    [[nodiscard]] Matrix4 to_matrix4() const noexcept;

    [[nodiscard]] Vector3 to_euler() const noexcept;

    // Arithmatic
    [[nodiscard]] Quaternion operator*(const Quaternion& rhs) const noexcept;

    [[nodiscard]] Quaternion operator*(Scalar rhs) const noexcept;

    [[nodiscard]] Quaternion operator+(const Quaternion& rhs) const noexcept;
}; // struct Quaternion

// Quaternion utilities
[[nodiscard]] Scalar dot(const Quaternion& q1, const Quaternion& q2) noexcept;

[[nodiscard]] Quaternion slerp(const Quaternion& a, const Quaternion& b, Scalar t) noexcept;

}; // namespace tengine::math
