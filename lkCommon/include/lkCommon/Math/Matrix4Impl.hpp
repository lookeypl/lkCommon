#pragma once

#ifndef _LKCOMMON_MATH_MATRIX4_HPP_
#error "Please include main header of Matrix4, not the implementation header."
#endif // _LKCOMMON_MATH_MATRIX4_HPP_

#include "Matrix4.hpp"


namespace lkCommon {
namespace Math {

LKCOMMON_INLINE Matrix4::Matrix4()
    : v()
{
}

LKCOMMON_INLINE Matrix4::Matrix4(const float a)
    : v(_mm_set1_ps(a), _mm_set1_ps(a), _mm_set1_ps(a), _mm_set1_ps(a))
{
}

LKCOMMON_INLINE Matrix4::Matrix4(const float* a)
    : v(_mm_load_ps(a), _mm_load_ps(a + 4), _mm_load_ps(a + 8), _mm_load_ps(a + 12))
{
}

LKCOMMON_INLINE Matrix4::Matrix4(const float a1, const float a2, const float a3, const float a4,
                                 const float b1, const float b2, const float b3, const float b4,
                                 const float c1, const float c2, const float c3, const float c4,
                                 const float d1, const float d2, const float d3, const float d4)
    : v(_mm_set_ps(d1, c1, b1, a1),
        _mm_set_ps(d2, c2, b2, a2),
        _mm_set_ps(d3, c3, b3, a3),
        _mm_set_ps(d4, c4, b4, a4))
{
}

LKCOMMON_INLINE Matrix4::Matrix4(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d)
    : v(a, b, c, d)
{
}

LKCOMMON_INLINE Matrix4::Matrix4(const __m128& a, const __m128& b, const __m128& c, const __m128& d)
    : v(a, b, c, d)
{
}

LKCOMMON_INLINE Matrix4::Matrix4(const Matrix4& other)
    : v(other.v.m[0], other.v.m[1], other.v.m[2], other.v.m[3])
{
}

LKCOMMON_INLINE Matrix4::Matrix4(Matrix4&& other) noexcept
    : v(std::move(other.v.m[0]), std::move(other.v.m[1]), std::move(other.v.m[2]), std::move(other.v.m[3]))
{
}

LKCOMMON_INLINE Matrix4& Matrix4::operator=(const Matrix4& other)
{
    v.m[0] = other.v.m[0];
    v.m[1] = other.v.m[1];
    v.m[2] = other.v.m[2];
    v.m[3] = other.v.m[3];
    return *this;
}

LKCOMMON_INLINE Matrix4& Matrix4::operator=(Matrix4&& other) noexcept
{
    v.m[0] = std::move(other.v.m[0]);
    v.m[1] = std::move(other.v.m[1]);
    v.m[2] = std::move(other.v.m[2]);
    v.m[3] = std::move(other.v.m[3]);
    return *this;
}

LKCOMMON_INLINE const float* Matrix4::Data() const
{
    return v.f;
}

// Access operators
LKCOMMON_INLINE Vector4& Matrix4::operator[](const size_t index)
{
    return v.v[index];
}

LKCOMMON_INLINE const Vector4& Matrix4::operator[](const size_t index) const
{
    return v.v[index];
}

// Addition
LKCOMMON_INLINE Matrix4& Matrix4::operator+=(const Matrix4& other)
{
    v.m[0] = _mm_add_ps(v.m[0], other.v.m[0]);
    v.m[1] = _mm_add_ps(v.m[1], other.v.m[1]);
    v.m[2] = _mm_add_ps(v.m[2], other.v.m[2]);
    v.m[3] = _mm_add_ps(v.m[3], other.v.m[3]);
    return *this;
}

LKCOMMON_INLINE Matrix4& Matrix4::operator+=(const float value)
{
    v.m[0] = _mm_add_ps(v.m[0], _mm_set_ps1(value));
    v.m[1] = _mm_add_ps(v.m[1], _mm_set_ps1(value));
    v.m[2] = _mm_add_ps(v.m[2], _mm_set_ps1(value));
    v.m[3] = _mm_add_ps(v.m[3], _mm_set_ps1(value));
    return *this;
}

LKCOMMON_INLINE Matrix4 Matrix4::operator+(const Matrix4& other) const
{
    return Matrix4(
        _mm_add_ps(v.m[0], other.v.m[0]),
        _mm_add_ps(v.m[1], other.v.m[1]),
        _mm_add_ps(v.m[2], other.v.m[2]),
        _mm_add_ps(v.m[3], other.v.m[3])
    );
}

LKCOMMON_INLINE Matrix4 Matrix4::operator+(const float value) const
{
    return Matrix4(
        _mm_add_ps(v.m[0], _mm_set_ps1(value)),
        _mm_add_ps(v.m[1], _mm_set_ps1(value)),
        _mm_add_ps(v.m[2], _mm_set_ps1(value)),
        _mm_add_ps(v.m[3], _mm_set_ps1(value))
    );
}

// Subtraction
LKCOMMON_INLINE Matrix4& Matrix4::operator-=(const Matrix4& other)
{
    v.m[0] = _mm_sub_ps(v.m[0], other.v.m[0]);
    v.m[1] = _mm_sub_ps(v.m[1], other.v.m[1]);
    v.m[2] = _mm_sub_ps(v.m[2], other.v.m[2]);
    v.m[3] = _mm_sub_ps(v.m[3], other.v.m[3]);
    return *this;
}

LKCOMMON_INLINE Matrix4& Matrix4::operator-=(const float value)
{
    v.m[0] = _mm_sub_ps(v.m[0], _mm_set_ps1(value));
    v.m[1] = _mm_sub_ps(v.m[1], _mm_set_ps1(value));
    v.m[2] = _mm_sub_ps(v.m[2], _mm_set_ps1(value));
    v.m[3] = _mm_sub_ps(v.m[3], _mm_set_ps1(value));
    return *this;
}

LKCOMMON_INLINE Matrix4 Matrix4::operator-(const Matrix4& other) const
{
    return Matrix4(
        _mm_sub_ps(v.m[0], other.v.m[0]),
        _mm_sub_ps(v.m[1], other.v.m[1]),
        _mm_sub_ps(v.m[2], other.v.m[2]),
        _mm_sub_ps(v.m[3], other.v.m[3])
    );
}

LKCOMMON_INLINE Matrix4 Matrix4::operator-(const float value) const
{
    return Matrix4(
        _mm_sub_ps(v.m[0], _mm_set_ps1(value)),
        _mm_sub_ps(v.m[1], _mm_set_ps1(value)),
        _mm_sub_ps(v.m[2], _mm_set_ps1(value)),
        _mm_sub_ps(v.m[3], _mm_set_ps1(value))
    );
}


// Multiplication
LKCOMMON_INLINE Matrix4& Matrix4::operator*=(const Matrix4& other)
{
    const __m128 vm[4] = { v.m[0], v.m[1], v.m[2], v.m[3] };
    v.m[0] = _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(_mm_set1_ps(other.v.f[0]), vm[0]),
            _mm_mul_ps(_mm_set1_ps(other.v.f[1]), vm[1])
        ),
        _mm_add_ps(
            _mm_mul_ps(_mm_set1_ps(other.v.f[2]), vm[2]),
            _mm_mul_ps(_mm_set1_ps(other.v.f[3]), vm[3])
        )
    );
    v.m[1] = _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(_mm_set1_ps(other.v.f[4]), vm[0]),
            _mm_mul_ps(_mm_set1_ps(other.v.f[5]), vm[1])
        ),
        _mm_add_ps(
            _mm_mul_ps(_mm_set1_ps(other.v.f[6]), vm[2]),
            _mm_mul_ps(_mm_set1_ps(other.v.f[7]), vm[3])
        )
    );
    v.m[2] = _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(_mm_set1_ps(other.v.f[8]), vm[0]),
            _mm_mul_ps(_mm_set1_ps(other.v.f[9]), vm[1])
        ),
        _mm_add_ps(
            _mm_mul_ps(_mm_set1_ps(other.v.f[10]), vm[2]),
            _mm_mul_ps(_mm_set1_ps(other.v.f[11]), vm[3])
        )
    );
    v.m[3] = _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(_mm_set1_ps(other.v.f[12]), vm[0]),
            _mm_mul_ps(_mm_set1_ps(other.v.f[13]), vm[1])
        ),
        _mm_add_ps(
            _mm_mul_ps(_mm_set1_ps(other.v.f[14]), vm[2]),
            _mm_mul_ps(_mm_set1_ps(other.v.f[15]), vm[3])
        )
    );

    return *this;
}

