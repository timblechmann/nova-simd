//  sse vector class
//
//  Copyright (C) 2010 Tim Blechmann
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; see the file COPYING.  If not, write to
//  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
//  Boston, MA 02111-1307, USA.

#ifndef VEC_SSE_HPP
#define VEC_SSE_HPP

#include <xmmintrin.h>

#ifdef __SSE2__
#include <emmintrin.h>
#endif

#if defined(__GNUC__) && defined(NDEBUG)
#define always_inline inline  __attribute__((always_inline))
#else
#define always_inline inline
#endif

namespace nova
{

template <>
struct vec<float>
{
#ifdef __SSE2__
    static inline __m128 gen_sign_mask(void)
    {
        __m128i x = _mm_setzero_si128();
        __m128i ones = _mm_cmpeq_epi32(x, x);
        return (__m128)_mm_slli_epi32 (_mm_srli_epi32(ones, 31), 31);
    }

    static inline __m128 gen_abs_mask(void)
    {
        __m128i x = _mm_setzero_si128();
        __m128i ones = _mm_cmpeq_epi32(x, x);
        return (__m128)_mm_srli_epi32 (_mm_slli_epi32(ones, 1), 1);
    }

    static inline __m128 gen_one(void)
    {
        __m128i x = _mm_setzero_si128();
        __m128i ones = _mm_cmpeq_epi32(x, x);
        return (__m128)_mm_slli_epi32 (_mm_srli_epi32(ones, 25), 23);
    }

    static inline __m128 gen_05(void)
    {
        __m128i x = _mm_setzero_si128();
        __m128i ones = _mm_cmpeq_epi32(x, x);
        return (__m128)_mm_slli_epi32 (_mm_srli_epi32(ones, 26), 24);
    }
#else
    /* SSE fallback */
    static inline __m128 gen_sign_mask(void)
    {
        static const int sign_mask = 0x80000000;
        float * casted = (float*)(&sign_mask);
        return _mm_set_ps1(*casted);
    }

    static inline __m128 gen_abs_mask(void)
    {
        static const int abs_mask = 0x7fffffff;
        float * casted = (float*)(&abs_mask);
        return _mm_set_ps1(*casted);
    }

    static inline __m128 gen_one(void)
    {
        return _mm_set_ps1(1.f);
    }

    static inline __m128 gen_05(void)
    {
        return _mm_set_ps1(0.5f);
    }
#endif

    static inline __m128 gen_zero(void)
    {
        volatile __m128 x;
        return _mm_xor_ps(x, x);
    }

    vec(__m128 const & arg):
        data_(arg)
    {}

public:
    static const int size = 4;
    static const int objects_per_cacheline = 64/sizeof(float);

    /* @{ */
    /** constructors */
    vec(void)
    {}

    explicit vec(float f)
    {
        set_vec(f);
    }

    vec(vec const & rhs)
    {
        data_ = rhs.data_;
    }
    /* @} */

    /* @{ */
    /** io */
    void load(const float * data)
    {
        data_ = _mm_loadu_ps(data);
    }

    void load_aligned(const float * data)
    {
        data_ = _mm_load_ps(data);
    }

    void load_first(const float * data)
    {
        data_ = _mm_load_ss(data);
    }

    void store(float * dest) const
    {
        _mm_storeu_ps(dest, data_);
    }

    void store_aligned(float * dest) const
    {
        _mm_store_ps(dest, data_);
    }

    void store_aligned_stream(float * dest) const
    {
        _mm_stream_ps(dest, data_);
    }

    void clear(void)
    {
        data_ = _mm_setzero_ps();
    }

    /* @} */

    /* @{ */
    /** element access */
    void set (std::size_t index, float value)
    {
        float * data = (float*)&data_;
        data[index] = value;
    }

    void set_vec (float value)
    {
        data_ = _mm_set_ps1(value);
    }

    float set_slope(float start, float slope)
    {
        float v1 = start + slope;
        float v2 = start + slope + slope;
        float v3 = start + slope + slope + slope;
        data_ = _mm_set_ps(v3, v2, v1, start);
        return slope + slope + slope + slope;
    }

