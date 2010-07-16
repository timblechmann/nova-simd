//  genertic math functions
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

#ifndef NOVA_SIMD_DETAIL_MATH_HPP
#define NOVA_SIMD_DETAIL_MATH_HPP

#include <algorithm>
#include <cmath>
#include <functional>


namespace nova {

namespace detail {

template <typename FloatType>
struct scalar_pointer_argument
{
    explicit scalar_pointer_argument(const FloatType * arg):
        data(arg)
    {}

    void increment(void)
    {
        data += 1;
    }

    FloatType get(void) const
    {
        return *data;
    }

    const FloatType * data;
};

template <typename FloatType>
struct scalar_scalar_argument
{
    explicit scalar_scalar_argument(FloatType const & arg):
        data(arg)
    {}

    void increment(void)
    {}

    FloatType get(void) const
    {
        return data;
    }

    FloatType data;
};

template <typename FloatType>
struct scalar_ramp_argument
{
    scalar_ramp_argument(FloatType const & base, FloatType const & slope):
        data(base), slope_(slope)
    {}

    void increment(void)
    {
        data += slope_;
    }

    FloatType get(void) const
    {
        return data;
    }

    FloatType data;
    const FloatType slope_;
};

}

inline detail::scalar_scalar_argument<float> wrap_arg_signal(float arg)
{
    return detail::scalar_scalar_argument<float>(arg);
}

inline detail::scalar_scalar_argument<double> wrap_arg_signal(double arg)
{
    return detail::scalar_scalar_argument<double>(arg);
}

inline detail::scalar_pointer_argument<float> wrap_arg_signal(const float * arg)
{
    return detail::scalar_pointer_argument<float>(arg);
}

inline detail::scalar_pointer_argument<double> wrap_arg_signal(const double * arg)
{
    return detail::scalar_pointer_argument<double>(arg);
}

template <typename FloatType>
inline detail::scalar_ramp_argument<FloatType> wrap_arg_signal(FloatType base, FloatType slope)
{
    return detail::scalar_ramp_argument<FloatType>(base, slope);
}


namespace detail {

///@{
template <typename FloatType,
          int VectorSize,
          typename Functor
         >
inline void apply_on_vector(FloatType * out, const FloatType * in, Functor f)
{
    for (int i = 0; i != VectorSize; ++i)
        out[i] = f(in[i]);
}

template <typename FloatType,
          int VectorSize,
          typename Functor
         >
inline void apply_on_vector(FloatType * out, const FloatType * in1, const FloatType * in2, Functor f)
{
    for (int i = 0; i != VectorSize; ++i)
        out[i] = f(in1[i], in2[i]);
}
///@}

///@{
template <typename FloatType,
          typename Functor
         >
inline void apply_on_vector(FloatType * out, const FloatType * in, unsigned int n, Functor f)
{
    do
        *out++ = f(*in++);
    while (--n);
}

template <typename FloatType,
          typename Arg1Type,
          typename Arg2Type,
          typename Functor
         >
inline void apply_on_vector(FloatType * out, Arg1Type in1, Arg2Type in2, unsigned int n, Functor f)
{
    do
    {
        *out++ = f(in1.get(), in2.get());
        in1.increment();
        in2.increment();
    }
    while (--n);
}


template <typename FloatType,
          typename Functor
         >
inline void apply_on_vector(FloatType * out, FloatType in1, const FloatType * in2, unsigned int n, Functor f)
{
    do
        *out++ = f(in1, *in2++);
    while (--n);
}

template <typename FloatType,
          typename Functor
         >
inline void apply_on_vector(FloatType * out, const FloatType * in1, FloatType in2, unsigned int n, Functor f)
{
    do
        *out++ = f(*in1++, in2);
    while (--n);
}


template <typename FloatType,
          typename Functor
         >
inline void apply_on_vector(FloatType * out, FloatType in1, FloatType in1_slope, const FloatType * in2, unsigned int n, Functor f)
{
    do {
        *out++ = f(in1, *in2++); in1 += in1_slope;
    } while (--n);
}

template <typename FloatType,
          typename Functor
         >
inline void apply_on_vector(FloatType * out, const FloatType * in1, FloatType in2, FloatType in2_slope, unsigned int n, Functor f)
{
    do {
        *out++ = f(*in1++, in2); in2 += in2_slope;
    } while (--n);
}
///@}


#define DEFINE_STD_UNARY_WRAPPER(NAME)          \
template<typename float_type>                   \
inline float_type NAME(float_type const & x)    \
{                                               \
    return std::NAME(x);                        \
}

DEFINE_STD_UNARY_WRAPPER(fabs)
DEFINE_STD_UNARY_WRAPPER(floor)
DEFINE_STD_UNARY_WRAPPER(ceil)

DEFINE_STD_UNARY_WRAPPER(sin)
DEFINE_STD_UNARY_WRAPPER(cos)
DEFINE_STD_UNARY_WRAPPER(tan)
DEFINE_STD_UNARY_WRAPPER(asin)
DEFINE_STD_UNARY_WRAPPER(acos)
DEFINE_STD_UNARY_WRAPPER(atan)

DEFINE_STD_UNARY_WRAPPER(tanh)

DEFINE_STD_UNARY_WRAPPER(log)
DEFINE_STD_UNARY_WRAPPER(log10)
DEFINE_STD_UNARY_WRAPPER(exp)

#define DEFINE_STD_BINARY_WRAPPER(NAME)         \
template<typename float_type>                   \
inline float_type NAME(float_type const & lhs, float_type const & rhs)  \
{                                               \
    return std::NAME(lhs, rhs);                 \
}

DEFINE_STD_BINARY_WRAPPER(pow)

template<typename float_type>
inline float_type sign(float_type const & f)
{
    if (f > 0)
        return 1;
    if (f == 0)
        return 0;
    else
        return -1;
}

template<typename float_type>
inline float_type square(float_type const & f)
{
    return f*f;
}

template<typename float_type>
inline float_type cube(float_type const & f)
{
    return f*f*f;
}


template<typename float_type>
inline float_type min(float_type const & x, float_type const & y)
{
    return std::min(x, y);
}

template<typename float_type>
inline float_type max(float_type const & x, float_type const & y)
{
    return std::max(x, y);
}

template<typename float_type>
inline float_type round(float_type const & arg)
{
    return std::floor(arg + float_type(0.5));
}

template<>
inline double round<double>(double const & arg)
{
    return ::round(arg);
}

#if _XOPEN_SOURCE >= 600 || _ISOC99_SOURCE
template<>
inline float round<float>(float const & arg)
{
    return ::roundf(arg);
}
#endif

template<typename float_type>
inline float_type frac(float_type const & arg)
{
    return arg - floor<float_type>(arg);
}

template <typename float_type>
inline float_type log2(float_type arg)
{
#if defined(__GXX_EXPERIMENTAL_CXX0X__)
    return std::log2(arg);
#else
    const float rlog2 = 1.f/std::log(2.f);
    return std::log(arg) * rlog2;
#endif
}

#if !defined(__GXX_EXPERIMENTAL_CXX0X__)

#if _XOPEN_SOURCE >= 600 || _ISOC99_SOURCE /* c99 compliant compiler */
template <>
inline float log2(float arg)
{
    return ::log2f(arg);
}

template <>
inline double log2(double arg)
{
    return ::log2(arg);
}
#endif

#endif /* __GXX_EXPERIMENTAL_CXX0X__ */

template <typename float_type>
inline float_type signed_sqrt(float_type in0)
{
    if (in0 >= 0)
        return std::sqrt(in0);
    else
        return -std::sqrt(-in0);
}

template <typename float_type>
inline float_type signed_pow(float_type in0, float_type in1)
{
    if (in0 > 0)
        return std::pow(in0, in1);
    else
        return -std::pow(-in0, in1);
}

}
}

#endif /* NOVA_SIMD_DETAIL_MATH_HPP */
