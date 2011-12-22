//  simd functions for mixing
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


#ifndef SIMD_MIX_HPP
#define SIMD_MIX_HPP

#include "vec.hpp"
#include "detail/wrap_argument_vector.hpp"

namespace nova
{

#if defined(__GNUC__) && defined(NDEBUG)
#define always_inline inline  __attribute__((always_inline))
#else
#define always_inline inline
#endif

namespace detail
{

template <typename F, unsigned int n>
struct mix_vec_simd
{
    static const int offset = vec<F>::size;

    template <typename InType0, typename Factor0Type, typename InType1, typename Factor1Type>
    static always_inline void mp_iteration(F * out, InType0 & in0, Factor0Type & factor0,
                                           InType1 & in1, Factor1Type & factor1)
    {
        vec<F> vout = in0.consume() * factor0.consume() + in1.consume() * factor1.consume();
        vout.store_aligned(out);

        mix_vec_simd<F, n-offset>::mp_iteration(out+offset, in0, factor0, in1, factor1);
    }
};

template <typename F>
struct mix_vec_simd<F, 0>
{
    template <typename InType0, typename Factor0Type, typename InType1, typename Factor1Type>
    static always_inline void mp_iteration(F * out, InType0 & in0, Factor0Type & factor0,
                                           InType1 & in1, Factor1Type & factor1)
    {}
};

template <typename FloatType, typename InType0, typename Factor0Type,
          typename InType1, typename Factor1Type>
void mix_vec_simd_impl(FloatType * out, InType0 in0, Factor0Type factor0,
                  InType1 in1, Factor1Type factor1, unsigned int n)
{
    const int per_loop = vec<FloatType>::objects_per_cacheline;

    n /= per_loop;
    do {
        detail::mix_vec_simd<FloatType, per_loop>::mp_iteration(out, in0, factor0, in1, factor1);
        out += per_loop;
    } while(--n);
}

template <typename FloatType, typename InType0, typename Factor0Type,
typename InType1, typename Factor1Type>
void mix_vec_impl(FloatType * out, InType0 in0, Factor0Type factor0,
                       InType1 in1, Factor1Type factor1, unsigned int n)
{
    do {
        *out++ = in0.consume() * factor0.consume() + in1.consume() * factor1.consume();
    } while(--n);
}


} // namespace detail

template <typename F, typename Factor0Type, typename Factor1Type>
void mix_vec(F * out, const F * in0, Factor0Type factor0, const F * in1, Factor1Type factor1, unsigned int n)
{
    detail::mix_vec_impl(out, wrap_argument(in0), wrap_argument(factor0),
                              wrap_argument(in1), wrap_argument(factor1), n);
}

template <typename FloatType, typename Factor0Type, typename Factor1Type>
void mix_vec_simd(FloatType * out, FloatType const * in0, Factor0Type factor0,
                  FloatType const * in1, Factor1Type factor1, unsigned int n)
{
    using detail::wrap_vector_arg;
    detail::mix_vec_simd_impl(out, wrap_vector_arg(wrap_argument(in0)), wrap_vector_arg(wrap_argument(factor0)),
                              wrap_vector_arg(wrap_argument(in1)), wrap_vector_arg(wrap_argument(factor1)), n);
}

template <unsigned int n, typename F, typename Factor0Type, typename Factor1Type>
void mix_vec_simd(F * out, const F * in0, Factor0Type factor0, const F * in1, Factor1Type factor1)
{
    mix_vec_simd(out, in0, factor0, in1, factor1, n);
}

}

#undef always_inline

#endif /* SIMD_MIX_HPP */