    float set_exp(float start, float curve)
    {
        float v1 = start * curve;
        float v2 = v1 * curve;
        float v3 = v2 * curve;
        data_ = _mm_set_ps(v3, v2, v1, start);
        return v3 * curve;
    }

    float get (std::size_t index)
    {
        if (index == 0)
            return _mm_cvtss_f32(data_);

        __m128 ret = _mm_shuffle_ps(data_, data_, _MM_SHUFFLE(index, index, index, index));
        return _mm_cvtss_f32(ret);
    }
    /* @} */

    /* @{ */
    /** arithmetic operators */
#define OPERATOR_ASSIGNMENT(op, opcode) \
    vec & operator op(vec const & rhs) \
    { \
        data_ = opcode(data_, rhs.data_);\
        return *this;\
    }

    OPERATOR_ASSIGNMENT(+=, _mm_add_ps)
    OPERATOR_ASSIGNMENT(-=, _mm_sub_ps)
    OPERATOR_ASSIGNMENT(*=, _mm_mul_ps)
    OPERATOR_ASSIGNMENT(/=, _mm_div_ps)

#define ARITHMETIC_OPERATOR(op, opcode) \
    vec operator op(vec const & rhs) \
    { \
        return opcode(data_, rhs.data_); \
    }

    ARITHMETIC_OPERATOR(+, _mm_add_ps)
    ARITHMETIC_OPERATOR(-, _mm_sub_ps)
    ARITHMETIC_OPERATOR(*, _mm_mul_ps)
    ARITHMETIC_OPERATOR(/, _mm_div_ps)
    /* @} */

    /* @{ */
    /** unary functions */
    friend inline vec abs(vec const & arg)
    {
        return _mm_and_ps(gen_abs_mask(), arg.data_);
    }

    friend always_inline vec sign(vec const & arg)
    {
        const __m128 zero = _mm_setzero_ps();
        const __m128 one = gen_one();
        const __m128 sign_mask = gen_sign_mask();

        const __m128 nonzero = _mm_cmpneq_ps(arg.data_, zero);
        const __m128 sign = _mm_and_ps(arg.data_, sign_mask);

        __m128 abs_ret = _mm_and_ps(nonzero, one);
        return _mm_or_ps(sign, abs_ret);
    }

    friend inline vec square(vec const & arg)
    {
        return _mm_mul_ps(arg.data_, arg.data_);
    }

    friend inline vec cube(vec const & arg)
    {
        return _mm_mul_ps(arg.data_, _mm_mul_ps(arg.data_, arg.data_));
    }
    /* @} */

    /* @{ */
    /** binary functions */
    friend inline vec max(vec const & lhs, vec const & rhs)
    {
        return _mm_max_ps(lhs.data_, rhs.data_);
    }

    friend inline vec min(vec const & lhs, vec const & rhs)
    {
        return _mm_min_ps(lhs.data_, rhs.data_);
    }
    /* @} */

    /* @{ */
    /** horizontal functions */
    inline float horizontal_min(void) const
    {
        __m128 xmm0, xmm1;
        xmm0 = data_;
        xmm1 = _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(2,2,2,2));
        xmm0 = _mm_min_ps(xmm0, xmm1);
        xmm1 = _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(1,1,1,1));
        xmm0 = _mm_min_ss(xmm0, xmm1);
        return _mm_cvtss_f32(xmm0);
    }

    inline float horizontal_max(void) const
    {
        __m128 xmm0, xmm1;
        xmm0 = data_;
        xmm1 = _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(2,2,2,2));
        xmm0 = _mm_max_ps(xmm0, xmm1);
        xmm1 = _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(1,1,1,1));
        xmm0 = _mm_max_ss(xmm0, xmm1);
        return _mm_cvtss_f32(xmm0);
    }
    /* @} */

private:
    typedef union
    {
        float f[4];
        __m128 m;
    } cast_union;

    __m128 data_;
};

} /* namespace nova */

#undef OPERATOR_ASSIGNMENT
#undef ARITHMETIC_OPERATOR
#undef always_inline

#endif /* VEC_SSE_HPP */
