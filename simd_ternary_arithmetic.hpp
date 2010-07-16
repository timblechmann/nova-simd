//  templated arithmetic simd functions
//  Copyright (C) 2009 Tim Blechmann <tim@klingt.org>
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

#ifndef SIMD_TERNARY_ARITHMETIC_HPP
#define SIMD_TERNARY_ARITHMETIC_HPP

#include <algorithm>

#include "vec.hpp"
#include "wrap_argument_vector.hpp"

#include "detail/unroll_helpers.hpp"

namespace nova
{
namespace detail
{

template<typename float_type>
struct clip
{
    float_type operator()(float_type value, float_type low, float_type high) const
    {
        return max_(min_(value, high),
                    low);
    }
};

template<typename float_type>
struct muladd
{
    float_type operator()(float_type value, float_type mul, float_type add) const
    {
        return value * mul + add;
    }
};

template<typename float_type>
struct ampmod
{
    float_type operator()(float_type signal, float_type modulator, float_type amount) const
    {
        return signal * (float_type(1) + modulator * amount);
    }
};

}

#define DEFINE_TERNARY_OPERATION(NAME, FUNCTOR)                         \
template <typename float_type,                                          \
          typename Arg1,                                                \
          typename Arg2,                                                \
          typename Arg3                                                 \
         >                                                              \
inline void NAME##_vec(float_type * out, Arg1 arg1, Arg2 arg2, Arg3 arg3, unsigned int n) \
{                                                                       \
    detail::apply_on_vector<float_type>(out, arg1, arg2, arg3, n, FUNCTOR<float_type>()); \
}                                                                       \
                                                                        \
template <typename float_type,                                          \
          typename Arg1,                                                \
          typename Arg2,                                                \
          typename Arg3                                                 \
         >                                                              \
inline void NAME##_vec_simd(float_type * out, Arg1 arg1, Arg2 arg2, Arg3 arg3, unsigned int n) \
{                                                                       \
    const unsigned int per_loop = vec<float_type>::objects_per_cacheline; \
    n /= per_loop;                                                      \
    do {                                                                \
        detail::compile_time_unroller<float_type, per_loop>::mp_iteration(out, arg1, arg2, arg3, \
                                                                          FUNCTOR<vec<float_type> >()); \
        out += per_loop;                                                \
    } while (--n);                                                      \
}                                                                       \
                                                                        \
template <int N,                                                        \
          typename float_type,                                          \
          typename Arg1,                                                \
          typename Arg2,                                                \
          typename Arg3                                                 \
         >                                                              \
inline void NAME##_vec_simd(float_type * out, Arg1 arg1, Arg2 arg2, Arg3 arg3) \
{                                                                       \
    detail::compile_time_unroller<float_type, N>::mp_iteration(out, arg1, arg2, arg3, \
                                                               FUNCTOR<vec<float_type> >()); \
}

DEFINE_TERNARY_OPERATION(clip, detail::clip)
DEFINE_TERNARY_OPERATION(muladd, detail::muladd)
DEFINE_TERNARY_OPERATION(ampmod, detail::ampmod)

}

#endif /* SIMD_TERNARY_ARITHMETIC_HPP */
