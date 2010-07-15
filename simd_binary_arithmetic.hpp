//  binary arithmetic simd functions
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

#ifndef SIMD_BINARY_ARITHMETIC_HPP
#define SIMD_BINARY_ARITHMETIC_HPP

#include <functional>
#include <algorithm>

#include "vec.hpp"

#if defined(__GNUC__) && defined(NDEBUG)
#define always_inline inline  __attribute__((always_inline))
#else
#define always_inline inline
#endif


namespace nova {

namespace detail
{

template<typename float_type>
struct clip2:
    public std::binary_function<float_type, float_type, float_type>
{
    float_type operator()(float_type const & f, float_type const & limit)
    {
        float_type zero(0);
        float_type neg = zero - float_type(limit);
        return max_(neg, min_(f, limit));
    }
};

template<typename float_type>
struct min_functor:
    public std::binary_function<float_type, float_type, float_type>
{
    float_type operator()(float_type const & x, float_type const & y)
    {
        return min_(x, y);
    }
};

template<typename float_type>
struct max_functor:
    public std::binary_function<float_type, float_type, float_type>
{
    float_type operator()(float_type const & x, float_type const & y)
    {
        return max_(x, y);
    }
};

template<typename float_type>
struct less:
    public std::binary_function<float_type, float_type, float_type>
{
    float_type operator()(float_type const & x, float_type const & y)
    {
        return x < y;
    }
};

template<typename float_type>
struct less_equal:
    public std::binary_function<float_type, float_type, float_type>
{
    float_type operator()(float_type const & x, float_type const & y)
    {
        return x <= y;
    }
};

template<typename float_type>
struct greater:
    public std::binary_function<float_type, float_type, float_type>
{
    float_type operator()(float_type const & x, float_type const & y)
    {
        return x > y;
    }
};

template<typename float_type>
struct greater_equal:
    public std::binary_function<float_type, float_type, float_type>
{
    float_type operator()(float_type const & x, float_type const & y)
    {
        return x >= y;
    }
};


template<typename float_type>
struct equal_to:
    public std::binary_function<float_type, float_type, float_type>
{
    float_type operator()(float_type const & x, float_type const & y)
    {
        return x == y;
    }
};

template<typename float_type>
struct not_equal_to:
    public std::binary_function<float_type, float_type, float_type>
{
    float_type operator()(float_type const & x, float_type const & y)
    {
        return x != y;
    }
};


} /* namespace detail */

#define DEFINE_NON_SIMD_FUNCTIONS(NAME, FUNCTOR)                        \
template <typename float_type>                                          \
inline void NAME##_vec(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n) \
{                                                                       \
    detail::apply_on_vector(out, arg1, arg2, n, FUNCTOR<float_type>()); \
}                                                                       \
                                                                        \
template <typename float_type>                                          \
inline void NAME##_vec(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n) \
{                                                                       \
    detail::apply_on_vector(out, arg1, arg2, n, FUNCTOR<float_type>()); \
}                                                                       \
                                                                        \
template <typename float_type>                                          \
inline void NAME##_vec(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n) \
{                                                                       \
    detail::apply_on_vector(out, arg1, arg2, n, FUNCTOR<float_type>()); \
}                                                                       \
                                                                        \
template <typename float_type>                                          \
inline void NAME##_vec(float_type * out, const float_type * arg1, float_type arg2, \
                      const float_type arg2_slope, unsigned int n)      \
{                                                                       \
    detail::apply_on_vector(out, arg1, arg2, arg2_slope, n, FUNCTOR<float_type>()); \
}                                                                       \
                                                                        \
template <typename float_type>                                          \
inline void NAME##_vec(float_type * out, float_type arg1, const float_type arg1_slope, \
                      const float_type * arg2, unsigned int n)          \
{                                                                       \
    detail::apply_on_vector(out, arg1, arg1_slope, arg2, n, FUNCTOR<float_type>()); \
}



