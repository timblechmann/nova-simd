//  templated arithmetic simd functions
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

#ifndef SIMD_BINARY_ARITHMETIC_GENERIC_HPP
#define SIMD_BINARY_ARITHMETIC_GENERIC_HPP

#include <functional>
#include <algorithm>

namespace nova {

template <typename float_type>
inline void plus_vec(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::plus<float_type>()(*arg1++, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void plus_vec(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    do {
        *out++ = std::plus<float_type>()(*arg1++, arg2);
    }
    while (--n);
}

template <typename float_type>
inline void plus_vec(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::plus<float_type>()(arg1, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void plus_vec(float_type * out, const float_type * arg1, float_type arg2,
                  const float_type arg2_slope, unsigned int n)
{
    do {
        *out++ = std::plus<float_type>()(*arg1++, arg2);
        arg2 += arg2_slope;
    }
    while (--n);
}

template <typename float_type>
inline void plus_vec(float_type * out, float_type arg1, const float_type arg1_slope,
                  const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::plus<float_type>()(arg1, *arg2++);
        arg1 += arg1_slope;
    }
    while (--n);
}

template <typename float_type>
inline void plus_vec_simd(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::plus<float_type>()(*arg1++, *arg2++);
        *out++ = std::plus<float_type>()(*arg1++, *arg2++);
        *out++ = std::plus<float_type>()(*arg1++, *arg2++);
        *out++ = std::plus<float_type>()(*arg1++, *arg2++);
        *out++ = std::plus<float_type>()(*arg1++, *arg2++);
        *out++ = std::plus<float_type>()(*arg1++, *arg2++);
        *out++ = std::plus<float_type>()(*arg1++, *arg2++);
        *out++ = std::plus<float_type>()(*arg1++, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void plus_vec_simd(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::plus<float_type>()(*arg1++, arg2);
        *out++ = std::plus<float_type>()(*arg1++, arg2);
        *out++ = std::plus<float_type>()(*arg1++, arg2);
        *out++ = std::plus<float_type>()(*arg1++, arg2);
        *out++ = std::plus<float_type>()(*arg1++, arg2);
        *out++ = std::plus<float_type>()(*arg1++, arg2);
        *out++ = std::plus<float_type>()(*arg1++, arg2);
        *out++ = std::plus<float_type>()(*arg1++, arg2);
    }
    while (--loops);
}

template <typename float_type>
inline void plus_vec_simd(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::plus<float_type>()(arg1, *arg2++);
        *out++ = std::plus<float_type>()(arg1, *arg2++);
        *out++ = std::plus<float_type>()(arg1, *arg2++);
        *out++ = std::plus<float_type>()(arg1, *arg2++);
        *out++ = std::plus<float_type>()(arg1, *arg2++);
        *out++ = std::plus<float_type>()(arg1, *arg2++);
        *out++ = std::plus<float_type>()(arg1, *arg2++);
        *out++ = std::plus<float_type>()(arg1, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void plus_vec_simd(float_type * out, const float_type * arg1, float_type arg2,
                              float_type arg2_slope, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::plus<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::plus<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::plus<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::plus<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::plus<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::plus<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::plus<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::plus<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
    }
    while (--loops);
}

template <typename float_type>
inline void plus_vec_simd(float_type * out, float_type arg1, const float_type arg1_slope,
                              const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::plus<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::plus<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::plus<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::plus<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::plus<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::plus<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::plus<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::plus<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
    }
    while (--loops);
}

template <typename float_type>
inline void minus_vec(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::minus<float_type>()(*arg1++, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void minus_vec(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    do {
        *out++ = std::minus<float_type>()(*arg1++, arg2);
    }
    while (--n);
}

template <typename float_type>
inline void minus_vec(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::minus<float_type>()(arg1, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void minus_vec(float_type * out, const float_type * arg1, float_type arg2,
                  const float_type arg2_slope, unsigned int n)
{
    do {
        *out++ = std::minus<float_type>()(*arg1++, arg2);
        arg2 += arg2_slope;
    }
    while (--n);
}

template <typename float_type>
inline void minus_vec(float_type * out, float_type arg1, const float_type arg1_slope,
                  const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::minus<float_type>()(arg1, *arg2++);
        arg1 += arg1_slope;
    }
    while (--n);
}

template <typename float_type>
inline void minus_vec_simd(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::minus<float_type>()(*arg1++, *arg2++);
        *out++ = std::minus<float_type>()(*arg1++, *arg2++);
        *out++ = std::minus<float_type>()(*arg1++, *arg2++);
        *out++ = std::minus<float_type>()(*arg1++, *arg2++);
        *out++ = std::minus<float_type>()(*arg1++, *arg2++);
        *out++ = std::minus<float_type>()(*arg1++, *arg2++);
        *out++ = std::minus<float_type>()(*arg1++, *arg2++);
        *out++ = std::minus<float_type>()(*arg1++, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void minus_vec_simd(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::minus<float_type>()(*arg1++, arg2);
        *out++ = std::minus<float_type>()(*arg1++, arg2);
        *out++ = std::minus<float_type>()(*arg1++, arg2);
        *out++ = std::minus<float_type>()(*arg1++, arg2);
        *out++ = std::minus<float_type>()(*arg1++, arg2);
        *out++ = std::minus<float_type>()(*arg1++, arg2);
        *out++ = std::minus<float_type>()(*arg1++, arg2);
        *out++ = std::minus<float_type>()(*arg1++, arg2);
    }
    while (--loops);
}

template <typename float_type>
inline void minus_vec_simd(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::minus<float_type>()(arg1, *arg2++);
        *out++ = std::minus<float_type>()(arg1, *arg2++);
        *out++ = std::minus<float_type>()(arg1, *arg2++);
        *out++ = std::minus<float_type>()(arg1, *arg2++);
        *out++ = std::minus<float_type>()(arg1, *arg2++);
        *out++ = std::minus<float_type>()(arg1, *arg2++);
        *out++ = std::minus<float_type>()(arg1, *arg2++);
        *out++ = std::minus<float_type>()(arg1, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void minus_vec_simd(float_type * out, const float_type * arg1, float_type arg2,
                              float_type arg2_slope, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::minus<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::minus<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::minus<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::minus<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::minus<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::minus<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::minus<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::minus<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
    }
    while (--loops);
}

template <typename float_type>
inline void minus_vec_simd(float_type * out, float_type arg1, const float_type arg1_slope,
                              const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::minus<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::minus<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::minus<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::minus<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::minus<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::minus<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::minus<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::minus<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
    }
    while (--loops);
}

template <typename float_type>
inline void times_vec(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::multiplies<float_type>()(*arg1++, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void times_vec(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    do {
        *out++ = std::multiplies<float_type>()(*arg1++, arg2);
    }
    while (--n);
}

template <typename float_type>
inline void times_vec(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::multiplies<float_type>()(arg1, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void times_vec(float_type * out, const float_type * arg1, float_type arg2,
                  const float_type arg2_slope, unsigned int n)
{
    do {
        *out++ = std::multiplies<float_type>()(*arg1++, arg2);
        arg2 += arg2_slope;
    }
    while (--n);
}

template <typename float_type>
inline void times_vec(float_type * out, float_type arg1, const float_type arg1_slope,
                  const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::multiplies<float_type>()(arg1, *arg2++);
        arg1 += arg1_slope;
    }
    while (--n);
}

template <typename float_type>
inline void times_vec_simd(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::multiplies<float_type>()(*arg1++, *arg2++);
        *out++ = std::multiplies<float_type>()(*arg1++, *arg2++);
        *out++ = std::multiplies<float_type>()(*arg1++, *arg2++);
        *out++ = std::multiplies<float_type>()(*arg1++, *arg2++);
        *out++ = std::multiplies<float_type>()(*arg1++, *arg2++);
        *out++ = std::multiplies<float_type>()(*arg1++, *arg2++);
        *out++ = std::multiplies<float_type>()(*arg1++, *arg2++);
        *out++ = std::multiplies<float_type>()(*arg1++, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void times_vec_simd(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::multiplies<float_type>()(*arg1++, arg2);
        *out++ = std::multiplies<float_type>()(*arg1++, arg2);
        *out++ = std::multiplies<float_type>()(*arg1++, arg2);
        *out++ = std::multiplies<float_type>()(*arg1++, arg2);
        *out++ = std::multiplies<float_type>()(*arg1++, arg2);
        *out++ = std::multiplies<float_type>()(*arg1++, arg2);
        *out++ = std::multiplies<float_type>()(*arg1++, arg2);
        *out++ = std::multiplies<float_type>()(*arg1++, arg2);
    }
    while (--loops);
}

template <typename float_type>
inline void times_vec_simd(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::multiplies<float_type>()(arg1, *arg2++);
        *out++ = std::multiplies<float_type>()(arg1, *arg2++);
        *out++ = std::multiplies<float_type>()(arg1, *arg2++);
        *out++ = std::multiplies<float_type>()(arg1, *arg2++);
        *out++ = std::multiplies<float_type>()(arg1, *arg2++);
        *out++ = std::multiplies<float_type>()(arg1, *arg2++);
        *out++ = std::multiplies<float_type>()(arg1, *arg2++);
        *out++ = std::multiplies<float_type>()(arg1, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void times_vec_simd(float_type * out, const float_type * arg1, float_type arg2,
                              float_type arg2_slope, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::multiplies<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::multiplies<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::multiplies<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::multiplies<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::multiplies<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::multiplies<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::multiplies<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::multiplies<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
    }
    while (--loops);
}

template <typename float_type>
inline void times_vec_simd(float_type * out, float_type arg1, const float_type arg1_slope,
                              const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::multiplies<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::multiplies<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::multiplies<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::multiplies<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::multiplies<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::multiplies<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::multiplies<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::multiplies<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
    }
    while (--loops);
}

template <typename float_type>
inline void over_vec(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::divides<float_type>()(*arg1++, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void over_vec(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    do {
        *out++ = std::divides<float_type>()(*arg1++, arg2);
    }
    while (--n);
}

template <typename float_type>
inline void over_vec(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::divides<float_type>()(arg1, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void over_vec(float_type * out, const float_type * arg1, float_type arg2,
                  const float_type arg2_slope, unsigned int n)
{
    do {
        *out++ = std::divides<float_type>()(*arg1++, arg2);
        arg2 += arg2_slope;
    }
    while (--n);
}

template <typename float_type>
inline void over_vec(float_type * out, float_type arg1, const float_type arg1_slope,
                  const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::divides<float_type>()(arg1, *arg2++);
        arg1 += arg1_slope;
    }
    while (--n);
}

template <typename float_type>
inline void over_vec_simd(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::divides<float_type>()(*arg1++, *arg2++);
        *out++ = std::divides<float_type>()(*arg1++, *arg2++);
        *out++ = std::divides<float_type>()(*arg1++, *arg2++);
        *out++ = std::divides<float_type>()(*arg1++, *arg2++);
        *out++ = std::divides<float_type>()(*arg1++, *arg2++);
        *out++ = std::divides<float_type>()(*arg1++, *arg2++);
        *out++ = std::divides<float_type>()(*arg1++, *arg2++);
        *out++ = std::divides<float_type>()(*arg1++, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void over_vec_simd(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::divides<float_type>()(*arg1++, arg2);
        *out++ = std::divides<float_type>()(*arg1++, arg2);
        *out++ = std::divides<float_type>()(*arg1++, arg2);
        *out++ = std::divides<float_type>()(*arg1++, arg2);
        *out++ = std::divides<float_type>()(*arg1++, arg2);
        *out++ = std::divides<float_type>()(*arg1++, arg2);
        *out++ = std::divides<float_type>()(*arg1++, arg2);
        *out++ = std::divides<float_type>()(*arg1++, arg2);
    }
    while (--loops);
}

template <typename float_type>
inline void over_vec_simd(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::divides<float_type>()(arg1, *arg2++);
        *out++ = std::divides<float_type>()(arg1, *arg2++);
        *out++ = std::divides<float_type>()(arg1, *arg2++);
        *out++ = std::divides<float_type>()(arg1, *arg2++);
        *out++ = std::divides<float_type>()(arg1, *arg2++);
        *out++ = std::divides<float_type>()(arg1, *arg2++);
        *out++ = std::divides<float_type>()(arg1, *arg2++);
        *out++ = std::divides<float_type>()(arg1, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void over_vec_simd(float_type * out, const float_type * arg1, float_type arg2,
                              float_type arg2_slope, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::divides<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::divides<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::divides<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::divides<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::divides<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::divides<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::divides<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::divides<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
    }
    while (--loops);
}

template <typename float_type>
inline void over_vec_simd(float_type * out, float_type arg1, const float_type arg1_slope,
                              const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::divides<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::divides<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::divides<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::divides<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::divides<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::divides<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::divides<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::divides<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
    }
    while (--loops);
}

template <typename float_type>
inline void min_vec(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::min<float_type>(*arg1++, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void min_vec(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    do {
        *out++ = std::min<float_type>(*arg1++, arg2);
    }
    while (--n);
}

template <typename float_type>
inline void min_vec(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::min<float_type>(arg1, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void min_vec(float_type * out, const float_type * arg1, float_type arg2,
                  const float_type arg2_slope, unsigned int n)
{
    do {
        *out++ = std::min<float_type>(*arg1++, arg2);
        arg2 += arg2_slope;
    }
    while (--n);
}

template <typename float_type>
inline void min_vec(float_type * out, float_type arg1, const float_type arg1_slope,
                  const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::min<float_type>(arg1, *arg2++);
        arg1 += arg1_slope;
    }
    while (--n);
}

template <typename float_type>
inline void min_vec_simd(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::min<float_type>(*arg1++, *arg2++);
        *out++ = std::min<float_type>(*arg1++, *arg2++);
        *out++ = std::min<float_type>(*arg1++, *arg2++);
        *out++ = std::min<float_type>(*arg1++, *arg2++);
        *out++ = std::min<float_type>(*arg1++, *arg2++);
        *out++ = std::min<float_type>(*arg1++, *arg2++);
        *out++ = std::min<float_type>(*arg1++, *arg2++);
        *out++ = std::min<float_type>(*arg1++, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void min_vec_simd(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::min<float_type>(*arg1++, arg2);
        *out++ = std::min<float_type>(*arg1++, arg2);
        *out++ = std::min<float_type>(*arg1++, arg2);
        *out++ = std::min<float_type>(*arg1++, arg2);
        *out++ = std::min<float_type>(*arg1++, arg2);
        *out++ = std::min<float_type>(*arg1++, arg2);
        *out++ = std::min<float_type>(*arg1++, arg2);
        *out++ = std::min<float_type>(*arg1++, arg2);
    }
    while (--loops);
}

template <typename float_type>
inline void min_vec_simd(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::min<float_type>(arg1, *arg2++);
        *out++ = std::min<float_type>(arg1, *arg2++);
        *out++ = std::min<float_type>(arg1, *arg2++);
        *out++ = std::min<float_type>(arg1, *arg2++);
        *out++ = std::min<float_type>(arg1, *arg2++);
        *out++ = std::min<float_type>(arg1, *arg2++);
        *out++ = std::min<float_type>(arg1, *arg2++);
        *out++ = std::min<float_type>(arg1, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void min_vec_simd(float_type * out, const float_type * arg1, float_type arg2,
                              float_type arg2_slope, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::min<float_type>(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::min<float_type>(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::min<float_type>(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::min<float_type>(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::min<float_type>(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::min<float_type>(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::min<float_type>(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::min<float_type>(*arg1++, arg2); arg2 += arg2_slope;
    }
    while (--loops);
}

template <typename float_type>
inline void min_vec_simd(float_type * out, float_type arg1, const float_type arg1_slope,
                              const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::min<float_type>(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::min<float_type>(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::min<float_type>(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::min<float_type>(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::min<float_type>(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::min<float_type>(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::min<float_type>(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::min<float_type>(arg1, *arg2++); arg1 += arg1_slope;
    }
    while (--loops);
}

template <typename float_type>
inline void max_vec(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::max<float_type>(*arg1++, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void max_vec(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    do {
        *out++ = std::max<float_type>(*arg1++, arg2);
    }
    while (--n);
}

template <typename float_type>
inline void max_vec(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::max<float_type>(arg1, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void max_vec(float_type * out, const float_type * arg1, float_type arg2,
                  const float_type arg2_slope, unsigned int n)
{
    do {
        *out++ = std::max<float_type>(*arg1++, arg2);
        arg2 += arg2_slope;
    }
    while (--n);
}

template <typename float_type>
inline void max_vec(float_type * out, float_type arg1, const float_type arg1_slope,
                  const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::max<float_type>(arg1, *arg2++);
        arg1 += arg1_slope;
    }
    while (--n);
}

template <typename float_type>
inline void max_vec_simd(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::max<float_type>(*arg1++, *arg2++);
        *out++ = std::max<float_type>(*arg1++, *arg2++);
        *out++ = std::max<float_type>(*arg1++, *arg2++);
        *out++ = std::max<float_type>(*arg1++, *arg2++);
        *out++ = std::max<float_type>(*arg1++, *arg2++);
        *out++ = std::max<float_type>(*arg1++, *arg2++);
        *out++ = std::max<float_type>(*arg1++, *arg2++);
        *out++ = std::max<float_type>(*arg1++, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void max_vec_simd(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::max<float_type>(*arg1++, arg2);
        *out++ = std::max<float_type>(*arg1++, arg2);
        *out++ = std::max<float_type>(*arg1++, arg2);
        *out++ = std::max<float_type>(*arg1++, arg2);
        *out++ = std::max<float_type>(*arg1++, arg2);
        *out++ = std::max<float_type>(*arg1++, arg2);
        *out++ = std::max<float_type>(*arg1++, arg2);
        *out++ = std::max<float_type>(*arg1++, arg2);
    }
    while (--loops);
}

template <typename float_type>
inline void max_vec_simd(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::max<float_type>(arg1, *arg2++);
        *out++ = std::max<float_type>(arg1, *arg2++);
        *out++ = std::max<float_type>(arg1, *arg2++);
        *out++ = std::max<float_type>(arg1, *arg2++);
        *out++ = std::max<float_type>(arg1, *arg2++);
        *out++ = std::max<float_type>(arg1, *arg2++);
        *out++ = std::max<float_type>(arg1, *arg2++);
        *out++ = std::max<float_type>(arg1, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void max_vec_simd(float_type * out, const float_type * arg1, float_type arg2,
                              float_type arg2_slope, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::max<float_type>(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::max<float_type>(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::max<float_type>(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::max<float_type>(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::max<float_type>(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::max<float_type>(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::max<float_type>(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::max<float_type>(*arg1++, arg2); arg2 += arg2_slope;
    }
    while (--loops);
}

template <typename float_type>
inline void max_vec_simd(float_type * out, float_type arg1, const float_type arg1_slope,
                              const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::max<float_type>(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::max<float_type>(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::max<float_type>(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::max<float_type>(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::max<float_type>(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::max<float_type>(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::max<float_type>(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::max<float_type>(arg1, *arg2++); arg1 += arg1_slope;
    }
    while (--loops);
}

template <typename float_type>
inline void less_vec(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::less<float_type>()(*arg1++, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void less_vec(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    do {
        *out++ = std::less<float_type>()(*arg1++, arg2);
    }
    while (--n);
}

template <typename float_type>
inline void less_vec(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::less<float_type>()(arg1, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void less_vec(float_type * out, const float_type * arg1, float_type arg2,
                  const float_type arg2_slope, unsigned int n)
{
    do {
        *out++ = std::less<float_type>()(*arg1++, arg2);
        arg2 += arg2_slope;
    }
    while (--n);
}

template <typename float_type>
inline void less_vec(float_type * out, float_type arg1, const float_type arg1_slope,
                  const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::less<float_type>()(arg1, *arg2++);
        arg1 += arg1_slope;
    }
    while (--n);
}

template <typename float_type>
inline void less_vec_simd(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::less<float_type>()(*arg1++, *arg2++);
        *out++ = std::less<float_type>()(*arg1++, *arg2++);
        *out++ = std::less<float_type>()(*arg1++, *arg2++);
        *out++ = std::less<float_type>()(*arg1++, *arg2++);
        *out++ = std::less<float_type>()(*arg1++, *arg2++);
        *out++ = std::less<float_type>()(*arg1++, *arg2++);
        *out++ = std::less<float_type>()(*arg1++, *arg2++);
        *out++ = std::less<float_type>()(*arg1++, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void less_vec_simd(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::less<float_type>()(*arg1++, arg2);
        *out++ = std::less<float_type>()(*arg1++, arg2);
        *out++ = std::less<float_type>()(*arg1++, arg2);
        *out++ = std::less<float_type>()(*arg1++, arg2);
        *out++ = std::less<float_type>()(*arg1++, arg2);
        *out++ = std::less<float_type>()(*arg1++, arg2);
        *out++ = std::less<float_type>()(*arg1++, arg2);
        *out++ = std::less<float_type>()(*arg1++, arg2);
    }
    while (--loops);
}

template <typename float_type>
inline void less_vec_simd(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::less<float_type>()(arg1, *arg2++);
        *out++ = std::less<float_type>()(arg1, *arg2++);
        *out++ = std::less<float_type>()(arg1, *arg2++);
        *out++ = std::less<float_type>()(arg1, *arg2++);
        *out++ = std::less<float_type>()(arg1, *arg2++);
        *out++ = std::less<float_type>()(arg1, *arg2++);
        *out++ = std::less<float_type>()(arg1, *arg2++);
        *out++ = std::less<float_type>()(arg1, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void less_vec_simd(float_type * out, const float_type * arg1, float_type arg2,
                              float_type arg2_slope, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::less<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::less<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::less<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::less<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::less<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::less<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::less<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::less<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
    }
    while (--loops);
}

template <typename float_type>
inline void less_vec_simd(float_type * out, float_type arg1, const float_type arg1_slope,
                              const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::less<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::less<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::less<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::less<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::less<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::less<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::less<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::less<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
    }
    while (--loops);
}

template <typename float_type>
inline void less_equal_vec(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::less_equal<float_type>()(*arg1++, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void less_equal_vec(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    do {
        *out++ = std::less_equal<float_type>()(*arg1++, arg2);
    }
    while (--n);
}

template <typename float_type>
inline void less_equal_vec(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::less_equal<float_type>()(arg1, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void less_equal_vec(float_type * out, const float_type * arg1, float_type arg2,
                  const float_type arg2_slope, unsigned int n)
{
    do {
        *out++ = std::less_equal<float_type>()(*arg1++, arg2);
        arg2 += arg2_slope;
    }
    while (--n);
}

template <typename float_type>
inline void less_equal_vec(float_type * out, float_type arg1, const float_type arg1_slope,
                  const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::less_equal<float_type>()(arg1, *arg2++);
        arg1 += arg1_slope;
    }
    while (--n);
}

template <typename float_type>
inline void less_equal_vec_simd(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::less_equal<float_type>()(*arg1++, *arg2++);
        *out++ = std::less_equal<float_type>()(*arg1++, *arg2++);
        *out++ = std::less_equal<float_type>()(*arg1++, *arg2++);
        *out++ = std::less_equal<float_type>()(*arg1++, *arg2++);
        *out++ = std::less_equal<float_type>()(*arg1++, *arg2++);
        *out++ = std::less_equal<float_type>()(*arg1++, *arg2++);
        *out++ = std::less_equal<float_type>()(*arg1++, *arg2++);
        *out++ = std::less_equal<float_type>()(*arg1++, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void less_equal_vec_simd(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::less_equal<float_type>()(*arg1++, arg2);
        *out++ = std::less_equal<float_type>()(*arg1++, arg2);
        *out++ = std::less_equal<float_type>()(*arg1++, arg2);
        *out++ = std::less_equal<float_type>()(*arg1++, arg2);
        *out++ = std::less_equal<float_type>()(*arg1++, arg2);
        *out++ = std::less_equal<float_type>()(*arg1++, arg2);
        *out++ = std::less_equal<float_type>()(*arg1++, arg2);
        *out++ = std::less_equal<float_type>()(*arg1++, arg2);
    }
    while (--loops);
}

template <typename float_type>
inline void less_equal_vec_simd(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::less_equal<float_type>()(arg1, *arg2++);
        *out++ = std::less_equal<float_type>()(arg1, *arg2++);
        *out++ = std::less_equal<float_type>()(arg1, *arg2++);
        *out++ = std::less_equal<float_type>()(arg1, *arg2++);
        *out++ = std::less_equal<float_type>()(arg1, *arg2++);
        *out++ = std::less_equal<float_type>()(arg1, *arg2++);
        *out++ = std::less_equal<float_type>()(arg1, *arg2++);
        *out++ = std::less_equal<float_type>()(arg1, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void less_equal_vec_simd(float_type * out, const float_type * arg1, float_type arg2,
                              float_type arg2_slope, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::less_equal<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::less_equal<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::less_equal<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::less_equal<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::less_equal<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::less_equal<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::less_equal<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::less_equal<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
    }
    while (--loops);
}

template <typename float_type>
inline void less_equal_vec_simd(float_type * out, float_type arg1, const float_type arg1_slope,
                              const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::less_equal<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::less_equal<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::less_equal<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::less_equal<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::less_equal<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::less_equal<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::less_equal<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::less_equal<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
    }
    while (--loops);
}

template <typename float_type>
inline void greater_vec(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::greater<float_type>()(*arg1++, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void greater_vec(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    do {
        *out++ = std::greater<float_type>()(*arg1++, arg2);
    }
    while (--n);
}

template <typename float_type>
inline void greater_vec(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::greater<float_type>()(arg1, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void greater_vec(float_type * out, const float_type * arg1, float_type arg2,
                  const float_type arg2_slope, unsigned int n)
{
    do {
        *out++ = std::greater<float_type>()(*arg1++, arg2);
        arg2 += arg2_slope;
    }
    while (--n);
}

template <typename float_type>
inline void greater_vec(float_type * out, float_type arg1, const float_type arg1_slope,
                  const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::greater<float_type>()(arg1, *arg2++);
        arg1 += arg1_slope;
    }
    while (--n);
}

template <typename float_type>
inline void greater_vec_simd(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::greater<float_type>()(*arg1++, *arg2++);
        *out++ = std::greater<float_type>()(*arg1++, *arg2++);
        *out++ = std::greater<float_type>()(*arg1++, *arg2++);
        *out++ = std::greater<float_type>()(*arg1++, *arg2++);
        *out++ = std::greater<float_type>()(*arg1++, *arg2++);
        *out++ = std::greater<float_type>()(*arg1++, *arg2++);
        *out++ = std::greater<float_type>()(*arg1++, *arg2++);
        *out++ = std::greater<float_type>()(*arg1++, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void greater_vec_simd(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::greater<float_type>()(*arg1++, arg2);
        *out++ = std::greater<float_type>()(*arg1++, arg2);
        *out++ = std::greater<float_type>()(*arg1++, arg2);
        *out++ = std::greater<float_type>()(*arg1++, arg2);
        *out++ = std::greater<float_type>()(*arg1++, arg2);
        *out++ = std::greater<float_type>()(*arg1++, arg2);
        *out++ = std::greater<float_type>()(*arg1++, arg2);
        *out++ = std::greater<float_type>()(*arg1++, arg2);
    }
    while (--loops);
}

template <typename float_type>
inline void greater_vec_simd(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::greater<float_type>()(arg1, *arg2++);
        *out++ = std::greater<float_type>()(arg1, *arg2++);
        *out++ = std::greater<float_type>()(arg1, *arg2++);
        *out++ = std::greater<float_type>()(arg1, *arg2++);
        *out++ = std::greater<float_type>()(arg1, *arg2++);
        *out++ = std::greater<float_type>()(arg1, *arg2++);
        *out++ = std::greater<float_type>()(arg1, *arg2++);
        *out++ = std::greater<float_type>()(arg1, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void greater_vec_simd(float_type * out, const float_type * arg1, float_type arg2,
                              float_type arg2_slope, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::greater<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::greater<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::greater<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::greater<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::greater<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::greater<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::greater<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::greater<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
    }
    while (--loops);
}

template <typename float_type>
inline void greater_vec_simd(float_type * out, float_type arg1, const float_type arg1_slope,
                              const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::greater<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::greater<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::greater<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::greater<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::greater<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::greater<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::greater<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::greater<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
    }
    while (--loops);
}

template <typename float_type>
inline void greater_equal_vec(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::greater_equal<float_type>()(*arg1++, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void greater_equal_vec(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    do {
        *out++ = std::greater_equal<float_type>()(*arg1++, arg2);
    }
    while (--n);
}

template <typename float_type>
inline void greater_equal_vec(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::greater_equal<float_type>()(arg1, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void greater_equal_vec(float_type * out, const float_type * arg1, float_type arg2,
                  const float_type arg2_slope, unsigned int n)
{
    do {
        *out++ = std::greater_equal<float_type>()(*arg1++, arg2);
        arg2 += arg2_slope;
    }
    while (--n);
}

template <typename float_type>
inline void greater_equal_vec(float_type * out, float_type arg1, const float_type arg1_slope,
                  const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::greater_equal<float_type>()(arg1, *arg2++);
        arg1 += arg1_slope;
    }
    while (--n);
}

template <typename float_type>
inline void greater_equal_vec_simd(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::greater_equal<float_type>()(*arg1++, *arg2++);
        *out++ = std::greater_equal<float_type>()(*arg1++, *arg2++);
        *out++ = std::greater_equal<float_type>()(*arg1++, *arg2++);
        *out++ = std::greater_equal<float_type>()(*arg1++, *arg2++);
        *out++ = std::greater_equal<float_type>()(*arg1++, *arg2++);
        *out++ = std::greater_equal<float_type>()(*arg1++, *arg2++);
        *out++ = std::greater_equal<float_type>()(*arg1++, *arg2++);
        *out++ = std::greater_equal<float_type>()(*arg1++, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void greater_equal_vec_simd(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::greater_equal<float_type>()(*arg1++, arg2);
        *out++ = std::greater_equal<float_type>()(*arg1++, arg2);
        *out++ = std::greater_equal<float_type>()(*arg1++, arg2);
        *out++ = std::greater_equal<float_type>()(*arg1++, arg2);
        *out++ = std::greater_equal<float_type>()(*arg1++, arg2);
        *out++ = std::greater_equal<float_type>()(*arg1++, arg2);
        *out++ = std::greater_equal<float_type>()(*arg1++, arg2);
        *out++ = std::greater_equal<float_type>()(*arg1++, arg2);
    }
    while (--loops);
}

template <typename float_type>
inline void greater_equal_vec_simd(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::greater_equal<float_type>()(arg1, *arg2++);
        *out++ = std::greater_equal<float_type>()(arg1, *arg2++);
        *out++ = std::greater_equal<float_type>()(arg1, *arg2++);
        *out++ = std::greater_equal<float_type>()(arg1, *arg2++);
        *out++ = std::greater_equal<float_type>()(arg1, *arg2++);
        *out++ = std::greater_equal<float_type>()(arg1, *arg2++);
        *out++ = std::greater_equal<float_type>()(arg1, *arg2++);
        *out++ = std::greater_equal<float_type>()(arg1, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void greater_equal_vec_simd(float_type * out, const float_type * arg1, float_type arg2,
                              float_type arg2_slope, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::greater_equal<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::greater_equal<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::greater_equal<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::greater_equal<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::greater_equal<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::greater_equal<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::greater_equal<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::greater_equal<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
    }
    while (--loops);
}

template <typename float_type>
inline void greater_equal_vec_simd(float_type * out, float_type arg1, const float_type arg1_slope,
                              const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::greater_equal<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::greater_equal<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::greater_equal<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::greater_equal<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::greater_equal<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::greater_equal<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::greater_equal<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::greater_equal<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
    }
    while (--loops);
}

template <typename float_type>
inline void equal_vec(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::equal_to<float_type>()(*arg1++, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void equal_vec(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    do {
        *out++ = std::equal_to<float_type>()(*arg1++, arg2);
    }
    while (--n);
}

template <typename float_type>
inline void equal_vec(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::equal_to<float_type>()(arg1, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void equal_vec(float_type * out, const float_type * arg1, float_type arg2,
                  const float_type arg2_slope, unsigned int n)
{
    do {
        *out++ = std::equal_to<float_type>()(*arg1++, arg2);
        arg2 += arg2_slope;
    }
    while (--n);
}

template <typename float_type>
inline void equal_vec(float_type * out, float_type arg1, const float_type arg1_slope,
                  const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::equal_to<float_type>()(arg1, *arg2++);
        arg1 += arg1_slope;
    }
    while (--n);
}

template <typename float_type>
inline void equal_vec_simd(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::equal_to<float_type>()(*arg1++, *arg2++);
        *out++ = std::equal_to<float_type>()(*arg1++, *arg2++);
        *out++ = std::equal_to<float_type>()(*arg1++, *arg2++);
        *out++ = std::equal_to<float_type>()(*arg1++, *arg2++);
        *out++ = std::equal_to<float_type>()(*arg1++, *arg2++);
        *out++ = std::equal_to<float_type>()(*arg1++, *arg2++);
        *out++ = std::equal_to<float_type>()(*arg1++, *arg2++);
        *out++ = std::equal_to<float_type>()(*arg1++, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void equal_vec_simd(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::equal_to<float_type>()(*arg1++, arg2);
        *out++ = std::equal_to<float_type>()(*arg1++, arg2);
        *out++ = std::equal_to<float_type>()(*arg1++, arg2);
        *out++ = std::equal_to<float_type>()(*arg1++, arg2);
        *out++ = std::equal_to<float_type>()(*arg1++, arg2);
        *out++ = std::equal_to<float_type>()(*arg1++, arg2);
        *out++ = std::equal_to<float_type>()(*arg1++, arg2);
        *out++ = std::equal_to<float_type>()(*arg1++, arg2);
    }
    while (--loops);
}

template <typename float_type>
inline void equal_vec_simd(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::equal_to<float_type>()(arg1, *arg2++);
        *out++ = std::equal_to<float_type>()(arg1, *arg2++);
        *out++ = std::equal_to<float_type>()(arg1, *arg2++);
        *out++ = std::equal_to<float_type>()(arg1, *arg2++);
        *out++ = std::equal_to<float_type>()(arg1, *arg2++);
        *out++ = std::equal_to<float_type>()(arg1, *arg2++);
        *out++ = std::equal_to<float_type>()(arg1, *arg2++);
        *out++ = std::equal_to<float_type>()(arg1, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void equal_vec_simd(float_type * out, const float_type * arg1, float_type arg2,
                              float_type arg2_slope, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::equal_to<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::equal_to<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::equal_to<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::equal_to<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::equal_to<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::equal_to<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::equal_to<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::equal_to<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
    }
    while (--loops);
}

template <typename float_type>
inline void equal_vec_simd(float_type * out, float_type arg1, const float_type arg1_slope,
                              const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::equal_to<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::equal_to<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::equal_to<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::equal_to<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::equal_to<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::equal_to<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::equal_to<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::equal_to<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
    }
    while (--loops);
}

template <typename float_type>
inline void notequal_vec(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::not_equal_to<float_type>()(*arg1++, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void notequal_vec(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    do {
        *out++ = std::not_equal_to<float_type>()(*arg1++, arg2);
    }
    while (--n);
}

template <typename float_type>
inline void notequal_vec(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::not_equal_to<float_type>()(arg1, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void notequal_vec(float_type * out, const float_type * arg1, float_type arg2,
                  const float_type arg2_slope, unsigned int n)
{
    do {
        *out++ = std::not_equal_to<float_type>()(*arg1++, arg2);
        arg2 += arg2_slope;
    }
    while (--n);
}

template <typename float_type>
inline void notequal_vec(float_type * out, float_type arg1, const float_type arg1_slope,
                  const float_type * arg2, unsigned int n)
{
    do {
        *out++ = std::not_equal_to<float_type>()(arg1, *arg2++);
        arg1 += arg1_slope;
    }
    while (--n);
}

template <typename float_type>
inline void notequal_vec_simd(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::not_equal_to<float_type>()(*arg1++, *arg2++);
        *out++ = std::not_equal_to<float_type>()(*arg1++, *arg2++);
        *out++ = std::not_equal_to<float_type>()(*arg1++, *arg2++);
        *out++ = std::not_equal_to<float_type>()(*arg1++, *arg2++);
        *out++ = std::not_equal_to<float_type>()(*arg1++, *arg2++);
        *out++ = std::not_equal_to<float_type>()(*arg1++, *arg2++);
        *out++ = std::not_equal_to<float_type>()(*arg1++, *arg2++);
        *out++ = std::not_equal_to<float_type>()(*arg1++, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void notequal_vec_simd(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::not_equal_to<float_type>()(*arg1++, arg2);
        *out++ = std::not_equal_to<float_type>()(*arg1++, arg2);
        *out++ = std::not_equal_to<float_type>()(*arg1++, arg2);
        *out++ = std::not_equal_to<float_type>()(*arg1++, arg2);
        *out++ = std::not_equal_to<float_type>()(*arg1++, arg2);
        *out++ = std::not_equal_to<float_type>()(*arg1++, arg2);
        *out++ = std::not_equal_to<float_type>()(*arg1++, arg2);
        *out++ = std::not_equal_to<float_type>()(*arg1++, arg2);
    }
    while (--loops);
}

template <typename float_type>
inline void notequal_vec_simd(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::not_equal_to<float_type>()(arg1, *arg2++);
        *out++ = std::not_equal_to<float_type>()(arg1, *arg2++);
        *out++ = std::not_equal_to<float_type>()(arg1, *arg2++);
        *out++ = std::not_equal_to<float_type>()(arg1, *arg2++);
        *out++ = std::not_equal_to<float_type>()(arg1, *arg2++);
        *out++ = std::not_equal_to<float_type>()(arg1, *arg2++);
        *out++ = std::not_equal_to<float_type>()(arg1, *arg2++);
        *out++ = std::not_equal_to<float_type>()(arg1, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void notequal_vec_simd(float_type * out, const float_type * arg1, float_type arg2,
                              float_type arg2_slope, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::not_equal_to<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::not_equal_to<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::not_equal_to<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::not_equal_to<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::not_equal_to<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::not_equal_to<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::not_equal_to<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = std::not_equal_to<float_type>()(*arg1++, arg2); arg2 += arg2_slope;
    }
    while (--loops);
}

template <typename float_type>
inline void notequal_vec_simd(float_type * out, float_type arg1, const float_type arg1_slope,
                              const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = std::not_equal_to<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::not_equal_to<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::not_equal_to<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::not_equal_to<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::not_equal_to<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::not_equal_to<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::not_equal_to<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = std::not_equal_to<float_type>()(arg1, *arg2++); arg1 += arg1_slope;
    }
    while (--loops);
}

} /* namespace nova */

#endif /* SIMD_BINARY_ARITHMETIC_GENERIC_HPP */
