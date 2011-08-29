//  horizontal simd functions
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


#ifndef SIMD_HORIZONTAL_FUNCTIONS_HPP
#define SIMD_HORIZONTAL_FUNCTIONS_HPP

#include "vec.hpp"

#include <algorithm>            /* for max */

#if defined(__GNUC__) && defined(NDEBUG)
#define always_inline inline  __attribute__((always_inline))
#else
#define always_inline inline
#endif

namespace nova {

/* horizontal max */
template <typename F>
inline F horizontal_max_vec(const F * in, unsigned int n)
{
    F current = std::numeric_limits<F>::min();
    using namespace std;

    do {
        current = max(current, *in++);
    } while(--n);

    return current;
}

template <typename F>
inline F horizontal_max_vec_simd(const F * in, unsigned int n)
{
    F init = std::numeric_limits<F>::min();
    vec<F> current(init);

    /* loop */
    const size_t vec_size = vec<F>::size;
    n /= vec_size;
    do {
        vec<F> val;
        val.load_aligned(in);

        current = max_(current, val);
        in += vec_size;
    } while(--n);

    return current.horizontal_max();
}

template <typename F>
inline F horizontal_min_vec(const F * in, unsigned int n)
{
    F current = std::numeric_limits<F>::min();
    using namespace std;

    do {
        current = min(current, *in++);
    } while(--n);

    return current;
}

template <typename F>
inline F horizontal_min_vec_simd(const F * in, unsigned int n)
{
    F init = std::numeric_limits<F>::min();
    vec<F> current(init);

    /* loop */
    const size_t vec_size = vec<F>::size;
    n /= vec_size;
    do {
        vec<F> val;
        val.load_aligned(in);

        current = min_(current, val);
        in += vec_size;
    } while(--n);

    return current.horizontal_min();
}

template <typename F>
inline F horizontal_sum_vec(const F * in, unsigned int n)
{
    F current = 0;
    using namespace std;

    do {
        current = current + *in++;
    } while(--n);

    return current;
}

template <typename F>
inline F horizontal_sum_vec_simd(const F * in, unsigned int n)
{
    vec<F> current(0);

    /* loop */
    const size_t vec_size = vec<F>::size;
    n /= vec_size;
    do {
        vec<F> val;
        val.load_aligned(in);

        current = current + val;
        in += vec_size;
    } while(--n);

    return current.horizontal_sum();
}



} /* namespace nova */

#undef always_inline

#endif /* SIMD_PEAKMETER_HPP */
