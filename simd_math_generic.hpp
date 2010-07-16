//  simdfied mathematical functions, generic implementation
//  Copyright (C) 2009 Tim Blechmann
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

#ifndef SIMD_MATH_GENERIC_HPP
#define SIMD_MATH_GENERIC_HPP

#include <cmath>

namespace nova
{

#define WRAP_MATH_FUNCTION_BINARY(name, function)                       \
    template <typename float_type>                                      \
    inline void name##_vec(float_type * out, const float_type * in0, const float_type * in1, unsigned int n) \
    {                                                                   \
        do {                                                            \
            *out++ = function(*in0++, *in1++);                          \
        }                                                               \
        while (--n);                                                    \
    }                                                                   \
                                                                        \
    template <typename float_type>                                      \
    inline void name##_vec_simd(float_type * out, const float_type * in0, const float_type * in1, unsigned int n) \
    {                                                                   \
        unsigned int loops = n/4;                                       \
        do {                                                            \
            *out++ = function(*in0++, *in1++);                          \
            *out++ = function(*in0++, *in1++);                          \
            *out++ = function(*in0++, *in1++);                          \
            *out++ = function(*in0++, *in1++);                          \
        }                                                               \
        while (--loops);                                                \
    }                                                                   \
                                                                        \
    template <typename float_type>                                      \
    inline void name##_vec(float_type * out, const float_type * in0, const float_type in1, unsigned int n) \
    {                                                                   \
        do {                                                            \
            *out++ = function(*in0++, in1);                             \
        }                                                               \
        while (--n);                                                    \
    }                                                                   \
                                                                        \
    template <typename float_type>                                      \
    inline void name##_vec_simd(float_type * out, const float_type * in0, const float_type in1, unsigned int n) \
    {                                                                   \
        unsigned int loops = n/4;                                       \
        do {                                                            \
            *out++ = function(*in0++, in1);                             \
            *out++ = function(*in0++, in1);                             \
            *out++ = function(*in0++, in1);                             \
            *out++ = function(*in0++, in1);                             \
        }                                                               \
        while (--loops);                                                \
    }                                                                   \
                                                                        \
    template <typename float_type>                                      \
    inline void name##_vec(float_type * out, const float_type in0, const float_type * in1, unsigned int n) \
    {                                                                   \
        do {                                                            \
            *out++ = function(in0, *in1++);                             \
        }                                                               \
        while (--n);                                                    \
    }                                                                   \
                                                                        \
    template <typename float_type>                                      \
    inline void name##_vec_simd(float_type * out, const float_type in0, const float_type * in1, unsigned int n) \
    {                                                                   \
        unsigned int loops = n/4;                                       \
        do {                                                            \
            *out++ = function(in0, *in1++);                             \
            *out++ = function(in0, *in1++);                             \
            *out++ = function(in0, *in1++);                             \
            *out++ = function(in0, *in1++);                             \
        }                                                               \
        while (--loops);                                                \
    }

namespace detail
{

template <typename F>
inline F spow(F in0, F in1)
{
    if (in0 > 0)
        return std::pow(in0, in1);
    else
        return -std::pow(-in0, in1);
}

} /* namespace detail */

WRAP_MATH_FUNCTION_BINARY(pow, std::pow)
WRAP_MATH_FUNCTION_BINARY(spow, detail::spow)

} /* namespace nova */

#undef WRAP_MATH_FUNCTION_BINARY


#endif /* SIMD_MATH_GENERIC_HPP */
