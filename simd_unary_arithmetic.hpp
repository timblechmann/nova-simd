//  unary arithmetic simd functions
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

#ifndef SIMD_UNARY_ARITHMETIC_HPP
#define SIMD_UNARY_ARITHMETIC_HPP

#include "vec.hpp"

#if defined(__GNUC__) && defined(NDEBUG)
#define always_inline inline  __attribute__((always_inline))
#else
#define always_inline inline
#endif

namespace nova
{

namespace detail {

template <typename float_type>
inline float_type sgn(float_type arg)
{
    if (arg > 0)
        return float_type(1);
    if (arg == 0)
        return float_type(0);
    else
        return float_type(-1);
}

template <typename float_type>
inline float_type square(float_type arg)
{
    return arg * arg;
}

template <typename float_type>
inline float_type cube(float_type arg)
{
    return arg * arg * arg;
}

} /* namespace detail */


template <typename float_type>
inline void abs_vec(float_type * out, const float_type * arg, unsigned int n)
{
    do {
        *out++ = std::fabs(*arg++);
    } while (--n);
}

template <typename float_type>
inline void sgn_vec(float_type * out, const float_type * arg, unsigned int n)
{
    do {
        *out++ = detail::sgn<float_type>(*arg++);
    } while (--n);
}

template <typename float_type>
inline void square_vec(float_type * out, const float_type * arg, unsigned int n)
{
    do {
        *out++ = detail::square<float_type>(*arg++);
    } while (--n);
}

template <typename float_type>
inline void cube_vec(float_type * out, const float_type * arg, unsigned int n)
{
    do {
        *out++ = detail::cube<float_type>(*arg++);
    } while (--n);
}

#define DEFINE_UNARY_FUNCTIONS(NAME, VEC_NAME)                          \
                                                                        \
namespace detail                                                        \
{                                                                       \
template <typename F, unsigned int n>                                   \
struct NAME##_arithmetic                                                \
{                                                                       \
    static const int offset = vec<F>::size;                             \
                                                                        \
    static always_inline void mp_iteration(F * out, const F * in)       \
    {                                                                   \
        vec<F> arg;                                                     \
        arg.load_aligned(in);                                           \
        arg = VEC_NAME(arg);                                            \
        arg.store_aligned(out);                                         \
        NAME##_arithmetic<F, n-offset>::mp_iteration(out+offset, in+offset); \
    }                                                                   \
};                                                                      \
                                                                        \
template <typename F>                                                   \
struct NAME##_arithmetic<F, 0>                                          \
{                                                                       \
    static always_inline void mp_iteration(F * out, const F * in)       \
    {}                                                                  \
};                                                                      \
} /* namespace detail */                                                \
                                                                        \
template <typename F>                                                   \
inline void NAME##_vec_simd(F * out, const F * arg, unsigned int n)     \
{                                                                       \
    const int per_loop = vec<F>::objects_per_cacheline;                 \
    unsigned int unroll = n / per_loop;                                 \
    do {                                                                \
        detail::NAME##_arithmetic<F, per_loop>::mp_iteration(out, arg); \
        out += per_loop; arg += per_loop;                               \
    }                                                                   \
    while (--unroll);                                                   \
}                                                                       \
                                                                        \
template <typename F, unsigned int n>                                   \
inline void NAME##_vec_simd(F * out, const F * arg)                     \
{                                                                       \
    detail::NAME##_arithmetic<F, n>::mp_iteration(out, arg);            \
}

DEFINE_UNARY_FUNCTIONS(abs, abs)
DEFINE_UNARY_FUNCTIONS(sgn, sign)
DEFINE_UNARY_FUNCTIONS(square, square)
DEFINE_UNARY_FUNCTIONS(cube, cube)

} /* namespace nova */

#undef always_inline

#endif /* SIMD_UNARY_ARITHMETIC_HPP */
