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
#include "wrap_arguments.hpp"
#include "wrap_argument_vector.hpp"

#include "detail/math.hpp"
#include "detail/unroll_helpers.hpp"

namespace nova {

#if defined(__GNUC__) && defined(NDEBUG)
#define always_inline inline  __attribute__((always_inline))
#else
#define always_inline inline
#endif

#define DEFINE_MATH_FUNCTIONS_UNARY(NAME, VEC_NAME)                     \
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

DEFINE_MATH_FUNCTIONS_UNARY(sin, sin)
DEFINE_MATH_FUNCTIONS_UNARY(cos, cos)
DEFINE_MATH_FUNCTIONS_UNARY(tan, tan)
DEFINE_MATH_FUNCTIONS_UNARY(asin, asin)
DEFINE_MATH_FUNCTIONS_UNARY(acos, acos)
DEFINE_MATH_FUNCTIONS_UNARY(atan, atan)

DEFINE_MATH_FUNCTIONS_UNARY(tanh, tanh)

DEFINE_MATH_FUNCTIONS_UNARY(log, log)
DEFINE_MATH_FUNCTIONS_UNARY(exp, exp)

DEFINE_MATH_FUNCTIONS_UNARY(ssqrt, signed_sqrt)


#define DEFINE_MATH_FUNCTIONS_BINARY(NAME, VEC_NAME)                    \
NOVA_SIMD_DEFINE_UNROLLER(NAME, VEC_NAME)                               \
                                                                        \
                                                                        \
template <typename F, typename Arg1Type, typename Arg2Type>             \
inline void NAME##_vec_simd_(F * out, Arg1Type arg1, Arg2Type arg2, unsigned int n) \
{                                                                       \
    const int per_loop = vec<F>::objects_per_cacheline;                 \
    unsigned int unroll = n / per_loop;                                 \
    do {                                                                \
        detail::NAME##_unroller<F, per_loop>::mp_iteration(out, arg1, arg2); \
        out += per_loop;                                                \
    }                                                                   \
    while (--unroll);                                                   \
}                                                                       \
                                                                        \
template <typename F, typename Arg1Type, typename Arg2Type>             \
inline void NAME##_vec_simd(F * out, Arg1Type arg1, Arg2Type arg2, unsigned int n) \
{                                                                       \
    NAME##_vec_simd_(out, wrap_arg_vector(arg1), wrap_arg_vector(arg2), n); \
}                                                                       \
                                                                        \
                                                                        \
template <typename float_type, typename arg1_type, typename arg2_type>  \
inline void NAME##_vec(float_type * out, arg1_type arg1, arg2_type arg2, unsigned int n) \
{                                                                       \
    detail::apply_on_vector(out, wrap_arg_signal(arg1), wrap_arg_signal(arg2), n, detail::VEC_NAME<float_type>); \
}                                                                       \
                                                                        \
template <unsigned int n, typename FloatType, typename Arg1Type, typename Arg2Type> \
inline void NAME##_vec_simd(FloatType * out, Arg1Type in1, Arg1Type in2) \
{                                                                       \
    NAME##_vec_simd_(out, wrap_arg_vector(in1), wrap_arg_vector(in2), n); \
}

DEFINE_MATH_FUNCTIONS_BINARY(pow, pow)
DEFINE_MATH_FUNCTIONS_BINARY(spow, signed_pow)
}

#undef DEFINE_MATH_FUNCTIONS_UNARY
#undef DEFINE_MATH_FUNCTIONS_BINARY
#undef always_inline


#endif /* SIMD_MATH_HPP */
