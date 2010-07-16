//  simdfied mathematical functions
//  Copyright (C) 2008, 2009 Tim Blechmann
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

#ifndef SIMD_MATH_VEC_HPP
#define SIMD_MATH_VEC_HPP

#include <cmath>

#include "libsimdmath/lib/powf4.h"

#ifdef __SSE__
#include "simd_utils.hpp"
#endif

#define vec_xor(a,b)           ((a)^(b))

namespace nova {

#define WRAP_MATH_FUNCTION_BINARY(name, function)                       \
    template <>                                                         \
    inline void name##_vec_simd<float>(float * out, const float * in0, const float * in1, unsigned int n) \
    {                                                                   \
        unsigned int loops = n/4;                                       \
        do {                                                            \
            function(out, in0, in1);                                    \
            in0 += 4;                                                   \
            in1 += 4;                                                   \
            out += 4;                                                   \
        } while (--loops);                                              \
    }                                                                   \
                                                                        \
    template <>                                                         \
    inline void name##_vec_simd<float>(float * out, const float * in0, const float in1, unsigned int n) \
    {                                                                   \
        unsigned int loops = n/4;                                       \
        do {                                                            \
            function(out, in0, in1);                                    \
            in0 += 4;                                                   \
            out += 4;                                                   \
        } while (--loops);                                              \
    }                                                                   \
                                                                        \
    template <>                                                         \
    inline void name##_vec_simd<float>(float * out, const float in0, const float * in1, unsigned int n) \
    {                                                                   \
        unsigned int loops = n/4;                                       \
        do {                                                            \
            function(out, in0, in1);                                    \
            in1 += 4;                                                   \
            out += 4;                                                   \
        } while (--loops);                                              \
    }


namespace detail
{

/** pow */
/* @{ */
inline void pow4(float * out, const float * in1, const float * in2)
{
    const vec_float4 * ip1 = (const vec_float4*)in1;
    const vec_float4 * ip2 = (const vec_float4*)in2;
    vec_float4 * op = (vec_float4*)out;
    *op = _powf4(*ip1, *ip2);
}

inline void pow4(float * out, const float * in1, const float in2)
{
    const vec_float4 ip2 = {in2, in2, in2, in2};
    pow4(out, in1, (float*)&ip2);
}

inline void pow4(float * out, const float in1, const float * in2)
{
    const vec_float4 ip1 = {in1, in1, in1, in1};
    pow4(out, (float*)&ip1, in2);
}
/* @} */


/** signed pow:
 *
 *  if (in1 > 0)
 *      return std::pow(in1, in2);
 *  else
 *      return -std::pow(-in1, in2);
 *
 * */
/* @{ */
inline void spow4(float * out, const float * in1, const float * in2)
{
    const vec_float4 * ip1 = (const vec_float4*)in1;
    const vec_float4 * ip2 = (const vec_float4*)in2;

    const vec_int4   sign_in1 = _signf4(*ip1);
    const vec_float4 abs_in1 = (vec_float4)vec_xor((vec_int4)*ip1, sign_in1);

    vec_float4 * op = (vec_float4*)out;

    vec_float4 result = _powf4(abs_in1, *ip2);

    *op = (vec_float4)(sign_in1 | (vec_int4)result);
}

inline void spow4(float * out, const float * in1, const float in2)
{
    const vec_float4 ip2 = {in2, in2, in2, in2};
    spow4(out, in1, (float*)&ip2);
}

inline void spow4(float * out, const float in1, const float * in2)
{
    const vec_float4 ip1 = {in1, in1, in1, in1};
    spow4(out, (float*)&ip1, in2);
}
/* @} */

} /* namespace detail */

WRAP_MATH_FUNCTION_BINARY(pow, detail::pow4)
WRAP_MATH_FUNCTION_BINARY(spow, detail::spow4)


} /* namespace nova */


#undef WRAP_MATH_FUNCTION_BINARY

#endif /* SIMD_MATH_VEC_HPP */
