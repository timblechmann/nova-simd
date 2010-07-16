//  simdfied mathematical functions
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

#ifndef SIMD_MATH_HPP
#define SIMD_MATH_HPP

#include "vec.hpp"

#include "detail/math.hpp"

#include "simd_math_generic.hpp"

#if !defined(NO_GPL3_CODE) && defined(__GNUC__)                         \
    && !( (__GNUC__ < 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ <= 2))) && defined(__SSE2__)
#include "simd_math_vec.hpp"
#else
#include "simd_math_fallback_float.hpp"
#endif

#include "simd_math_fallback_double.hpp"

namespace nova {


#if defined(__GNUC__) && defined(NDEBUG)
#define always_inline inline  __attribute__((always_inline))
#else
#define always_inline inline
#endif

#define DEFINE_MATH_FUNCTIONS(NAME, VEC_NAME)                           \
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
}                                                                       \
                                                                        \
template <typename float_type>                                          \
inline void NAME##_vec(float_type * out, const float_type * arg, unsigned int n) \
{                                                                       \
    detail::apply_on_vector(out, arg, n, detail::VEC_NAME<float_type>); \
}                                                                       \
                                                                        \
template <unsigned int n>                                               \
inline void NAME##_vec_simd(float * out, const float * in)              \
{                                                                       \
    NAME##_vec_simd(out, in, n);                                        \
}                                                                       \
                                                                        \
template <unsigned int n>                                               \
inline void NAME##_vec_simd(double * out, const double * in)            \
{                                                                       \
    NAME##_vec_simd(out, in, n);                                        \
}

DEFINE_MATH_FUNCTIONS(sin, sin)
DEFINE_MATH_FUNCTIONS(cos, cos)
DEFINE_MATH_FUNCTIONS(tan, tan)
DEFINE_MATH_FUNCTIONS(asin, asin)
DEFINE_MATH_FUNCTIONS(acos, acos)
DEFINE_MATH_FUNCTIONS(atan, atan)

DEFINE_MATH_FUNCTIONS(tanh, tanh)

DEFINE_MATH_FUNCTIONS(log, log)
DEFINE_MATH_FUNCTIONS(exp, exp)

DEFINE_MATH_FUNCTIONS(ssqrt, signed_sqrt)

}

#undef always_inline

#endif /* SIMD_MATH_HPP */
