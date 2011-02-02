//  get pointer functions
//  need to be declared before they are used in a tempate definition
//
//  Copyright (C) 2011 Tim Blechmann
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

#ifndef DETAIL_GET_POINTER_HPP
#define DETAIL_GET_POINTER_HPP


#ifdef __AVX__
#include "immintrin.h"

inline float * get_pointer(__m256 & arg)
{
    return (float *)&arg;
}

inline const float * get_pointer(__m256 const & arg)
{
    return (const float *)&arg;
}

inline double * get_pointer(__m256d & arg)
{
    return (double *)&arg;
}

inline const double * get_pointer(__m256d const & arg)
{
    return (const double *)&arg;
}
#endif

#ifdef __SSE__
#include <xmmintrin.h>

inline float * get_pointer(__m128 & arg)
{
    return (float *)&arg;
}

inline const float * get_pointer(__m128 const & arg)
{
    return (const float *)&arg;
}

#endif

#ifdef __SSE2__
#include <emmintrin.h>

inline double * get_pointer(__m128d & arg)
{
    return (double *)&arg;
}

inline const double * get_pointer(__m128d const & arg)
{
    return (const double *)&arg;
}

#endif

#ifdef __ALTIVEC__
#include <altivec.h>

inline float * get_pointer(vector float & arg)
{
    return (float *)&arg;
}

inline const float * get_pointer(vector float const & arg)
{
    return (const float *)&arg;
}

#endif

#ifdef __ARM_NEON__
#include <arm_neon.h>

inline float * get_pointer(float32x4_t & arg)
{
    return (float *)&arg;
}

inline const float * get_pointer(float32x4_t const & arg)
{
    return (const float *)&arg;
}

#endif


#endif /* DETAIL_GET_POINTER_HPP */