LKCOMMON_INLINE Matrix4& Matrix4::operator*=(const float value)
{
    v.m[0] = _mm_mul_ps(v.m[0], _mm_set_ps1(value));
    v.m[1] = _mm_mul_ps(v.m[1], _mm_set_ps1(value));
    v.m[2] = _mm_mul_ps(v.m[2], _mm_set_ps1(value));
    v.m[3] = _mm_mul_ps(v.m[3], _mm_set_ps1(value));
    return *this;
}

LKCOMMON_INLINE Matrix4 Matrix4::operator*(const Matrix4& other) const
{
    return Matrix4(
        _mm_add_ps(
            _mm_add_ps(
                _mm_mul_ps(_mm_set1_ps(other.v.f[0]), v.m[0]),
                _mm_mul_ps(_mm_set1_ps(other.v.f[1]), v.m[1])
            ),
            _mm_add_ps(
                _mm_mul_ps(_mm_set1_ps(other.v.f[2]), v.m[2]),
                _mm_mul_ps(_mm_set1_ps(other.v.f[3]), v.m[3])
            )
        ),
        _mm_add_ps(
            _mm_add_ps(
                _mm_mul_ps(_mm_set1_ps(other.v.f[4]), v.m[0]),
                _mm_mul_ps(_mm_set1_ps(other.v.f[5]), v.m[1])
            ),
            _mm_add_ps(
                _mm_mul_ps(_mm_set1_ps(other.v.f[6]), v.m[2]),
                _mm_mul_ps(_mm_set1_ps(other.v.f[7]), v.m[3])
            )
        ),
        _mm_add_ps(
            _mm_add_ps(
                _mm_mul_ps(_mm_set1_ps(other.v.f[8]), v.m[0]),
                _mm_mul_ps(_mm_set1_ps(other.v.f[9]), v.m[1])
            ),
            _mm_add_ps(
                _mm_mul_ps(_mm_set1_ps(other.v.f[10]), v.m[2]),
                _mm_mul_ps(_mm_set1_ps(other.v.f[11]), v.m[3])
            )
        ),
        _mm_add_ps(
            _mm_add_ps(
                _mm_mul_ps(_mm_set1_ps(other.v.f[12]), v.m[0]),
                _mm_mul_ps(_mm_set1_ps(other.v.f[13]), v.m[1])
            ),
            _mm_add_ps(
                _mm_mul_ps(_mm_set1_ps(other.v.f[14]), v.m[2]),
                _mm_mul_ps(_mm_set1_ps(other.v.f[15]), v.m[3])
            )
        )
    );
}