#define DEFINE_SIMD_FUNCTIONS(NAME, FUNCTOR)                            \
namespace detail {                                                      \
                                                                        \
template <typename float_type, int n>                                   \
always_inline void NAME##_vec_simd_mp(float_type * out, const float_type * src1, const float_type * src2) \
{                                                                       \
    const unsigned int size = vec<float_type>::size;                    \
                                                                        \
    vec<float_type> in1, in2;                                           \
    in1.load_aligned(src1);                                             \
    in2.load_aligned(src2);                                             \
                                                                        \
    vec<float_type> result = FUNCTOR<vec<float_type> >()(in1, in2);           \
    result.store_aligned(out);                                          \
                                                                        \
    NAME##_vec_simd_mp<float_type, n-size>(out+size, src1+size, src2+size); \
}                                                                       \
                                                                        \
template <>                                                             \
always_inline void NAME##_vec_simd_mp<float, 0>(float * out, const float * src1, const float * src2) \
{}                                                                      \
                                                                        \
template <>                                                             \
always_inline void NAME##_vec_simd_mp<double, 0>(double * out, const double * src1, const double * src2) \
{}                                                                      \
                                                                        \
                                                                        \
template <typename float_type, int n>                                   \
always_inline void NAME##_vec_simd_mp(float_type * out, const float_type * src1, const vec<float_type> & src2) \
{                                                                       \
    const unsigned int size = vec<float_type>::size;                    \
    vec<float_type> in1;                                                \
    in1.load_aligned(src1);                                             \
                                                                        \
    vec<float_type> result = FUNCTOR<vec<float_type> >()(in1, src2);          \
    result.store_aligned(out);                                          \
                                                                        \
    NAME##_vec_simd_mp<float_type, n-size>(out+size, src1+size, src2);  \
}                                                                       \
                                                                        \
template <>                                                             \
always_inline void NAME##_vec_simd_mp<float, 0>(float * out, const float * src1, const vec<float> & src2) \
{}                                                                      \
                                                                        \
template <>                                                             \
always_inline void NAME##_vec_simd_mp<double, 0>(double * out, const double * src1, const vec<double> & src2) \
{}                                                                      \
                                                                        \
                                                                        \
template <typename float_type, int n>                                   \
always_inline void NAME##_vec_simd_mp(float_type * out, const float_type * src1, \
                                                vec<float_type> & src2, const vec<float_type> & src2_slope) \
{                                                                       \
    const unsigned int size = vec<float_type>::size;                    \
    vec<float_type> in1;                                                \
    in1.load_aligned(src1);                                             \
                                                                        \
    vec<float_type> result = FUNCTOR<vec<float_type> >()(in1, src2);          \
    result.store_aligned(out);                                          \
                                                                        \
    src2 += src2_slope;                                                 \
    NAME##_vec_simd_mp<float_type, n-size>(out+size, src1+size, src2, src2_slope); \
}                                                                       \
                                                                        \
template <>                                                             \
always_inline void NAME##_vec_simd_mp<float, 0>(float * out, const float * src1, \
                                                          vec<float> & src2, const vec<float> & src2_slope) \
{}                                                                      \
                                                                        \
template <>                                                             \
always_inline void NAME##_vec_simd_mp<double, 0>(double * out, const double * src1, \
                                                           vec<double> & src2, const vec<double> & src2_slope) \
{}                                                                      \
                                                                        \
                                                                        \
template <typename float_type, int n>                                   \
always_inline void NAME##_vec_simd_mp(float_type * out, const vec<float_type> & src1, const float_type * src2) \
{                                                                       \
    const unsigned int size = vec<float_type>::size;                    \
    vec<float_type> in2;                                                \
    in2.load_aligned(src2);                                             \
                                                                        \
    vec<float_type> result = FUNCTOR<vec<float_type> >()(src1, in2);          \
    result.store_aligned(out);                                          \
                                                                        \
    NAME##_vec_simd_mp<float_type, n-size>(out+size, src1, src2+size);  \
}                                                                       \
                                                                        \
