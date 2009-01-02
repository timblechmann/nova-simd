//  binary arithmetic simd functions for sse
//  Copyright (C) 2008, 2009 Tim Blechmann <tim@klingt.org>
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
//
//  implemented as part of nova

#ifndef SIMD_BINARY_ARITHMETIC_SSE_HPP
#define SIMD_BINARY_ARITHMETIC_SSE_HPP

#include <xmmintrin.h>
#include "simd_binary_arithmetic.hpp"

namespace nova {

#if defined(__GNUC__) && defined(NDEBUG)
#define always_inline inline  __attribute__((always_inline))
#else
#define always_inline inline
#endif

#define samples_per_loop 8


namespace detail {

template <int n>
always_inline void plus_vec_simd_mp(float * out, const float * src1, const float * src2)
{
    const __m128 in1 = _mm_load_ps(src1);
    const __m128 in2 = _mm_load_ps(src2);

    const __m128 result = _mm_add_ps(in1, in2);

    _mm_store_ps(out, result);

    plus_vec_simd_mp<n-4>(out+4, src1+4, src2+4);
}

template <>
always_inline void plus_vec_simd_mp<0>(float * out, const float * src1, const float * src2)
{}

template <int n>
always_inline void plus_vec_simd_mp_iteration(float * out, const float * src1, const __m128 & src2)
{
    const __m128 in1 = _mm_load_ps(src1);

    const __m128 result = _mm_add_ps(in1, src2);

    _mm_store_ps(out, result);

    plus_vec_simd_mp_iteration<n-4>(out+4, src1+4, src2);
}

template <>
always_inline void plus_vec_simd_mp_iteration<0>(float * out, const float * src1, const __m128 & src2)
{}

template <int n>
always_inline void plus_vec_simd_mp_iteration(float * out, const __m128 & src1, const float * src2)
{
    const __m128 in2 = _mm_load_ps(src2);

    const __m128 result = _mm_add_ps(src1, in2);

    _mm_store_ps(out, result);

    plus_vec_simd_mp_iteration<n-4>(out+4, src1, src2+4);
}

template <>
always_inline void plus_vec_simd_mp_iteration<0>(float * out, const __m128 & src1, const float * src2)
{}

} /* namespace detail */


/* vector/vector */
using detail::plus_vec_simd_mp;

template <int n>
void plus_vec_simd(float * out, const float * src1, const float * src2)
{
    plus_vec_simd_mp<n>(out, src1, src2);
}

/* vector/scalar */
template <int n>
always_inline void plus_vec_simd_mp(float * out, const float * src1, const float src2)
{
    __m128 in2 = _mm_set_ps1(src2);
    detail::plus_vec_simd_mp_iteration<n>(out, src1, in2);
}

template <int n>
void plus_vec_simd(float * out, const float * src1, const float src2)
{
    plus_vec_simd_mp<n>(out, src1, src2);
}

/* vector/scalar */
template <int n>
always_inline void plus_vec_simd_mp(float * out, const float src1, const float * src2)
{
    __m128 in1 = _mm_set_ps1(src1);
    detail::plus_vec_simd_mp_iteration<n>(out, in1, src2);
}

template <int n>
void plus_vec_simd(float * out, const float src1, const float * src2)
{
    plus_vec_simd<n>(out, src1, src2);
}


template <>
inline void plus_vec_simd(float * out, const float * arg1, const float * arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    do {
        detail::plus_vec_simd_mp<samples_per_loop>(out, arg1, arg2);
        out += samples_per_loop;
        arg1 += samples_per_loop;
        arg2 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void plus_vec_simd(float * out, const float * arg1, const float arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    const __m128 in2 = _mm_set_ps1(arg2);
    do {
        detail::plus_vec_simd_mp_iteration<samples_per_loop>(out, arg1, in2);
        out += samples_per_loop;
        arg1 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void plus_vec_simd(float * out, const float arg1, const float * arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    const __m128 in1 = _mm_set_ps1(arg1);
    do {
        detail::plus_vec_simd_mp_iteration<samples_per_loop>(out, in1, arg2);
        out += samples_per_loop;
        arg2 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void plus_vec_simd(float * out, const float * arg1, float arg2,
                              float arg2_slope, unsigned int n)
{
    unsigned int loops = n / 8;
    __m128 in2   = _mm_set_ps(arg2,
                              arg2 +   arg2_slope,
                              arg2 + 2*arg2_slope,
                              arg2 + 3*arg2_slope);
    __m128 vslope = _mm_set_ps1(4*arg2_slope);

    do {
        __m128 in1 = _mm_load_ps(arg1);
        __m128 result = _mm_add_ps(in1, in2);

        _mm_store_ps(out, result);
        in2 = _mm_add_ps(in2, vslope);

        in1 = _mm_load_ps(arg1+4);
        result = _mm_add_ps(in1, in2);

        _mm_store_ps(out+4, result);
        in2 = _mm_add_ps(in2, vslope);

        out += 8;
        arg1 += 8;
    }
    while (--loops);
}

template <>
inline void plus_vec_simd(float * out, float arg1, const float arg1_slope,
                              const float * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    __m128 in1   = _mm_set_ps(arg1,
                              arg1 +   arg1_slope,
                              arg1 + 2*arg1_slope,
                              arg1 + 3*arg1_slope);
    __m128 vslope = _mm_set_ps1(4*arg1_slope);

    do {
        __m128 in2 = _mm_load_ps(arg2);
        __m128 result = _mm_add_ps(in1, in2);

        _mm_store_ps(out, result);
        in1 = _mm_add_ps(in1, vslope);

        in2 = _mm_load_ps(arg2+4);
        result = _mm_add_ps(in1, in2);

        _mm_store_ps(out+4, result);
        in1 = _mm_add_ps(in1, vslope);

        out += 8;
        arg2 += 8;
    }
    while (--loops);
}

namespace detail {

template <int n>
always_inline void minus_vec_simd_mp(float * out, const float * src1, const float * src2)
{
    const __m128 in1 = _mm_load_ps(src1);
    const __m128 in2 = _mm_load_ps(src2);

    const __m128 result = _mm_sub_ps(in1, in2);

    _mm_store_ps(out, result);

    minus_vec_simd_mp<n-4>(out+4, src1+4, src2+4);
}

template <>
always_inline void minus_vec_simd_mp<0>(float * out, const float * src1, const float * src2)
{}

template <int n>
always_inline void minus_vec_simd_mp_iteration(float * out, const float * src1, const __m128 & src2)
{
    const __m128 in1 = _mm_load_ps(src1);

    const __m128 result = _mm_sub_ps(in1, src2);

    _mm_store_ps(out, result);

    minus_vec_simd_mp_iteration<n-4>(out+4, src1+4, src2);
}

template <>
always_inline void minus_vec_simd_mp_iteration<0>(float * out, const float * src1, const __m128 & src2)
{}

template <int n>
always_inline void minus_vec_simd_mp_iteration(float * out, const __m128 & src1, const float * src2)
{
    const __m128 in2 = _mm_load_ps(src2);

    const __m128 result = _mm_sub_ps(src1, in2);

    _mm_store_ps(out, result);

    minus_vec_simd_mp_iteration<n-4>(out+4, src1, src2+4);
}

template <>
always_inline void minus_vec_simd_mp_iteration<0>(float * out, const __m128 & src1, const float * src2)
{}

} /* namespace detail */


/* vector/vector */
using detail::minus_vec_simd_mp;

template <int n>
void minus_vec_simd(float * out, const float * src1, const float * src2)
{
    minus_vec_simd_mp<n>(out, src1, src2);
}

/* vector/scalar */
template <int n>
always_inline void minus_vec_simd_mp(float * out, const float * src1, const float src2)
{
    __m128 in2 = _mm_set_ps1(src2);
    detail::minus_vec_simd_mp_iteration<n>(out, src1, in2);
}

template <int n>
void minus_vec_simd(float * out, const float * src1, const float src2)
{
    minus_vec_simd_mp<n>(out, src1, src2);
}

/* vector/scalar */
template <int n>
always_inline void minus_vec_simd_mp(float * out, const float src1, const float * src2)
{
    __m128 in1 = _mm_set_ps1(src1);
    detail::minus_vec_simd_mp_iteration<n>(out, in1, src2);
}

template <int n>
void minus_vec_simd(float * out, const float src1, const float * src2)
{
    minus_vec_simd<n>(out, src1, src2);
}


template <>
inline void minus_vec_simd(float * out, const float * arg1, const float * arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    do {
        detail::minus_vec_simd_mp<samples_per_loop>(out, arg1, arg2);
        out += samples_per_loop;
        arg1 += samples_per_loop;
        arg2 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void minus_vec_simd(float * out, const float * arg1, const float arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    const __m128 in2 = _mm_set_ps1(arg2);
    do {
        detail::minus_vec_simd_mp_iteration<samples_per_loop>(out, arg1, in2);
        out += samples_per_loop;
        arg1 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void minus_vec_simd(float * out, const float arg1, const float * arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    const __m128 in1 = _mm_set_ps1(arg1);
    do {
        detail::minus_vec_simd_mp_iteration<samples_per_loop>(out, in1, arg2);
        out += samples_per_loop;
        arg2 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void minus_vec_simd(float * out, const float * arg1, float arg2,
                              float arg2_slope, unsigned int n)
{
    unsigned int loops = n / 8;
    __m128 in2   = _mm_set_ps(arg2,
                              arg2 +   arg2_slope,
                              arg2 + 2*arg2_slope,
                              arg2 + 3*arg2_slope);
    __m128 vslope = _mm_set_ps1(4*arg2_slope);

    do {
        __m128 in1 = _mm_load_ps(arg1);
        __m128 result = _mm_sub_ps(in1, in2);

        _mm_store_ps(out, result);
        in2 = _mm_add_ps(in2, vslope);

        in1 = _mm_load_ps(arg1+4);
        result = _mm_sub_ps(in1, in2);

        _mm_store_ps(out+4, result);
        in2 = _mm_add_ps(in2, vslope);

        out += 8;
        arg1 += 8;
    }
    while (--loops);
}

template <>
inline void minus_vec_simd(float * out, float arg1, const float arg1_slope,
                              const float * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    __m128 in1   = _mm_set_ps(arg1,
                              arg1 +   arg1_slope,
                              arg1 + 2*arg1_slope,
                              arg1 + 3*arg1_slope);
    __m128 vslope = _mm_set_ps1(4*arg1_slope);

    do {
        __m128 in2 = _mm_load_ps(arg2);
        __m128 result = _mm_sub_ps(in1, in2);

        _mm_store_ps(out, result);
        in1 = _mm_add_ps(in1, vslope);

        in2 = _mm_load_ps(arg2+4);
        result = _mm_sub_ps(in1, in2);

        _mm_store_ps(out+4, result);
        in1 = _mm_add_ps(in1, vslope);

        out += 8;
        arg2 += 8;
    }
    while (--loops);
}

namespace detail {

template <int n>
always_inline void times_vec_simd_mp(float * out, const float * src1, const float * src2)
{
    const __m128 in1 = _mm_load_ps(src1);
    const __m128 in2 = _mm_load_ps(src2);

    const __m128 result = _mm_mul_ps(in1, in2);

    _mm_store_ps(out, result);

    times_vec_simd_mp<n-4>(out+4, src1+4, src2+4);
}

template <>
always_inline void times_vec_simd_mp<0>(float * out, const float * src1, const float * src2)
{}

template <int n>
always_inline void times_vec_simd_mp_iteration(float * out, const float * src1, const __m128 & src2)
{
    const __m128 in1 = _mm_load_ps(src1);

    const __m128 result = _mm_mul_ps(in1, src2);

    _mm_store_ps(out, result);

    times_vec_simd_mp_iteration<n-4>(out+4, src1+4, src2);
}

template <>
always_inline void times_vec_simd_mp_iteration<0>(float * out, const float * src1, const __m128 & src2)
{}

template <int n>
always_inline void times_vec_simd_mp_iteration(float * out, const __m128 & src1, const float * src2)
{
    const __m128 in2 = _mm_load_ps(src2);

    const __m128 result = _mm_mul_ps(src1, in2);

    _mm_store_ps(out, result);

    times_vec_simd_mp_iteration<n-4>(out+4, src1, src2+4);
}

template <>
always_inline void times_vec_simd_mp_iteration<0>(float * out, const __m128 & src1, const float * src2)
{}

} /* namespace detail */


/* vector/vector */
using detail::times_vec_simd_mp;

template <int n>
void times_vec_simd(float * out, const float * src1, const float * src2)
{
    times_vec_simd_mp<n>(out, src1, src2);
}

/* vector/scalar */
template <int n>
always_inline void times_vec_simd_mp(float * out, const float * src1, const float src2)
{
    __m128 in2 = _mm_set_ps1(src2);
    detail::times_vec_simd_mp_iteration<n>(out, src1, in2);
}

template <int n>
void times_vec_simd(float * out, const float * src1, const float src2)
{
    times_vec_simd_mp<n>(out, src1, src2);
}

/* vector/scalar */
template <int n>
always_inline void times_vec_simd_mp(float * out, const float src1, const float * src2)
{
    __m128 in1 = _mm_set_ps1(src1);
    detail::times_vec_simd_mp_iteration<n>(out, in1, src2);
}

template <int n>
void times_vec_simd(float * out, const float src1, const float * src2)
{
    times_vec_simd<n>(out, src1, src2);
}


template <>
inline void times_vec_simd(float * out, const float * arg1, const float * arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    do {
        detail::times_vec_simd_mp<samples_per_loop>(out, arg1, arg2);
        out += samples_per_loop;
        arg1 += samples_per_loop;
        arg2 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void times_vec_simd(float * out, const float * arg1, const float arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    const __m128 in2 = _mm_set_ps1(arg2);
    do {
        detail::times_vec_simd_mp_iteration<samples_per_loop>(out, arg1, in2);
        out += samples_per_loop;
        arg1 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void times_vec_simd(float * out, const float arg1, const float * arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    const __m128 in1 = _mm_set_ps1(arg1);
    do {
        detail::times_vec_simd_mp_iteration<samples_per_loop>(out, in1, arg2);
        out += samples_per_loop;
        arg2 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void times_vec_simd(float * out, const float * arg1, float arg2,
                              float arg2_slope, unsigned int n)
{
    unsigned int loops = n / 8;
    __m128 in2   = _mm_set_ps(arg2,
                              arg2 +   arg2_slope,
                              arg2 + 2*arg2_slope,
                              arg2 + 3*arg2_slope);
    __m128 vslope = _mm_set_ps1(4*arg2_slope);

    do {
        __m128 in1 = _mm_load_ps(arg1);
        __m128 result = _mm_mul_ps(in1, in2);

        _mm_store_ps(out, result);
        in2 = _mm_add_ps(in2, vslope);

        in1 = _mm_load_ps(arg1+4);
        result = _mm_mul_ps(in1, in2);

        _mm_store_ps(out+4, result);
        in2 = _mm_add_ps(in2, vslope);

        out += 8;
        arg1 += 8;
    }
    while (--loops);
}

template <>
inline void times_vec_simd(float * out, float arg1, const float arg1_slope,
                              const float * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    __m128 in1   = _mm_set_ps(arg1,
                              arg1 +   arg1_slope,
                              arg1 + 2*arg1_slope,
                              arg1 + 3*arg1_slope);
    __m128 vslope = _mm_set_ps1(4*arg1_slope);

    do {
        __m128 in2 = _mm_load_ps(arg2);
        __m128 result = _mm_mul_ps(in1, in2);

        _mm_store_ps(out, result);
        in1 = _mm_add_ps(in1, vslope);

        in2 = _mm_load_ps(arg2+4);
        result = _mm_mul_ps(in1, in2);

        _mm_store_ps(out+4, result);
        in1 = _mm_add_ps(in1, vslope);

        out += 8;
        arg2 += 8;
    }
    while (--loops);
}

namespace detail {

template <int n>
always_inline void over_vec_simd_mp(float * out, const float * src1, const float * src2)
{
    const __m128 in1 = _mm_load_ps(src1);
    const __m128 in2 = _mm_load_ps(src2);

    const __m128 result = _mm_div_ps(in1, in2);

    _mm_store_ps(out, result);

    over_vec_simd_mp<n-4>(out+4, src1+4, src2+4);
}

template <>
always_inline void over_vec_simd_mp<0>(float * out, const float * src1, const float * src2)
{}

template <int n>
always_inline void over_vec_simd_mp_iteration(float * out, const float * src1, const __m128 & src2)
{
    const __m128 in1 = _mm_load_ps(src1);

    const __m128 result = _mm_div_ps(in1, src2);

    _mm_store_ps(out, result);

    over_vec_simd_mp_iteration<n-4>(out+4, src1+4, src2);
}

template <>
always_inline void over_vec_simd_mp_iteration<0>(float * out, const float * src1, const __m128 & src2)
{}

template <int n>
always_inline void over_vec_simd_mp_iteration(float * out, const __m128 & src1, const float * src2)
{
    const __m128 in2 = _mm_load_ps(src2);

    const __m128 result = _mm_div_ps(src1, in2);

    _mm_store_ps(out, result);

    over_vec_simd_mp_iteration<n-4>(out+4, src1, src2+4);
}

template <>
always_inline void over_vec_simd_mp_iteration<0>(float * out, const __m128 & src1, const float * src2)
{}

} /* namespace detail */


/* vector/vector */
using detail::over_vec_simd_mp;

template <int n>
void over_vec_simd(float * out, const float * src1, const float * src2)
{
    over_vec_simd_mp<n>(out, src1, src2);
}

/* vector/scalar */
template <int n>
always_inline void over_vec_simd_mp(float * out, const float * src1, const float src2)
{
    __m128 in2 = _mm_set_ps1(src2);
    detail::over_vec_simd_mp_iteration<n>(out, src1, in2);
}

template <int n>
void over_vec_simd(float * out, const float * src1, const float src2)
{
    over_vec_simd_mp<n>(out, src1, src2);
}

/* vector/scalar */
template <int n>
always_inline void over_vec_simd_mp(float * out, const float src1, const float * src2)
{
    __m128 in1 = _mm_set_ps1(src1);
    detail::over_vec_simd_mp_iteration<n>(out, in1, src2);
}

template <int n>
void over_vec_simd(float * out, const float src1, const float * src2)
{
    over_vec_simd<n>(out, src1, src2);
}


template <>
inline void over_vec_simd(float * out, const float * arg1, const float * arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    do {
        detail::over_vec_simd_mp<samples_per_loop>(out, arg1, arg2);
        out += samples_per_loop;
        arg1 += samples_per_loop;
        arg2 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void over_vec_simd(float * out, const float * arg1, const float arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    const __m128 in2 = _mm_set_ps1(arg2);
    do {
        detail::over_vec_simd_mp_iteration<samples_per_loop>(out, arg1, in2);
        out += samples_per_loop;
        arg1 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void over_vec_simd(float * out, const float arg1, const float * arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    const __m128 in1 = _mm_set_ps1(arg1);
    do {
        detail::over_vec_simd_mp_iteration<samples_per_loop>(out, in1, arg2);
        out += samples_per_loop;
        arg2 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void over_vec_simd(float * out, const float * arg1, float arg2,
                              float arg2_slope, unsigned int n)
{
    unsigned int loops = n / 8;
    __m128 in2   = _mm_set_ps(arg2,
                              arg2 +   arg2_slope,
                              arg2 + 2*arg2_slope,
                              arg2 + 3*arg2_slope);
    __m128 vslope = _mm_set_ps1(4*arg2_slope);

    do {
        __m128 in1 = _mm_load_ps(arg1);
        __m128 result = _mm_div_ps(in1, in2);

        _mm_store_ps(out, result);
        in2 = _mm_add_ps(in2, vslope);

        in1 = _mm_load_ps(arg1+4);
        result = _mm_div_ps(in1, in2);

        _mm_store_ps(out+4, result);
        in2 = _mm_add_ps(in2, vslope);

        out += 8;
        arg1 += 8;
    }
    while (--loops);
}

template <>
inline void over_vec_simd(float * out, float arg1, const float arg1_slope,
                              const float * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    __m128 in1   = _mm_set_ps(arg1,
                              arg1 +   arg1_slope,
                              arg1 + 2*arg1_slope,
                              arg1 + 3*arg1_slope);
    __m128 vslope = _mm_set_ps1(4*arg1_slope);

    do {
        __m128 in2 = _mm_load_ps(arg2);
        __m128 result = _mm_div_ps(in1, in2);

        _mm_store_ps(out, result);
        in1 = _mm_add_ps(in1, vslope);

        in2 = _mm_load_ps(arg2+4);
        result = _mm_div_ps(in1, in2);

        _mm_store_ps(out+4, result);
        in1 = _mm_add_ps(in1, vslope);

        out += 8;
        arg2 += 8;
    }
    while (--loops);
}

namespace detail {

template <int n>
always_inline void min_vec_simd_mp(float * out, const float * src1, const float * src2)
{
    const __m128 in1 = _mm_load_ps(src1);
    const __m128 in2 = _mm_load_ps(src2);

    const __m128 result = _mm_min_ps(in1, in2);

    _mm_store_ps(out, result);

    min_vec_simd_mp<n-4>(out+4, src1+4, src2+4);
}

template <>
always_inline void min_vec_simd_mp<0>(float * out, const float * src1, const float * src2)
{}

template <int n>
always_inline void min_vec_simd_mp_iteration(float * out, const float * src1, const __m128 & src2)
{
    const __m128 in1 = _mm_load_ps(src1);

    const __m128 result = _mm_min_ps(in1, src2);

    _mm_store_ps(out, result);

    min_vec_simd_mp_iteration<n-4>(out+4, src1+4, src2);
}

template <>
always_inline void min_vec_simd_mp_iteration<0>(float * out, const float * src1, const __m128 & src2)
{}

template <int n>
always_inline void min_vec_simd_mp_iteration(float * out, const __m128 & src1, const float * src2)
{
    const __m128 in2 = _mm_load_ps(src2);

    const __m128 result = _mm_min_ps(src1, in2);

    _mm_store_ps(out, result);

    min_vec_simd_mp_iteration<n-4>(out+4, src1, src2+4);
}

template <>
always_inline void min_vec_simd_mp_iteration<0>(float * out, const __m128 & src1, const float * src2)
{}

} /* namespace detail */


/* vector/vector */
using detail::min_vec_simd_mp;

template <int n>
void min_vec_simd(float * out, const float * src1, const float * src2)
{
    min_vec_simd_mp<n>(out, src1, src2);
}

/* vector/scalar */
template <int n>
always_inline void min_vec_simd_mp(float * out, const float * src1, const float src2)
{
    __m128 in2 = _mm_set_ps1(src2);
    detail::min_vec_simd_mp_iteration<n>(out, src1, in2);
}

template <int n>
void min_vec_simd(float * out, const float * src1, const float src2)
{
    min_vec_simd_mp<n>(out, src1, src2);
}

/* vector/scalar */
template <int n>
always_inline void min_vec_simd_mp(float * out, const float src1, const float * src2)
{
    __m128 in1 = _mm_set_ps1(src1);
    detail::min_vec_simd_mp_iteration<n>(out, in1, src2);
}

template <int n>
void min_vec_simd(float * out, const float src1, const float * src2)
{
    min_vec_simd<n>(out, src1, src2);
}


template <>
inline void min_vec_simd(float * out, const float * arg1, const float * arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    do {
        detail::min_vec_simd_mp<samples_per_loop>(out, arg1, arg2);
        out += samples_per_loop;
        arg1 += samples_per_loop;
        arg2 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void min_vec_simd(float * out, const float * arg1, const float arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    const __m128 in2 = _mm_set_ps1(arg2);
    do {
        detail::min_vec_simd_mp_iteration<samples_per_loop>(out, arg1, in2);
        out += samples_per_loop;
        arg1 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void min_vec_simd(float * out, const float arg1, const float * arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    const __m128 in1 = _mm_set_ps1(arg1);
    do {
        detail::min_vec_simd_mp_iteration<samples_per_loop>(out, in1, arg2);
        out += samples_per_loop;
        arg2 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void min_vec_simd(float * out, const float * arg1, float arg2,
                              float arg2_slope, unsigned int n)
{
    unsigned int loops = n / 8;
    __m128 in2   = _mm_set_ps(arg2,
                              arg2 +   arg2_slope,
                              arg2 + 2*arg2_slope,
                              arg2 + 3*arg2_slope);
    __m128 vslope = _mm_set_ps1(4*arg2_slope);

    do {
        __m128 in1 = _mm_load_ps(arg1);
        __m128 result = _mm_min_ps(in1, in2);

        _mm_store_ps(out, result);
        in2 = _mm_add_ps(in2, vslope);

        in1 = _mm_load_ps(arg1+4);
        result = _mm_min_ps(in1, in2);

        _mm_store_ps(out+4, result);
        in2 = _mm_add_ps(in2, vslope);

        out += 8;
        arg1 += 8;
    }
    while (--loops);
}

template <>
inline void min_vec_simd(float * out, float arg1, const float arg1_slope,
                              const float * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    __m128 in1   = _mm_set_ps(arg1,
                              arg1 +   arg1_slope,
                              arg1 + 2*arg1_slope,
                              arg1 + 3*arg1_slope);
    __m128 vslope = _mm_set_ps1(4*arg1_slope);

    do {
        __m128 in2 = _mm_load_ps(arg2);
        __m128 result = _mm_min_ps(in1, in2);

        _mm_store_ps(out, result);
        in1 = _mm_add_ps(in1, vslope);

        in2 = _mm_load_ps(arg2+4);
        result = _mm_min_ps(in1, in2);

        _mm_store_ps(out+4, result);
        in1 = _mm_add_ps(in1, vslope);

        out += 8;
        arg2 += 8;
    }
    while (--loops);
}

namespace detail {

template <int n>
always_inline void max_vec_simd_mp(float * out, const float * src1, const float * src2)
{
    const __m128 in1 = _mm_load_ps(src1);
    const __m128 in2 = _mm_load_ps(src2);

    const __m128 result = _mm_max_ps(in1, in2);

    _mm_store_ps(out, result);

    max_vec_simd_mp<n-4>(out+4, src1+4, src2+4);
}

template <>
always_inline void max_vec_simd_mp<0>(float * out, const float * src1, const float * src2)
{}

template <int n>
always_inline void max_vec_simd_mp_iteration(float * out, const float * src1, const __m128 & src2)
{
    const __m128 in1 = _mm_load_ps(src1);

    const __m128 result = _mm_max_ps(in1, src2);

    _mm_store_ps(out, result);

    max_vec_simd_mp_iteration<n-4>(out+4, src1+4, src2);
}

template <>
always_inline void max_vec_simd_mp_iteration<0>(float * out, const float * src1, const __m128 & src2)
{}

template <int n>
always_inline void max_vec_simd_mp_iteration(float * out, const __m128 & src1, const float * src2)
{
    const __m128 in2 = _mm_load_ps(src2);

    const __m128 result = _mm_max_ps(src1, in2);

    _mm_store_ps(out, result);

    max_vec_simd_mp_iteration<n-4>(out+4, src1, src2+4);
}

template <>
always_inline void max_vec_simd_mp_iteration<0>(float * out, const __m128 & src1, const float * src2)
{}

} /* namespace detail */


/* vector/vector */
using detail::max_vec_simd_mp;

template <int n>
void max_vec_simd(float * out, const float * src1, const float * src2)
{
    max_vec_simd_mp<n>(out, src1, src2);
}

/* vector/scalar */
template <int n>
always_inline void max_vec_simd_mp(float * out, const float * src1, const float src2)
{
    __m128 in2 = _mm_set_ps1(src2);
    detail::max_vec_simd_mp_iteration<n>(out, src1, in2);
}

template <int n>
void max_vec_simd(float * out, const float * src1, const float src2)
{
    max_vec_simd_mp<n>(out, src1, src2);
}

/* vector/scalar */
template <int n>
always_inline void max_vec_simd_mp(float * out, const float src1, const float * src2)
{
    __m128 in1 = _mm_set_ps1(src1);
    detail::max_vec_simd_mp_iteration<n>(out, in1, src2);
}

template <int n>
void max_vec_simd(float * out, const float src1, const float * src2)
{
    max_vec_simd<n>(out, src1, src2);
}


template <>
inline void max_vec_simd(float * out, const float * arg1, const float * arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    do {
        detail::max_vec_simd_mp<samples_per_loop>(out, arg1, arg2);
        out += samples_per_loop;
        arg1 += samples_per_loop;
        arg2 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void max_vec_simd(float * out, const float * arg1, const float arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    const __m128 in2 = _mm_set_ps1(arg2);
    do {
        detail::max_vec_simd_mp_iteration<samples_per_loop>(out, arg1, in2);
        out += samples_per_loop;
        arg1 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void max_vec_simd(float * out, const float arg1, const float * arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    const __m128 in1 = _mm_set_ps1(arg1);
    do {
        detail::max_vec_simd_mp_iteration<samples_per_loop>(out, in1, arg2);
        out += samples_per_loop;
        arg2 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void max_vec_simd(float * out, const float * arg1, float arg2,
                              float arg2_slope, unsigned int n)
{
    unsigned int loops = n / 8;
    __m128 in2   = _mm_set_ps(arg2,
                              arg2 +   arg2_slope,
                              arg2 + 2*arg2_slope,
                              arg2 + 3*arg2_slope);
    __m128 vslope = _mm_set_ps1(4*arg2_slope);

    do {
        __m128 in1 = _mm_load_ps(arg1);
        __m128 result = _mm_max_ps(in1, in2);

        _mm_store_ps(out, result);
        in2 = _mm_add_ps(in2, vslope);

        in1 = _mm_load_ps(arg1+4);
        result = _mm_max_ps(in1, in2);

        _mm_store_ps(out+4, result);
        in2 = _mm_add_ps(in2, vslope);

        out += 8;
        arg1 += 8;
    }
    while (--loops);
}

template <>
inline void max_vec_simd(float * out, float arg1, const float arg1_slope,
                              const float * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    __m128 in1   = _mm_set_ps(arg1,
                              arg1 +   arg1_slope,
                              arg1 + 2*arg1_slope,
                              arg1 + 3*arg1_slope);
    __m128 vslope = _mm_set_ps1(4*arg1_slope);

    do {
        __m128 in2 = _mm_load_ps(arg2);
        __m128 result = _mm_max_ps(in1, in2);

        _mm_store_ps(out, result);
        in1 = _mm_add_ps(in1, vslope);

        in2 = _mm_load_ps(arg2+4);
        result = _mm_max_ps(in1, in2);

        _mm_store_ps(out+4, result);
        in1 = _mm_add_ps(in1, vslope);

        out += 8;
        arg2 += 8;
    }
    while (--loops);
}

namespace detail {

template <int n>
always_inline void less_vec_simd_mp(float * out, const float * src1, const float * src2)
{
    const __m128 in1 = _mm_load_ps(src1);
    const __m128 in2 = _mm_load_ps(src2);

    const __m128 result = _mm_cmplt_ps(in1, in2);

    _mm_store_ps(out, result);

    less_vec_simd_mp<n-4>(out+4, src1+4, src2+4);
}

template <>
always_inline void less_vec_simd_mp<0>(float * out, const float * src1, const float * src2)
{}

template <int n>
always_inline void less_vec_simd_mp_iteration(float * out, const float * src1, const __m128 & src2)
{
    const __m128 in1 = _mm_load_ps(src1);

    const __m128 result = _mm_cmplt_ps(in1, src2);

    _mm_store_ps(out, result);

    less_vec_simd_mp_iteration<n-4>(out+4, src1+4, src2);
}

template <>
always_inline void less_vec_simd_mp_iteration<0>(float * out, const float * src1, const __m128 & src2)
{}

template <int n>
always_inline void less_vec_simd_mp_iteration(float * out, const __m128 & src1, const float * src2)
{
    const __m128 in2 = _mm_load_ps(src2);

    const __m128 result = _mm_cmplt_ps(src1, in2);

    _mm_store_ps(out, result);

    less_vec_simd_mp_iteration<n-4>(out+4, src1, src2+4);
}

template <>
always_inline void less_vec_simd_mp_iteration<0>(float * out, const __m128 & src1, const float * src2)
{}

} /* namespace detail */


/* vector/vector */
using detail::less_vec_simd_mp;

template <int n>
void less_vec_simd(float * out, const float * src1, const float * src2)
{
    less_vec_simd_mp<n>(out, src1, src2);
}

/* vector/scalar */
template <int n>
always_inline void less_vec_simd_mp(float * out, const float * src1, const float src2)
{
    __m128 in2 = _mm_set_ps1(src2);
    detail::less_vec_simd_mp_iteration<n>(out, src1, in2);
}

template <int n>
void less_vec_simd(float * out, const float * src1, const float src2)
{
    less_vec_simd_mp<n>(out, src1, src2);
}

/* vector/scalar */
template <int n>
always_inline void less_vec_simd_mp(float * out, const float src1, const float * src2)
{
    __m128 in1 = _mm_set_ps1(src1);
    detail::less_vec_simd_mp_iteration<n>(out, in1, src2);
}

template <int n>
void less_vec_simd(float * out, const float src1, const float * src2)
{
    less_vec_simd<n>(out, src1, src2);
}


template <>
inline void less_vec_simd(float * out, const float * arg1, const float * arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    do {
        detail::less_vec_simd_mp<samples_per_loop>(out, arg1, arg2);
        out += samples_per_loop;
        arg1 += samples_per_loop;
        arg2 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void less_vec_simd(float * out, const float * arg1, const float arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    const __m128 in2 = _mm_set_ps1(arg2);
    do {
        detail::less_vec_simd_mp_iteration<samples_per_loop>(out, arg1, in2);
        out += samples_per_loop;
        arg1 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void less_vec_simd(float * out, const float arg1, const float * arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    const __m128 in1 = _mm_set_ps1(arg1);
    do {
        detail::less_vec_simd_mp_iteration<samples_per_loop>(out, in1, arg2);
        out += samples_per_loop;
        arg2 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void less_vec_simd(float * out, const float * arg1, float arg2,
                              float arg2_slope, unsigned int n)
{
    unsigned int loops = n / 8;
    __m128 in2   = _mm_set_ps(arg2,
                              arg2 +   arg2_slope,
                              arg2 + 2*arg2_slope,
                              arg2 + 3*arg2_slope);
    __m128 vslope = _mm_set_ps1(4*arg2_slope);

    do {
        __m128 in1 = _mm_load_ps(arg1);
        __m128 result = _mm_cmplt_ps(in1, in2);

        _mm_store_ps(out, result);
        in2 = _mm_add_ps(in2, vslope);

        in1 = _mm_load_ps(arg1+4);
        result = _mm_cmplt_ps(in1, in2);

        _mm_store_ps(out+4, result);
        in2 = _mm_add_ps(in2, vslope);

        out += 8;
        arg1 += 8;
    }
    while (--loops);
}

template <>
inline void less_vec_simd(float * out, float arg1, const float arg1_slope,
                              const float * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    __m128 in1   = _mm_set_ps(arg1,
                              arg1 +   arg1_slope,
                              arg1 + 2*arg1_slope,
                              arg1 + 3*arg1_slope);
    __m128 vslope = _mm_set_ps1(4*arg1_slope);

    do {
        __m128 in2 = _mm_load_ps(arg2);
        __m128 result = _mm_cmplt_ps(in1, in2);

        _mm_store_ps(out, result);
        in1 = _mm_add_ps(in1, vslope);

        in2 = _mm_load_ps(arg2+4);
        result = _mm_cmplt_ps(in1, in2);

        _mm_store_ps(out+4, result);
        in1 = _mm_add_ps(in1, vslope);

        out += 8;
        arg2 += 8;
    }
    while (--loops);
}

namespace detail {

template <int n>
always_inline void less_equal_vec_simd_mp(float * out, const float * src1, const float * src2)
{
    const __m128 in1 = _mm_load_ps(src1);
    const __m128 in2 = _mm_load_ps(src2);

    const __m128 result = _mm_cmple_ps(in1, in2);

    _mm_store_ps(out, result);

    less_equal_vec_simd_mp<n-4>(out+4, src1+4, src2+4);
}

template <>
always_inline void less_equal_vec_simd_mp<0>(float * out, const float * src1, const float * src2)
{}

template <int n>
always_inline void less_equal_vec_simd_mp_iteration(float * out, const float * src1, const __m128 & src2)
{
    const __m128 in1 = _mm_load_ps(src1);

    const __m128 result = _mm_cmple_ps(in1, src2);

    _mm_store_ps(out, result);

    less_equal_vec_simd_mp_iteration<n-4>(out+4, src1+4, src2);
}

template <>
always_inline void less_equal_vec_simd_mp_iteration<0>(float * out, const float * src1, const __m128 & src2)
{}

template <int n>
always_inline void less_equal_vec_simd_mp_iteration(float * out, const __m128 & src1, const float * src2)
{
    const __m128 in2 = _mm_load_ps(src2);

    const __m128 result = _mm_cmple_ps(src1, in2);

    _mm_store_ps(out, result);

    less_equal_vec_simd_mp_iteration<n-4>(out+4, src1, src2+4);
}

template <>
always_inline void less_equal_vec_simd_mp_iteration<0>(float * out, const __m128 & src1, const float * src2)
{}

} /* namespace detail */


/* vector/vector */
using detail::less_equal_vec_simd_mp;

template <int n>
void less_equal_vec_simd(float * out, const float * src1, const float * src2)
{
    less_equal_vec_simd_mp<n>(out, src1, src2);
}

/* vector/scalar */
template <int n>
always_inline void less_equal_vec_simd_mp(float * out, const float * src1, const float src2)
{
    __m128 in2 = _mm_set_ps1(src2);
    detail::less_equal_vec_simd_mp_iteration<n>(out, src1, in2);
}

template <int n>
void less_equal_vec_simd(float * out, const float * src1, const float src2)
{
    less_equal_vec_simd_mp<n>(out, src1, src2);
}

/* vector/scalar */
template <int n>
always_inline void less_equal_vec_simd_mp(float * out, const float src1, const float * src2)
{
    __m128 in1 = _mm_set_ps1(src1);
    detail::less_equal_vec_simd_mp_iteration<n>(out, in1, src2);
}

template <int n>
void less_equal_vec_simd(float * out, const float src1, const float * src2)
{
    less_equal_vec_simd<n>(out, src1, src2);
}


template <>
inline void less_equal_vec_simd(float * out, const float * arg1, const float * arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    do {
        detail::less_equal_vec_simd_mp<samples_per_loop>(out, arg1, arg2);
        out += samples_per_loop;
        arg1 += samples_per_loop;
        arg2 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void less_equal_vec_simd(float * out, const float * arg1, const float arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    const __m128 in2 = _mm_set_ps1(arg2);
    do {
        detail::less_equal_vec_simd_mp_iteration<samples_per_loop>(out, arg1, in2);
        out += samples_per_loop;
        arg1 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void less_equal_vec_simd(float * out, const float arg1, const float * arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    const __m128 in1 = _mm_set_ps1(arg1);
    do {
        detail::less_equal_vec_simd_mp_iteration<samples_per_loop>(out, in1, arg2);
        out += samples_per_loop;
        arg2 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void less_equal_vec_simd(float * out, const float * arg1, float arg2,
                              float arg2_slope, unsigned int n)
{
    unsigned int loops = n / 8;
    __m128 in2   = _mm_set_ps(arg2,
                              arg2 +   arg2_slope,
                              arg2 + 2*arg2_slope,
                              arg2 + 3*arg2_slope);
    __m128 vslope = _mm_set_ps1(4*arg2_slope);

    do {
        __m128 in1 = _mm_load_ps(arg1);
        __m128 result = _mm_cmple_ps(in1, in2);

        _mm_store_ps(out, result);
        in2 = _mm_add_ps(in2, vslope);

        in1 = _mm_load_ps(arg1+4);
        result = _mm_cmple_ps(in1, in2);

        _mm_store_ps(out+4, result);
        in2 = _mm_add_ps(in2, vslope);

        out += 8;
        arg1 += 8;
    }
    while (--loops);
}

template <>
inline void less_equal_vec_simd(float * out, float arg1, const float arg1_slope,
                              const float * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    __m128 in1   = _mm_set_ps(arg1,
                              arg1 +   arg1_slope,
                              arg1 + 2*arg1_slope,
                              arg1 + 3*arg1_slope);
    __m128 vslope = _mm_set_ps1(4*arg1_slope);

    do {
        __m128 in2 = _mm_load_ps(arg2);
        __m128 result = _mm_cmple_ps(in1, in2);

        _mm_store_ps(out, result);
        in1 = _mm_add_ps(in1, vslope);

        in2 = _mm_load_ps(arg2+4);
        result = _mm_cmple_ps(in1, in2);

        _mm_store_ps(out+4, result);
        in1 = _mm_add_ps(in1, vslope);

        out += 8;
        arg2 += 8;
    }
    while (--loops);
}

namespace detail {

template <int n>
always_inline void greater_vec_simd_mp(float * out, const float * src1, const float * src2)
{
    const __m128 in1 = _mm_load_ps(src1);
    const __m128 in2 = _mm_load_ps(src2);

    const __m128 result = _mm_cmpgt_ps(in1, in2);

    _mm_store_ps(out, result);

    greater_vec_simd_mp<n-4>(out+4, src1+4, src2+4);
}

template <>
always_inline void greater_vec_simd_mp<0>(float * out, const float * src1, const float * src2)
{}

template <int n>
always_inline void greater_vec_simd_mp_iteration(float * out, const float * src1, const __m128 & src2)
{
    const __m128 in1 = _mm_load_ps(src1);

    const __m128 result = _mm_cmpgt_ps(in1, src2);

    _mm_store_ps(out, result);

    greater_vec_simd_mp_iteration<n-4>(out+4, src1+4, src2);
}

template <>
always_inline void greater_vec_simd_mp_iteration<0>(float * out, const float * src1, const __m128 & src2)
{}

template <int n>
always_inline void greater_vec_simd_mp_iteration(float * out, const __m128 & src1, const float * src2)
{
    const __m128 in2 = _mm_load_ps(src2);

    const __m128 result = _mm_cmpgt_ps(src1, in2);

    _mm_store_ps(out, result);

    greater_vec_simd_mp_iteration<n-4>(out+4, src1, src2+4);
}

template <>
always_inline void greater_vec_simd_mp_iteration<0>(float * out, const __m128 & src1, const float * src2)
{}

} /* namespace detail */


/* vector/vector */
using detail::greater_vec_simd_mp;

template <int n>
void greater_vec_simd(float * out, const float * src1, const float * src2)
{
    greater_vec_simd_mp<n>(out, src1, src2);
}

/* vector/scalar */
template <int n>
always_inline void greater_vec_simd_mp(float * out, const float * src1, const float src2)
{
    __m128 in2 = _mm_set_ps1(src2);
    detail::greater_vec_simd_mp_iteration<n>(out, src1, in2);
}

template <int n>
void greater_vec_simd(float * out, const float * src1, const float src2)
{
    greater_vec_simd_mp<n>(out, src1, src2);
}

/* vector/scalar */
template <int n>
always_inline void greater_vec_simd_mp(float * out, const float src1, const float * src2)
{
    __m128 in1 = _mm_set_ps1(src1);
    detail::greater_vec_simd_mp_iteration<n>(out, in1, src2);
}

template <int n>
void greater_vec_simd(float * out, const float src1, const float * src2)
{
    greater_vec_simd<n>(out, src1, src2);
}


template <>
inline void greater_vec_simd(float * out, const float * arg1, const float * arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    do {
        detail::greater_vec_simd_mp<samples_per_loop>(out, arg1, arg2);
        out += samples_per_loop;
        arg1 += samples_per_loop;
        arg2 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void greater_vec_simd(float * out, const float * arg1, const float arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    const __m128 in2 = _mm_set_ps1(arg2);
    do {
        detail::greater_vec_simd_mp_iteration<samples_per_loop>(out, arg1, in2);
        out += samples_per_loop;
        arg1 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void greater_vec_simd(float * out, const float arg1, const float * arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    const __m128 in1 = _mm_set_ps1(arg1);
    do {
        detail::greater_vec_simd_mp_iteration<samples_per_loop>(out, in1, arg2);
        out += samples_per_loop;
        arg2 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void greater_vec_simd(float * out, const float * arg1, float arg2,
                              float arg2_slope, unsigned int n)
{
    unsigned int loops = n / 8;
    __m128 in2   = _mm_set_ps(arg2,
                              arg2 +   arg2_slope,
                              arg2 + 2*arg2_slope,
                              arg2 + 3*arg2_slope);
    __m128 vslope = _mm_set_ps1(4*arg2_slope);

    do {
        __m128 in1 = _mm_load_ps(arg1);
        __m128 result = _mm_cmpgt_ps(in1, in2);

        _mm_store_ps(out, result);
        in2 = _mm_add_ps(in2, vslope);

        in1 = _mm_load_ps(arg1+4);
        result = _mm_cmpgt_ps(in1, in2);

        _mm_store_ps(out+4, result);
        in2 = _mm_add_ps(in2, vslope);

        out += 8;
        arg1 += 8;
    }
    while (--loops);
}

template <>
inline void greater_vec_simd(float * out, float arg1, const float arg1_slope,
                              const float * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    __m128 in1   = _mm_set_ps(arg1,
                              arg1 +   arg1_slope,
                              arg1 + 2*arg1_slope,
                              arg1 + 3*arg1_slope);
    __m128 vslope = _mm_set_ps1(4*arg1_slope);

    do {
        __m128 in2 = _mm_load_ps(arg2);
        __m128 result = _mm_cmpgt_ps(in1, in2);

        _mm_store_ps(out, result);
        in1 = _mm_add_ps(in1, vslope);

        in2 = _mm_load_ps(arg2+4);
        result = _mm_cmpgt_ps(in1, in2);

        _mm_store_ps(out+4, result);
        in1 = _mm_add_ps(in1, vslope);

        out += 8;
        arg2 += 8;
    }
    while (--loops);
}

namespace detail {

template <int n>
always_inline void greater_equal_vec_simd_mp(float * out, const float * src1, const float * src2)
{
    const __m128 in1 = _mm_load_ps(src1);
    const __m128 in2 = _mm_load_ps(src2);

    const __m128 result = _mm_cmpge_ps(in1, in2);

    _mm_store_ps(out, result);

    greater_equal_vec_simd_mp<n-4>(out+4, src1+4, src2+4);
}

template <>
always_inline void greater_equal_vec_simd_mp<0>(float * out, const float * src1, const float * src2)
{}

template <int n>
always_inline void greater_equal_vec_simd_mp_iteration(float * out, const float * src1, const __m128 & src2)
{
    const __m128 in1 = _mm_load_ps(src1);

    const __m128 result = _mm_cmpge_ps(in1, src2);

    _mm_store_ps(out, result);

    greater_equal_vec_simd_mp_iteration<n-4>(out+4, src1+4, src2);
}

template <>
always_inline void greater_equal_vec_simd_mp_iteration<0>(float * out, const float * src1, const __m128 & src2)
{}

template <int n>
always_inline void greater_equal_vec_simd_mp_iteration(float * out, const __m128 & src1, const float * src2)
{
    const __m128 in2 = _mm_load_ps(src2);

    const __m128 result = _mm_cmpge_ps(src1, in2);

    _mm_store_ps(out, result);

    greater_equal_vec_simd_mp_iteration<n-4>(out+4, src1, src2+4);
}

template <>
always_inline void greater_equal_vec_simd_mp_iteration<0>(float * out, const __m128 & src1, const float * src2)
{}

} /* namespace detail */


/* vector/vector */
using detail::greater_equal_vec_simd_mp;

template <int n>
void greater_equal_vec_simd(float * out, const float * src1, const float * src2)
{
    greater_equal_vec_simd_mp<n>(out, src1, src2);
}

/* vector/scalar */
template <int n>
always_inline void greater_equal_vec_simd_mp(float * out, const float * src1, const float src2)
{
    __m128 in2 = _mm_set_ps1(src2);
    detail::greater_equal_vec_simd_mp_iteration<n>(out, src1, in2);
}

template <int n>
void greater_equal_vec_simd(float * out, const float * src1, const float src2)
{
    greater_equal_vec_simd_mp<n>(out, src1, src2);
}

/* vector/scalar */
template <int n>
always_inline void greater_equal_vec_simd_mp(float * out, const float src1, const float * src2)
{
    __m128 in1 = _mm_set_ps1(src1);
    detail::greater_equal_vec_simd_mp_iteration<n>(out, in1, src2);
}

template <int n>
void greater_equal_vec_simd(float * out, const float src1, const float * src2)
{
    greater_equal_vec_simd<n>(out, src1, src2);
}


template <>
inline void greater_equal_vec_simd(float * out, const float * arg1, const float * arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    do {
        detail::greater_equal_vec_simd_mp<samples_per_loop>(out, arg1, arg2);
        out += samples_per_loop;
        arg1 += samples_per_loop;
        arg2 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void greater_equal_vec_simd(float * out, const float * arg1, const float arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    const __m128 in2 = _mm_set_ps1(arg2);
    do {
        detail::greater_equal_vec_simd_mp_iteration<samples_per_loop>(out, arg1, in2);
        out += samples_per_loop;
        arg1 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void greater_equal_vec_simd(float * out, const float arg1, const float * arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    const __m128 in1 = _mm_set_ps1(arg1);
    do {
        detail::greater_equal_vec_simd_mp_iteration<samples_per_loop>(out, in1, arg2);
        out += samples_per_loop;
        arg2 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void greater_equal_vec_simd(float * out, const float * arg1, float arg2,
                              float arg2_slope, unsigned int n)
{
    unsigned int loops = n / 8;
    __m128 in2   = _mm_set_ps(arg2,
                              arg2 +   arg2_slope,
                              arg2 + 2*arg2_slope,
                              arg2 + 3*arg2_slope);
    __m128 vslope = _mm_set_ps1(4*arg2_slope);

    do {
        __m128 in1 = _mm_load_ps(arg1);
        __m128 result = _mm_cmpge_ps(in1, in2);

        _mm_store_ps(out, result);
        in2 = _mm_add_ps(in2, vslope);

        in1 = _mm_load_ps(arg1+4);
        result = _mm_cmpge_ps(in1, in2);

        _mm_store_ps(out+4, result);
        in2 = _mm_add_ps(in2, vslope);

        out += 8;
        arg1 += 8;
    }
    while (--loops);
}

template <>
inline void greater_equal_vec_simd(float * out, float arg1, const float arg1_slope,
                              const float * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    __m128 in1   = _mm_set_ps(arg1,
                              arg1 +   arg1_slope,
                              arg1 + 2*arg1_slope,
                              arg1 + 3*arg1_slope);
    __m128 vslope = _mm_set_ps1(4*arg1_slope);

    do {
        __m128 in2 = _mm_load_ps(arg2);
        __m128 result = _mm_cmpge_ps(in1, in2);

        _mm_store_ps(out, result);
        in1 = _mm_add_ps(in1, vslope);

        in2 = _mm_load_ps(arg2+4);
        result = _mm_cmpge_ps(in1, in2);

        _mm_store_ps(out+4, result);
        in1 = _mm_add_ps(in1, vslope);

        out += 8;
        arg2 += 8;
    }
    while (--loops);
}

namespace detail {

template <int n>
always_inline void equal_vec_simd_mp(float * out, const float * src1, const float * src2)
{
    const __m128 in1 = _mm_load_ps(src1);
    const __m128 in2 = _mm_load_ps(src2);

    const __m128 result = _mm_cmpeq_ps(in1, in2);

    _mm_store_ps(out, result);

    equal_vec_simd_mp<n-4>(out+4, src1+4, src2+4);
}

template <>
always_inline void equal_vec_simd_mp<0>(float * out, const float * src1, const float * src2)
{}

template <int n>
always_inline void equal_vec_simd_mp_iteration(float * out, const float * src1, const __m128 & src2)
{
    const __m128 in1 = _mm_load_ps(src1);

    const __m128 result = _mm_cmpeq_ps(in1, src2);

    _mm_store_ps(out, result);

    equal_vec_simd_mp_iteration<n-4>(out+4, src1+4, src2);
}

template <>
always_inline void equal_vec_simd_mp_iteration<0>(float * out, const float * src1, const __m128 & src2)
{}

template <int n>
always_inline void equal_vec_simd_mp_iteration(float * out, const __m128 & src1, const float * src2)
{
    const __m128 in2 = _mm_load_ps(src2);

    const __m128 result = _mm_cmpeq_ps(src1, in2);

    _mm_store_ps(out, result);

    equal_vec_simd_mp_iteration<n-4>(out+4, src1, src2+4);
}

template <>
always_inline void equal_vec_simd_mp_iteration<0>(float * out, const __m128 & src1, const float * src2)
{}

} /* namespace detail */


/* vector/vector */
using detail::equal_vec_simd_mp;

template <int n>
void equal_vec_simd(float * out, const float * src1, const float * src2)
{
    equal_vec_simd_mp<n>(out, src1, src2);
}

/* vector/scalar */
template <int n>
always_inline void equal_vec_simd_mp(float * out, const float * src1, const float src2)
{
    __m128 in2 = _mm_set_ps1(src2);
    detail::equal_vec_simd_mp_iteration<n>(out, src1, in2);
}

template <int n>
void equal_vec_simd(float * out, const float * src1, const float src2)
{
    equal_vec_simd_mp<n>(out, src1, src2);
}

/* vector/scalar */
template <int n>
always_inline void equal_vec_simd_mp(float * out, const float src1, const float * src2)
{
    __m128 in1 = _mm_set_ps1(src1);
    detail::equal_vec_simd_mp_iteration<n>(out, in1, src2);
}

template <int n>
void equal_vec_simd(float * out, const float src1, const float * src2)
{
    equal_vec_simd<n>(out, src1, src2);
}


template <>
inline void equal_vec_simd(float * out, const float * arg1, const float * arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    do {
        detail::equal_vec_simd_mp<samples_per_loop>(out, arg1, arg2);
        out += samples_per_loop;
        arg1 += samples_per_loop;
        arg2 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void equal_vec_simd(float * out, const float * arg1, const float arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    const __m128 in2 = _mm_set_ps1(arg2);
    do {
        detail::equal_vec_simd_mp_iteration<samples_per_loop>(out, arg1, in2);
        out += samples_per_loop;
        arg1 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void equal_vec_simd(float * out, const float arg1, const float * arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    const __m128 in1 = _mm_set_ps1(arg1);
    do {
        detail::equal_vec_simd_mp_iteration<samples_per_loop>(out, in1, arg2);
        out += samples_per_loop;
        arg2 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void equal_vec_simd(float * out, const float * arg1, float arg2,
                              float arg2_slope, unsigned int n)
{
    unsigned int loops = n / 8;
    __m128 in2   = _mm_set_ps(arg2,
                              arg2 +   arg2_slope,
                              arg2 + 2*arg2_slope,
                              arg2 + 3*arg2_slope);
    __m128 vslope = _mm_set_ps1(4*arg2_slope);

    do {
        __m128 in1 = _mm_load_ps(arg1);
        __m128 result = _mm_cmpeq_ps(in1, in2);

        _mm_store_ps(out, result);
        in2 = _mm_add_ps(in2, vslope);

        in1 = _mm_load_ps(arg1+4);
        result = _mm_cmpeq_ps(in1, in2);

        _mm_store_ps(out+4, result);
        in2 = _mm_add_ps(in2, vslope);

        out += 8;
        arg1 += 8;
    }
    while (--loops);
}

template <>
inline void equal_vec_simd(float * out, float arg1, const float arg1_slope,
                              const float * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    __m128 in1   = _mm_set_ps(arg1,
                              arg1 +   arg1_slope,
                              arg1 + 2*arg1_slope,
                              arg1 + 3*arg1_slope);
    __m128 vslope = _mm_set_ps1(4*arg1_slope);

    do {
        __m128 in2 = _mm_load_ps(arg2);
        __m128 result = _mm_cmpeq_ps(in1, in2);

        _mm_store_ps(out, result);
        in1 = _mm_add_ps(in1, vslope);

        in2 = _mm_load_ps(arg2+4);
        result = _mm_cmpeq_ps(in1, in2);

        _mm_store_ps(out+4, result);
        in1 = _mm_add_ps(in1, vslope);

        out += 8;
        arg2 += 8;
    }
    while (--loops);
}

namespace detail {

template <int n>
always_inline void notequal_vec_simd_mp(float * out, const float * src1, const float * src2)
{
    const __m128 in1 = _mm_load_ps(src1);
    const __m128 in2 = _mm_load_ps(src2);

    const __m128 result = _mm_cmpneq_ps(in1, in2);

    _mm_store_ps(out, result);

    notequal_vec_simd_mp<n-4>(out+4, src1+4, src2+4);
}

template <>
always_inline void notequal_vec_simd_mp<0>(float * out, const float * src1, const float * src2)
{}

template <int n>
always_inline void notequal_vec_simd_mp_iteration(float * out, const float * src1, const __m128 & src2)
{
    const __m128 in1 = _mm_load_ps(src1);

    const __m128 result = _mm_cmpneq_ps(in1, src2);

    _mm_store_ps(out, result);

    notequal_vec_simd_mp_iteration<n-4>(out+4, src1+4, src2);
}

template <>
always_inline void notequal_vec_simd_mp_iteration<0>(float * out, const float * src1, const __m128 & src2)
{}

template <int n>
always_inline void notequal_vec_simd_mp_iteration(float * out, const __m128 & src1, const float * src2)
{
    const __m128 in2 = _mm_load_ps(src2);

    const __m128 result = _mm_cmpneq_ps(src1, in2);

    _mm_store_ps(out, result);

    notequal_vec_simd_mp_iteration<n-4>(out+4, src1, src2+4);
}

template <>
always_inline void notequal_vec_simd_mp_iteration<0>(float * out, const __m128 & src1, const float * src2)
{}

} /* namespace detail */


/* vector/vector */
using detail::notequal_vec_simd_mp;

template <int n>
void notequal_vec_simd(float * out, const float * src1, const float * src2)
{
    notequal_vec_simd_mp<n>(out, src1, src2);
}

/* vector/scalar */
template <int n>
always_inline void notequal_vec_simd_mp(float * out, const float * src1, const float src2)
{
    __m128 in2 = _mm_set_ps1(src2);
    detail::notequal_vec_simd_mp_iteration<n>(out, src1, in2);
}

template <int n>
void notequal_vec_simd(float * out, const float * src1, const float src2)
{
    notequal_vec_simd_mp<n>(out, src1, src2);
}

/* vector/scalar */
template <int n>
always_inline void notequal_vec_simd_mp(float * out, const float src1, const float * src2)
{
    __m128 in1 = _mm_set_ps1(src1);
    detail::notequal_vec_simd_mp_iteration<n>(out, in1, src2);
}

template <int n>
void notequal_vec_simd(float * out, const float src1, const float * src2)
{
    notequal_vec_simd<n>(out, src1, src2);
}


template <>
inline void notequal_vec_simd(float * out, const float * arg1, const float * arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    do {
        detail::notequal_vec_simd_mp<samples_per_loop>(out, arg1, arg2);
        out += samples_per_loop;
        arg1 += samples_per_loop;
        arg2 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void notequal_vec_simd(float * out, const float * arg1, const float arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    const __m128 in2 = _mm_set_ps1(arg2);
    do {
        detail::notequal_vec_simd_mp_iteration<samples_per_loop>(out, arg1, in2);
        out += samples_per_loop;
        arg1 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void notequal_vec_simd(float * out, const float arg1, const float * arg2, unsigned int n)
{
    unsigned int loops = n / samples_per_loop;
    const __m128 in1 = _mm_set_ps1(arg1);
    do {
        detail::notequal_vec_simd_mp_iteration<samples_per_loop>(out, in1, arg2);
        out += samples_per_loop;
        arg2 += samples_per_loop;
    }
    while (--loops);
}

template <>
inline void notequal_vec_simd(float * out, const float * arg1, float arg2,
                              float arg2_slope, unsigned int n)
{
    unsigned int loops = n / 8;
    __m128 in2   = _mm_set_ps(arg2,
                              arg2 +   arg2_slope,
                              arg2 + 2*arg2_slope,
                              arg2 + 3*arg2_slope);
    __m128 vslope = _mm_set_ps1(4*arg2_slope);

    do {
        __m128 in1 = _mm_load_ps(arg1);
        __m128 result = _mm_cmpneq_ps(in1, in2);

        _mm_store_ps(out, result);
        in2 = _mm_add_ps(in2, vslope);

        in1 = _mm_load_ps(arg1+4);
        result = _mm_cmpneq_ps(in1, in2);

        _mm_store_ps(out+4, result);
        in2 = _mm_add_ps(in2, vslope);

        out += 8;
        arg1 += 8;
    }
    while (--loops);
}

template <>
inline void notequal_vec_simd(float * out, float arg1, const float arg1_slope,
                              const float * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    __m128 in1   = _mm_set_ps(arg1,
                              arg1 +   arg1_slope,
                              arg1 + 2*arg1_slope,
                              arg1 + 3*arg1_slope);
    __m128 vslope = _mm_set_ps1(4*arg1_slope);

    do {
        __m128 in2 = _mm_load_ps(arg2);
        __m128 result = _mm_cmpneq_ps(in1, in2);

        _mm_store_ps(out, result);
        in1 = _mm_add_ps(in1, vslope);

        in2 = _mm_load_ps(arg2+4);
        result = _mm_cmpneq_ps(in1, in2);

        _mm_store_ps(out+4, result);
        in1 = _mm_add_ps(in1, vslope);

        out += 8;
        arg2 += 8;
    }
    while (--loops);
}

#undef always_inline
#undef samples_per_loop

} /* namespace nova */

#endif /* SIMD_BINARY_ARITHMETIC_SSE_HPP */
