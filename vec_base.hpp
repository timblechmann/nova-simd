//  vector base class
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

#ifndef VEC_BASE_HPP
#define VEC_BASE_HPP

#include <cassert>
#include <numeric>

#include "detail/math.hpp"

#if defined(__GNUC__) && defined(NDEBUG)
#define always_inline inline  __attribute__((always_inline))
#else
#define always_inline inline
#endif


namespace nova {


/* vector base class
 *
 * requirements:
 * - WrappedType is the wrapped scalar type
 * - get_pointer(VecType) should return WrappedType*
 * - VecSize should be the number of WrappedType elements inside a VecType
 *
 */
template <typename WrappedType,
          typename VecType,
          int VecSize>
class vec_base
{
public:
    static const int size = VecSize;

protected:
    vec_base (void)
    {}

    vec_base (VecType arg):
        data_(arg)
    {}

    operator VecType (void) const
    {
        return data_;
    }

public:
    WrappedType get (int index) const
    {
        assert(index < size);
        const WrappedType * data = get_pointer(data_);
        return data[index];
    }

    void set (int index, WrappedType arg)
    {
        assert(index < size);
        WrappedType * data = get_pointer(data_);
        data[index] = arg;
    }

private:
    template <typename Functor>
    static always_inline VecType apply_unary(VecType const & arg, Functor & f)
    {
        VecType ret;
        const WrappedType * arg_data = get_pointer(arg);
        WrappedType * ret_data = get_pointer(ret);
        for (int i = 0; i != VecSize; ++i)
            ret_data[i] = f(arg_data[i]);
        return ret;
    }

    template <typename Functor>
    static always_inline VecType apply_binary(VecType const & arg1, VecType const & arg2, Functor & f)
    {
        VecType ret;
        const WrappedType * arg1_data = get_pointer(arg1);
        const WrappedType * arg2_data = get_pointer(arg2);
        WrappedType * ret_data = get_pointer(ret);
        for (int i = 0; i != VecSize; ++i)
            ret_data[i] = f(arg1_data[i], arg2_data[i]);
        return ret;
    }

#define DEFINE_UNARY_STATIC(NAME, METHOD)                   \
    static always_inline VecType NAME(VecType const & arg)  \
    {                                                       \
       return apply_unary(arg, METHOD<WrappedType>);        \
    }

#define DEFINE_BINARY_STATIC(NAME, METHOD)                                          \
    static always_inline VecType NAME(VecType const & arg1, VecType const & arg2)   \
    {                                                                               \
       return apply_binary(arg1, arg2, METHOD<WrappedType>);                        \
    }


protected:
    DEFINE_UNARY_STATIC(sin, detail::sin)
    DEFINE_UNARY_STATIC(cos, detail::cos)
    DEFINE_UNARY_STATIC(tan, detail::tan)

    DEFINE_UNARY_STATIC(asin, detail::asin)
    DEFINE_UNARY_STATIC(acos, detail::acos)
    DEFINE_UNARY_STATIC(atan, detail::atan)

    DEFINE_UNARY_STATIC(tanh, detail::tanh)

    DEFINE_UNARY_STATIC(log, detail::log)
    DEFINE_UNARY_STATIC(log2, detail::log2)
    DEFINE_UNARY_STATIC(log10, detail::log10)
    DEFINE_UNARY_STATIC(exp, detail::exp)
    DEFINE_UNARY_STATIC(signed_sqrt, detail::signed_sqrt)

    DEFINE_UNARY_STATIC(round, detail::round)
    DEFINE_UNARY_STATIC(ceil, detail::ceil)
    DEFINE_UNARY_STATIC(floor, detail::floor)
    DEFINE_UNARY_STATIC(frac, detail::frac)

    DEFINE_BINARY_STATIC(pow, detail::pow)
    DEFINE_BINARY_STATIC(signed_pow, detail::signed_pow)

    WrappedType horizontal_min(void) const
    {
        const WrappedType * data = get_pointer(data_);
        return *std::min_element(data, data + size);
    }

    WrappedType horizontal_max(void) const
    {
        const WrappedType * data = get_pointer(data_);
        return *std::max_element(data, data + size);
    }

    WrappedType horizontal_sum(void) const
    {
        const WrappedType * data = get_pointer(data_);
        return std::accumulate(data, data + size, 0);
    }

protected:
    VecType data_;
};

}

#define NOVA_SIMD_DELEGATE_UNARY_TO_BASE(NAME)  \
    inline friend vec NAME(vec const & arg)     \
    {                                           \
        return base::NAME(arg.data_);           \
    }

#define NOVA_SIMD_DELEGATE_BINARY_TO_BASE(NAME)                 \
    inline friend vec NAME(vec const & arg1, vec const & arg2)  \
    {                                                           \
        return base::NAME(arg1.data_, arg2.data_);              \
    }

#undef always_inline


#endif /* VEC_BASE_HPP */