template <>                                                             \
always_inline void NAME##_vec_simd_mp<float, 0>(float * out, const vec<float> & src1, const float * src2) \
{}                                                                      \
                                                                        \
template <>                                                             \
always_inline void NAME##_vec_simd_mp<double, 0>(double * out, const vec<double> & src1, const double * src2) \
{}                                                                      \
                                                                        \
                                                                        \
template <typename float_type, int n>                                   \
always_inline void NAME##_vec_simd_mp(float_type * out, vec<float_type> & src1, const vec<float_type> & src1_slope, \
                                                const float_type * src2) \
{                                                                       \
    const unsigned int size = vec<float_type>::size;                    \
    vec<float_type> in2;                                                \
    in2.load_aligned(src2);                                             \
                                                                        \
    vec<float_type> result = FUNCTOR<vec<float_type> >()(src1, in2);          \
    result.store_aligned(out);                                          \
                                                                        \
    src1 += src1_slope;                                                 \
    NAME##_vec_simd_mp<float_type, n-size>(out+size, src1, src1_slope, src2+size); \
}                                                                       \
                                                                        \
template <>                                                             \
always_inline void NAME##_vec_simd_mp<float, 0>(float * out, vec<float> & src1, const vec<float> & src1_slope, \
                                                          const float * src2) \
{}                                                                      \
                                                                        \
template <>                                                             \
always_inline void NAME##_vec_simd_mp<double, 0>(double * out, vec<double> & src1, const vec<double> & src1_slope, \
                                                           const double * src2) \
{}                                                                      \
                                                                        \
} /* namespace detail */                                                \
                                                                        \
                                                                        \
template <typename float_type>                                          \
inline void NAME##_vec_simd(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n) \
{                                                                       \
    const unsigned int samples_per_loop = vec<float_type>::objects_per_cacheline; \
    n /= samples_per_loop;                                              \
    do {                                                                \
        detail::NAME##_vec_simd_mp<float_type, samples_per_loop>(out, arg1, arg2); \
        arg1 += samples_per_loop; arg2 += samples_per_loop; out += samples_per_loop; \
    } while (--n);                                                      \
}                                                                       \
                                                                        \
template <typename float_type>                                          \
inline void NAME##_vec_simd(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n) \
{                                                                       \
    const unsigned int samples_per_loop = vec<float_type>::objects_per_cacheline; \
    n /= samples_per_loop;                                              \
    vec<float_type> in2(arg2);                                          \
    do {                                                                \
        detail::NAME##_vec_simd_mp<float_type, samples_per_loop>(out, arg1, in2); \
        arg1 += samples_per_loop; out += samples_per_loop;              \
    } while (--n);                                                      \
}                                                                       \
                                                                        \
template <typename float_type>                                          \
inline void NAME##_vec_simd(float_type * out, const float_type * arg1, const float_type arg2, \
                            const float_type arg2_slope, unsigned int n) \
{                                                                       \
    const unsigned int samples_per_loop = vec<float_type>::objects_per_cacheline; \
    n /= samples_per_loop;                                              \
    vec<float_type> in2;                                                \
    float_type diff = in2.set_slope(arg2, arg2_slope);                  \
    const vec<float_type> offset(diff);                                 \
    do {                                                                \
        detail::NAME##_vec_simd_mp<float_type, samples_per_loop>(out, arg1, in2, offset); \
        arg1 += samples_per_loop; out += samples_per_loop;              \
    } while (--n);                                                      \
}                                                                       \
                                                                        \
template <typename float_type>                                          \
inline void NAME##_vec_simd(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n) \
{                                                                       \
    const unsigned int samples_per_loop = vec<float_type>::objects_per_cacheline; \
    n /= samples_per_loop;                                              \
    vec<float_type> in1(arg1);                                          \
    do {                                                                \
        detail::NAME##_vec_simd_mp<float_type, samples_per_loop>(out, in1, arg2); \
        arg2 += samples_per_loop; out += samples_per_loop;              \
    } while (--n);                                                      \
}                                                                       \
                                                                        \
