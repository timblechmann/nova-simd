//  genertic math functions
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

#ifndef NOVA_SIMD_DETAIL_MATH_HPP
#define NOVA_SIMD_DETAIL_MATH_HPP

#include <algorithm>
#include <cmath>
#include <functional>


namespace nova {
namespace detail {

///@{
template <typename FloatType,
          int VectorSize,
          typename Functor
         >
inline void apply_on_vector(FloatType * out, const FloatType * in, Functor f)
{
    for (int i = 0; i != VectorSize; ++i)
        out[i] = f(in[i]);
}

template <typename FloatType,
          int VectorSize,
          typename Functor
         >
inline void apply_on_vector(FloatType * out, const FloatType * in1, const FloatType * in2, Functor f)
{
    for (int i = 0; i != VectorSize; ++i)
        out[i] = f(in1[i], in2[i]);
}
///@}

///@{
template <typename FloatType,
          typename Functor
         >
inline void apply_on_vector(FloatType * out, const FloatType * in, unsigned int n, Functor f)
{
    do
        *out++ = f(*in++);
    while (--n);
}

template <typename FloatType,
          typename Functor
         >
inline void apply_on_vector(FloatType * out, const FloatType * in1, const FloatType * in2, unsigned int n, Functor f)
{
    do
        *out++ = f(*in1++, *in2++);
    while (--n);
}

template <typename FloatType,
          typename Functor
         >
inline void apply_on_vector(FloatType * out, FloatType in1, const FloatType * in2, unsigned int n, Functor f)
{
    do
        *out++ = f(in1, *in2++);
    while (--n);
}

template <typename FloatType,
          typename Functor
         >
inline void apply_on_vector(FloatType * out, const FloatType * in1, FloatType in2, unsigned int n, Functor f)
{
    do
        *out++ = f(*in1++, in2);
    while (--n);
}


template <typename FloatType,
          typename Functor
         >
inline void apply_on_vector(FloatType * out, FloatType in1, FloatType in1_slope, const FloatType * in2, unsigned int n, Functor f)
{
    do {
        *out++ = f(in1, *in2++); in1 += in1_slope;
    } while (--n);
}

template <typename FloatType,
          typename Functor
         >
inline void apply_on_vector(FloatType * out, const FloatType * in1, FloatType in2, FloatType in2_slope, unsigned int n, Functor f)
{
    do {
        *out++ = f(*in1++, in2); in2 += in2_slope;
    } while (--n);
}
///@}


template<typename float_type>
float_type sign(float_type const & f)
{
    if (f > 0)
        return 1;
    if (f == 0)
        return 0;
    else
        return -1;
}

template<typename float_type>
float_type square(float_type const & f)
{
    return f*f;
}

template<typename float_type>
float_type cube(float_type const & f)
{
    return f*f*f;
}


template<typename float_type>
float_type fabs(float_type f)
{
    return std::fabs(f);
}

template<typename float_type>
float_type floor(float_type const & f)
{
    return std::floor(f);
}

template<typename float_type>
float_type ceil(float_type const & f)
{
    return std::ceil(f);
}


template<typename float_type>
float_type min(float_type const & x, float_type const & y)
{
    return std::min(x, y);
}

template<typename float_type>
float_type max(float_type const & x, float_type const & y)
{
    return std::max(x, y);
}

template<typename float_type>
float_type round(float_type const & arg)
{
    return std::floor(arg + float_type(0.5));
}

template<>
double round<double>(double const & arg)
{
    return ::round(arg);
}

#if _XOPEN_SOURCE >= 600 || _ISOC99_SOURCE
template<>
float round<float>(float const & arg)
{
    return ::roundf(arg);
}
#endif

template<typename float_type>
float_type frac(float_type const & arg)
{
    return arg - floor<float_type>(arg);
}

}
}

#endif /* NOVA_SIMD_DETAIL_MATH_HPP */
