//  amplitude modulation
//
//  Copyright (C) 2008 Tim Blechmann
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

#ifndef DSP_AMPMOD_HPP
#define DSP_AMPMOD_HPP

#ifdef __SSE2__
#include <xmmintrin.h>
#include <emmintrin.h>
#include "simd_utils.hpp"
#endif

namespace nova
{

template <typename float_type>
inline void amp_mod(float_type * out, const float_type * in1, const float_type * in2,
                    const float_type amount, unsigned int n)
{
    for (unsigned int i = 0; i != n; ++i)
        out[i] = in1[i] * (float_type(1) + in2[i] * amount);
}

template <typename float_type>
inline void amp_mod(float_type * out, const float_type * in1, const float_type * in2,
                    float_type amount, const float_type amount_slope,
                    unsigned int n)
{
    for (unsigned int i = 0; i != n; ++i)
    {
        out[i] = in1[i] * (float_type(1) + in2[i] * amount);
        amount += amount_slope;
    }
}

template <typename float_type>
inline void amp_mod(float_type * out, const float_type * in1, const float_type * in2,
                    const float_type * amount, unsigned int n)
{
    for (unsigned int i = 0; i != n; ++i)
        out[i] = in1[i] * (float_type(1) + in2[i] * amount[i]);
}

template <typename float_type>
inline void amp_mod4(float_type * out, const float_type * in1, const float_type * in2,
                     const float_type amount, unsigned int n)
{
    amp_mod(out, in1, in2, amount, n);
}

template <typename float_type>
inline void amp_mod4(float_type * out, const float_type * in1, const float_type * in2,
                     float_type amount, const float_type amount_slope,
                     unsigned int n)
{
    amp_mod4(out, in1, in2, amount, amount_slope, n);
}

template <typename float_type>
inline void amp_mod4(float_type * out, const float_type * in1, const float_type * in2,
                     const float_type * amount, unsigned int n)
{
    amp_mod(out, in1, in2, amount, n);
}

#ifdef __SSE2__
namespace detail
{

/** compute x1 * (1 + x2 * amount)  */
__m128 inline amp_mod4_loop(__m128 x1, __m128 x2, __m128 amount, __m128 one)
{
    return _mm_mul_ps(x1,
                      _mm_add_ps(one,
                                 _mm_mul_ps(x2, amount)));
}

} /* namespace detail */

template <>
inline void amp_mod4(float * out, const float * in1, const float * in2,
                     const float amount, unsigned int n)
{
    std::size_t loops = n >> 2;
    const __m128 one = detail::gen_one();

    const __m128 amnt = _mm_set_ps1(amount);

    do
    {
        const __m128 x1 = _mm_load_ps(in1);
        in1 += 4;
        const __m128 x2 = _mm_load_ps(in2);
        in2 += 4;

        const __m128 result = detail::amp_mod4_loop(x1, x2, amnt, one);

        _mm_store_ps(out, result);
        out += 4;
    }
    while (--loops);
}

template <>
inline void amp_mod4(float * out, const float * in1, const float * in2,
                     const float amount, const float amount_slope, unsigned int n)
{
    std::size_t loops = n >> 2;
    const __m128 one = detail::gen_one();

    __m128 amnt = _mm_set_ps(amount,
                             amount + amount_slope,
                             amount + amount_slope + amount_slope,
                             amount + amount_slope + amount_slope + amount_slope);
    const __m128 slope = _mm_set_ps1(4 * amount_slope);

    do
    {
        const __m128 x1 = _mm_load_ps(in1);
        in1 += 4;
        const __m128 x2 = _mm_load_ps(in2);
        in2 += 4;

        const __m128 result = detail::amp_mod4_loop(x1, x2, amnt, one);

        amnt = _mm_add_ps(amnt, slope);

        _mm_store_ps(out, result);
        out += 4;
    }
    while (--loops);
}

template <>
inline void amp_mod4(float * out, const float * in1, const float * in2,
                     const float * amount, unsigned int n)
{
    std::size_t loops = n >> 2;
    const __m128 one = detail::gen_one();

    do
    {
        const __m128 x1 = _mm_load_ps(in1);
        in1 += 4;
        const __m128 x2 = _mm_load_ps(in2);
        in2 += 4;
        const __m128 amnt = _mm_load_ps(amount);
        amount += 4;

        const __m128 result = detail::amp_mod4_loop(x1, x2, amnt, one);

        _mm_store_ps(out, result);
        out += 4;
    }
    while (--loops);
}

#endif /* __SSE2__ */

} /* namespace nova */

#endif /* DSP_AMPMOD_HPP */
