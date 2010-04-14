//  simdfied round functions
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

#ifndef SIMD_ROUND_HPP
#define SIMD_ROUND_HPP

#if defined(__GNUC__) && defined(NDEBUG)
#define always_inline inline  __attribute__((always_inline))
#else
#define always_inline inline
#endif

#include "vec.hpp"

#include <cmath>

namespace nova
{

#define DEFINE_ROUND_FUNCTION(NAME, FUNCTION, VECFUNCTION)              \
template <typename float_type>                                          \
inline void NAME##_vec(float_type * out, const float_type * arg, unsigned int n) \
{                                                                       \
    do                                                                  \
        *out++ = FUNCTION(*arg++);                                      \
    while (--n);                                                        \
}                                                                       \
                                                                        \
namespace detail                                                        \
{                                                                       \
                                                                        \
template <typename float_type, int n>                                   \
inline void NAME##_vec_simd_mp(float_type * out, const float_type * src) \
{                                                                       \
    const unsigned int size = vec<float_type>::size;                    \
                                                                        \
    vec<float_type> in;                                                 \
    in.load_aligned(src);                                               \
                                                                        \
    vec<float_type> result = VECFUNCTION(in);                           \
    result.store_aligned(out);                                          \
                                                                        \
    NAME##_vec_simd_mp<float_type, n-size>(out+size, src+size);         \
}                                                                       \
                                                                        \
template <>                                                             \
inline void NAME##_vec_simd_mp<float, 0>(float * out, const float * src) \
{}                                                                      \
                                                                        \
template <>                                                             \
inline void NAME##_vec_simd_mp<double, 0>(double * out, const double * src) \
{}                                                                      \
                                                                        \
} /* namespace detail */                                                \
                                                                        \
template <typename float_type>                                          \
inline void NAME##_vec_simd(float_type * out, const float_type * arg, unsigned int n) \
{                                                                       \
    const unsigned int unroll = vec<float_type>::objects_per_cacheline; \
    unsigned int loops = n / unroll;                                    \
    do {                                                                \
        detail::NAME##_vec_simd_mp<float_type, unroll>(out, arg);       \
        out += unroll; arg += unroll;                                   \
    }                                                                   \
    while (--loops);                                                    \
}                                                                       \
                                                                        \
template <typename float_type, unsigned int n>                          \
inline void NAME##_vec_simd(float_type * out, const float_type * arg)   \
{                                                                       \
    detail::NAME##_vec_simd_mp<float_type, n>(out, arg);                \
}


DEFINE_ROUND_FUNCTION(round, helper::round<float_type>, round)
DEFINE_ROUND_FUNCTION(frac, helper::frac<float_type>, frac)
DEFINE_ROUND_FUNCTION(ceil, std::ceil, ceil)
DEFINE_ROUND_FUNCTION(floor, std::floor, floor)

#undef always_inline

} /* namespace nova */

#endif /* SIMD_ROUND_HPP */
