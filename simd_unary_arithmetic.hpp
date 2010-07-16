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

#include "wrap_argument_vector.hpp"
#include "detail/unroll_helpers.hpp"

#if defined(__GNUC__) && defined(NDEBUG)
#define always_inline inline  __attribute__((always_inline))
#else
#define always_inline inline
#endif

namespace nova
{

namespace detail
{

template <typename FloatType>
struct fabs_
{
    always_inline FloatType operator()(FloatType arg) const
    {
        return fabs<FloatType>(arg);
    }

    always_inline vec<FloatType> operator()(vec<FloatType> const & arg) const
    {
        return abs(arg);
    }
};

template <typename FloatType>
struct sign_
{
    always_inline FloatType operator()(FloatType arg) const
    {
        return sign<FloatType>(arg);
    }

    always_inline vec<FloatType> operator()(vec<FloatType> const & arg) const
    {
        return sign(arg);
    }
};

template <typename FloatType>
struct square_
{
    always_inline FloatType operator()(FloatType arg) const
    {
        return square<FloatType>(arg);
    }

    always_inline vec<FloatType> operator()(vec<FloatType> const & arg) const
    {
        return square(arg);
    }
};

template <typename FloatType>
struct cube_
{
    always_inline FloatType operator()(FloatType arg) const
    {
        return cube<FloatType>(arg);
    }

    always_inline vec<FloatType> operator()(vec<FloatType> const & arg) const
    {
        return cube(arg);
    }
};

} /* namespace detail */

#define DEFINE_UNARY_FUNCTIONS(NAME, FUNCTOR)                           \
                                                                        \
template <typename float_type>                                          \
inline void NAME##_vec(float_type * out, const float_type * arg, unsigned int n) \
{                                                                       \
    detail::apply_on_vector(out, arg, n, FUNCTOR<float_type>());        \
}                                                                       \
                                                                        \
template <typename FloatType>                                           \
inline void NAME##_vec_simd(FloatType * out, const FloatType * arg, unsigned int n) \
{                                                                       \
    detail::generate_simd_loop(out, wrap_arg_vector(arg), n, FUNCTOR<FloatType>()); \
}                                                                       \
                                                                        \
template <unsigned int n, typename FloatType>                           \
inline void NAME##_vec_simd(FloatType * out, const FloatType * arg)     \
{                                                                       \
    detail::vector_pointer_argument<FloatType> varg(arg); \
    detail::compile_time_unroller<FloatType, n>::mp_iteration(out, varg, FUNCTOR<FloatType>()); \
}

DEFINE_UNARY_FUNCTIONS(abs, detail::fabs_)
DEFINE_UNARY_FUNCTIONS(sgn, detail::sign_)
DEFINE_UNARY_FUNCTIONS(square, detail::square_)
DEFINE_UNARY_FUNCTIONS(cube, detail::cube_)

} /* namespace nova */

#undef always_inline

#endif /* SIMD_UNARY_ARITHMETIC_HPP */