template <typename float_type>                                          \
inline void NAME##_vec_simd(float_type * out, const float_type arg1, const float_type arg1_slope, \
                            const float_type * arg2, unsigned int n)    \
{                                                                       \
    const unsigned int samples_per_loop = vec<float_type>::objects_per_cacheline; \
    n /= samples_per_loop;                                              \
    vec<float_type> in1;                                                \
    float_type diff = in1.set_slope(arg1, arg1_slope);                  \
    const vec<float_type> offset(diff);                                 \
    do {                                                                \
        detail::NAME##_vec_simd_mp<float_type, samples_per_loop>(out, in1, offset, arg2); \
        arg2 += samples_per_loop; out += samples_per_loop;              \
    } while (--n);                                                      \
}                                                                       \
                                                                        \
                                                                        \
template <typename float_type, unsigned int n>                          \
inline void NAME##_vec_simd(float_type * out, const float_type * arg1, const float_type * arg2) \
{                                                                       \
    detail::NAME##_vec_simd_mp<float_type, n>(out, arg1, arg2);         \
}                                                                       \
                                                                        \
template <typename float_type, unsigned int n>                          \
inline void NAME##_vec_simd(float_type * out, const float_type * arg1, const float_type arg2) \
{                                                                       \
    vec<float_type> in2(arg2);                                          \
    detail::NAME##_vec_simd_mp<float_type, n>(out, arg1, in2);          \
}                                                                       \
                                                                        \
template <typename float_type, unsigned int n>                          \
inline void NAME##_vec_simd(float_type * out, const float_type * arg1, const float_type arg2, \
                            const float_type arg2_slope)                \
{                                                                       \
    vec<float_type> in2;                                                \
    float_type diff = in2.set_slope(arg2, arg2_slope);                  \
    const vec<float_type> offset(diff);                                 \
    detail::NAME##_vec_simd_mp<float_type, n>(out, arg1, in2, offset);  \
}                                                                       \
                                                                        \
template <typename float_type, unsigned int n>                          \
inline void NAME##_vec_simd(float_type * out, const float_type arg1, const float_type * arg2) \
{                                                                       \
    vec<float_type> in1(arg1);                                          \
    detail::NAME##_vec_simd_mp<float_type, n>(out, in1, arg2);          \
}                                                                       \
                                                                        \
template <typename float_type, unsigned int n>                          \
inline void NAME##_vec_simd(float_type * out, const float_type arg1, const float_type arg1_slope, \
                            const float_type * arg2)                    \
{                                                                       \
    vec<float_type> in1;                                                \
    float_type diff = in1.set_slope(arg1, arg1_slope);                  \
    const vec<float_type> offset(diff);                                 \
    detail::NAME##_vec_simd_mp<float_type, n>(out, in1, offset, arg2);  \
}

#define DEFINE_FUNCTIONS(NAME, FUNCTOR)         \
    DEFINE_NON_SIMD_FUNCTIONS(NAME, FUNCTOR)    \
    DEFINE_SIMD_FUNCTIONS(NAME, FUNCTOR)

DEFINE_FUNCTIONS(plus, std::plus)
DEFINE_FUNCTIONS(minus, std::minus)
DEFINE_FUNCTIONS(times, std::multiplies)
DEFINE_FUNCTIONS(over, std::divides)

DEFINE_FUNCTIONS(min, detail::min_functor)
DEFINE_FUNCTIONS(max, detail::max_functor)
DEFINE_FUNCTIONS(less, detail::less)
DEFINE_FUNCTIONS(less_equal, detail::less_equal)
DEFINE_FUNCTIONS(greater, detail::greater)
DEFINE_FUNCTIONS(greater_equal, detail::greater_equal)
DEFINE_FUNCTIONS(equal, detail::equal_to)
DEFINE_FUNCTIONS(notequal, detail::not_equal_to)

DEFINE_FUNCTIONS(clip2, detail::clip2)

} /* namespace nova */

#undef always_inline


#endif /* SIMD_BINARY_ARITHMETIC_HPP */
