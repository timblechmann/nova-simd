//  templated memory simd functions
//  Copyright (C) 2008, 2009 Tim Blechmann <tim@klingt.org>
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


#ifndef SIMD_MEMORY_GENERIC_HPP
#define SIMD_MEMORY_GENERIC_HPP

#include <cassert>
#include <cstring>

namespace nova {

template <typename F>
inline void zerovec(F * dest, unsigned int n)
{
    std::memset(dest, 0, n*sizeof(F));
}

template <typename F>
inline void zerovec_simd(F * dest, unsigned int n)
{
    std::memset(dest, 0, n*sizeof(F));
}

template <typename F>
inline void zerovec_na_simd(F * dest, unsigned int n)
{
    std::memset(dest, 0, n*sizeof(F));
}

template <typename F>
inline void setvec(F * dest, F f, uint n)
{
    assert(n);
    do
        *dest++ = f;
    while (--n);
}

template <typename F>
inline void setvec_simd(F * dest, F f, uint n)
{
    for (uint i = 0; i != n; i+=8)
    {
        dest[i+0] = dest[i+1] = dest[i+2] = dest[i+3] = dest[i+4] =
            dest[i+5] = dest[i+6] = dest[i+7] = f;
    }
}


template <typename F>
inline void copyvec(F * dest, const F * src, uint n)
{
    std::memcpy(dest, src, n*sizeof(F));
}

template <typename F>
inline void copyvec_aa_simd(F * dest, const F * src, uint n)
{
    std::memcpy(dest, src, n*sizeof(F));
}

template <typename F>
inline void copyvec_simd(F * dest, const F * src, uint n)
{
    copyvec_aa_simd(dest, src, n);
}

template <typename F>
inline void copyvec_nn_simd(F * dest, const F * src, uint n)
{
    std::memcpy(dest, src, n*sizeof(F));
}

template <typename F>
inline void copyvec_an_simd(F * dest, const F * src, uint n)
{
    std::memcpy(dest, src, n*sizeof(F));
}

template <typename F>
inline void copyvec_na_simd(F * dest, const F * src, uint n)
{
    std::memcpy(dest, src, n*sizeof(F));
}

} /* namespace nova */

#endif /* SIMD_MEMORY_GENERIC_HPP */
