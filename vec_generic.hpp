//  generic vector class
//
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

#ifndef VEC_GENERIC_HPP
#define VEC_GENERIC_HPP

#include <cmath>
#include <algorithm>

namespace nova
{

template <typename float_type>
struct vec
{
public:
    static const int size = 4;
    static const int objects_per_cacheline = 64/sizeof(float_type);

    /* @{ */
    /** constructors */
    vec(void)
    {}

    explicit vec(float_type f)
    {
        set_vec(f);
    }

    vec(vec const & rhs)
    {
        for (int i = 0; i != size; ++i)
            data_[i] = rhs.data_[i];
    }
    /* @} */

    /* @{ */
    /** io */
    void load(const float_type * data)
    {
        for (int i = 0; i != size; ++i)
            data_[i] = data[i];
    }

    void load_first(const float_type * data)
    {
        data_[0] = *data;

        for (int i = 1; i != size; ++i)
            data_[i] = 0;
    }

    void load_aligned(const float_type * data)
    {
        load(data);
    }

    void store(float_type * dest) const
    {
        for (int i = 0; i != size; ++i)
            dest[i] = data_[i];
    }

    void store_aligned(float_type * dest) const
    {
        store(dest);
    }

    void store_aligned_stream(float_type * dest) const
    {
        store(dest);
    }

    void clear(void)
    {
        set_vec(0);
    }
    /* @} */

    /* @{ */
    /** element access */
    void set (std::size_t index, float_type value)
    {
        data_[index] = value;
    }

    void set_vec (float_type value)
    {
        for (int i = 0; i != size; ++i)
            data_[i] = value;
    }

    float_type set_slope(float_type start, float_type slope)
    {
        float_type diff = 0;
        for (int i = 0; i != size; ++i)
        {
            data_[i] = start + diff;
            diff += slope;
        }
        return diff;
    }

    float_type set_exp(float_type start, float_type curve)
    {
        float_type value = start;
        for (int i = 0; i != size; ++i)
        {
            data_[i] = value;
            value *= curve;
        }
        return value;
    }

    float_type get (std::size_t index)
    {
        return data_[index];
    }
    /* @} */

    /* @{ */
    /** arithmetic operators */
#define OPERATOR_ASSIGNMENT(op) \
    vec & operator op(vec const & rhs) \
    { \
        for (int i = 0; i != size; ++i) \
            data_[i] op rhs.data_[i]; \
        return *this; \
    }

    OPERATOR_ASSIGNMENT(+=)
    OPERATOR_ASSIGNMENT(-=)
    OPERATOR_ASSIGNMENT(*=)
    OPERATOR_ASSIGNMENT(/=)

#define ARITHMETIC_OPERATOR(op) \
    vec operator op(vec const & rhs) \
    { \
        vec ret; \
        for (int i = 0; i != size; ++i) \
            ret.data_[i] = data_[i] op rhs.data_[i]; \
        return ret; \
    }

    ARITHMETIC_OPERATOR(+)
    ARITHMETIC_OPERATOR(-)
    ARITHMETIC_OPERATOR(*)
    ARITHMETIC_OPERATOR(/)

    /* @} */

    /* @{ */
    /** unary functions */
    friend inline vec abs(vec const & arg)
    {
        vec ret;
        for (int i = 0; i != size; ++i)
            ret.data_[i] = std::fabs(arg.data_[i]);
        return ret;
    }

    friend inline vec sign(vec const & arg)
    {
        vec ret;
        for (int i = 0; i != size; ++i)
        {
            if (arg.data_[i] > 0)
                ret.data_[i] = 1;
            else if (arg.data_[i] < 0)
                ret.data_[i] = -1;
            else ret.data_[i] = 0;
        }
        return ret;
    }

    friend inline vec square(vec const & arg)
    {
        vec ret;
        for (int i = 0; i != size; ++i)
            ret.data_[i] = arg.data_[i] * arg.data_[i];
        return ret;
    }

    friend inline vec cube(vec const & arg)
    {
        vec ret;
        for (int i = 0; i != size; ++i)
            ret.data_[i] = arg.data_[i] * arg.data_[i] * arg.data_[i];
        return ret;
    }
    /* @} */

    /* @{ */
    /** binary functions */
    friend inline vec max(vec const & lhs, vec const & rhs)
    {
        vec ret;
        for (int i = 0; i != size; ++i)
            ret.data_[i] = std::max(lhs.data_[i], rhs.data_[i]);
        return ret;
    }

    friend inline vec min(vec const & lhs, vec const & rhs)
    {
        vec ret;
        for (int i = 0; i != size; ++i)
            ret.data_[i] = std::min(lhs.data_[i], rhs.data_[i]);
        return ret;
    }
    /* @} */

    /* @{ */
    /** horizontal functions */
    inline float horizontal_min(void) const
    {
        return *std::min_element(data_, data_ + size);
    }

    inline float horizontal_max(void) const
    {
        return *std::max_element(data_, data_ + size);
    }
    /* @} */

private:
    float_type data_[size];
};

} /* namespace nova */

#undef OPERATOR_ASSIGNMENT
#undef ARITHMETIC_OPERATOR

#endif /* VEC_GENERIC_HPP */
