//  arithmetic simd functions for sse
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


#ifndef SIMD_MEMORY_SSE_HPP
#define SIMD_MEMORY_SSE_HPP

#include <xmmintrin.h>

#if defined(__GNUC__) && defined(NDEBUG)
#define always_inline inline  __attribute__((always_inline))
#else
#define always_inline inline
#endif

#define samples_per_loop nova::unroll_constraints<float>::samples_per_loop

namespace nova {

template <>
inline void set_exp_vec_simd(float * dest, float f, float curve, uint n)
{
    n = n / 8;

    __m128 val = _mm_setr_ps(f, f * curve, f * curve * curve,
                             f * curve * curve * curve);
    const __m128 vcurve =_mm_set_ps1(curve * curve * curve * curve);

    do
    {
        _mm_store_ps(dest, val);
        val = _mm_mul_ps(val, vcurve);

        _mm_store_ps(dest+4, val);
        val = _mm_mul_ps(val, vcurve);

        dest += 8;
    }
    while (--n);
}


namespace detail
{

template <unsigned int n>
always_inline void copyvec_simd_mp(float *dest, const float *src)
{
    __m128 data = _mm_load_ps(src);
    _mm_store_ps(dest, data);

    copyvec_simd_mp<n-4>(dest+4, src+4);
}

template <>
always_inline void copyvec_simd_mp<0>(float *dest, const float *src)
{}

} /* namespace detail */

using detail::copyvec_simd_mp;

template <unsigned int n>
void copyvec_simd(float *dest, const float *src)
{
    copyvec_simd_mp<n>(dest, src);
}

template <>
inline void copyvec_aa_simd(float * dest, const float * src, uint n)
{
    n = n / samples_per_loop;

    do
    {
        copyvec_simd_mp<samples_per_loop>(dest, src);
        dest += samples_per_loop;
        src += samples_per_loop;
    }
    while(--n);
}


template <>
inline void copyvec_aa_simd(double * dest, const double * src, uint n)
{
    n = n / samples_per_loop * 2;

    do
    {
        copyvec_simd_mp<samples_per_loop>((float*)dest, (float*)src);
        dest += samples_per_loop;
        src += samples_per_loop;
    }
    while(--n);
}

} /* namespace nova */

#undef samples_per_loop
#undef always_inline

#endif /* SIMD_MEMORY_SSE_HPP */
