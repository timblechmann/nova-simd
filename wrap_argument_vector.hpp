//  vector signal argument wrappers
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

#ifndef NOVA_SIMD_WRAP_ARGUMENT_VECTOR_HPP
#define NOVA_SIMD_WRAP_ARGUMENT_VECTOR_HPP

#include "vec.hpp"

namespace nova {
namespace detail {

template <typename FloatType>
struct vector_pointer_argument
{
    explicit vector_pointer_argument(const FloatType * arg):
        data(arg)
    {}

    void increment(void)
    {
        data += vec<FloatType>::size;
    }

    vec<FloatType> get(void) const
    {
        vec<FloatType> ret;
        ret.load_aligned(data);
        return ret;
    }

    const FloatType * data;
};

template <typename FloatType>
struct vector_scalar_argument
{
    explicit vector_scalar_argument(FloatType const & arg):
        data(arg)
    {}

    void increment(void)
    {}

    vec<FloatType> get(void) const
    {
        return vec<FloatType>(data);
    }

    FloatType data;
};

template <typename FloatType>
struct vector_ramp_argument
{
    vector_ramp_argument(FloatType const & base, FloatType const & slope):
        slope_(vec<FloatType>::size * slope)
    {
        data.set_slope(base, slope);
    }

    void increment(void)
    {
        data += slope_;
    }

    vec<FloatType> get(void) const
    {
        return data;
    }

    vec<FloatType> data;
    const FloatType slope_;
};

}

inline detail::vector_scalar_argument<float> wrap_arg_vector(float arg)
{
    return detail::vector_scalar_argument<float>(arg);
}

inline detail::vector_scalar_argument<double> wrap_arg_vector(double arg)
{
    return detail::vector_scalar_argument<double>(arg);
}

inline detail::vector_pointer_argument<float> wrap_arg_vector(const float * arg)
{
    return detail::vector_pointer_argument<float>(arg);
}

inline detail::vector_pointer_argument<double> wrap_arg_vector(const double * arg)
{
    return detail::vector_pointer_argument<double>(arg);
}

template <typename FloatType>
inline detail::vector_ramp_argument<FloatType> wrap_arg_vector(FloatType base, FloatType slope)
{
    return detail::vector_ramp_argument<FloatType>(base, slope);
}

}

#endif /* NOVA_SIMD_WRAP_ARGUMENT_VECTOR_HPP */