LKCOMMON_INLINE Matrix4 Matrix4::operator*(const float value) const
{
    return Matrix4(
        _mm_mul_ps(v.m[0], _mm_set_ps1(value)),
        _mm_mul_ps(v.m[1], _mm_set_ps1(value)),
        _mm_mul_ps(v.m[2], _mm_set_ps1(value)),
        _mm_mul_ps(v.m[3], _mm_set_ps1(value))
    );
}


// Division
LKCOMMON_INLINE Matrix4& Matrix4::operator/=(const float value)
{
    v.m[0] = _mm_div_ps(v.m[0], _mm_set_ps1(value));
    v.m[1] = _mm_div_ps(v.m[1], _mm_set_ps1(value));
    v.m[2] = _mm_div_ps(v.m[2], _mm_set_ps1(value));
    v.m[3] = _mm_div_ps(v.m[3], _mm_set_ps1(value));
    return *this;
}

LKCOMMON_INLINE Matrix4 Matrix4::operator/(const float value) const
{
    return Matrix4(
        _mm_div_ps(v.m[0], _mm_set_ps1(value)),
        _mm_div_ps(v.m[1], _mm_set_ps1(value)),
        _mm_div_ps(v.m[2], _mm_set_ps1(value)),
        _mm_div_ps(v.m[3], _mm_set_ps1(value))
    );
}

// Comparison
LKCOMMON_INLINE bool Matrix4::operator==(const Matrix4& other) const
{
    return (_mm_movemask_ps(_mm_cmpeq_ps(v.m[0], other.v.m[0])) == 0xF &&
            _mm_movemask_ps(_mm_cmpeq_ps(v.m[1], other.v.m[1])) == 0xF &&
            _mm_movemask_ps(_mm_cmpeq_ps(v.m[2], other.v.m[2])) == 0xF &&
            _mm_movemask_ps(_mm_cmpeq_ps(v.m[3], other.v.m[3])) == 0xF);
}

LKCOMMON_INLINE bool Matrix4::operator!=(const Matrix4& other) const
{
    return (_mm_movemask_ps(_mm_cmpneq_ps(v.m[0], other.v.m[0])) != 0 ||
            _mm_movemask_ps(_mm_cmpneq_ps(v.m[1], other.v.m[1])) != 0 ||
            _mm_movemask_ps(_mm_cmpneq_ps(v.m[2], other.v.m[2])) != 0 ||
            _mm_movemask_ps(_mm_cmpneq_ps(v.m[3], other.v.m[3])) != 0);
}

// Operations
LKCOMMON_INLINE Matrix4 Matrix4::Transpose() const
{
    const __m128 t0 = _mm_unpacklo_ps(v.m[0], v.m[1]);
    const __m128 t1 = _mm_unpacklo_ps(v.m[2], v.m[3]);
    const __m128 t2 = _mm_unpackhi_ps(v.m[0], v.m[1]);
    const __m128 t3 = _mm_unpackhi_ps(v.m[2], v.m[3]);

    return Matrix4(
        _mm_movelh_ps(t0, t1),
        _mm_movehl_ps(t1, t0),
        _mm_movelh_ps(t2, t3),
        _mm_movehl_ps(t3, t2)
    );
}

// External operators (cross-type)
LKCOMMON_INLINE Vector4 operator*(const Matrix4& a, const Vector4& b)
{
    return Vector4(_mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(_mm_set1_ps(b[0]), a.v.m[0]),
            _mm_mul_ps(_mm_set1_ps(b[1]), a.v.m[1])
        ),
        _mm_add_ps(
            _mm_mul_ps(_mm_set1_ps(b[2]), a.v.m[2]),
            _mm_mul_ps(_mm_set1_ps(b[3]), a.v.m[3])
        )
    ));
}

} // namespace Math
} // namespace lkCommon
