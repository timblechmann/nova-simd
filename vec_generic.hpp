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
        float_type value = start;
        for (int i = 0; i != size; ++i)
        {
            data_[i] = value;
            value += slope;
        }
        return value;
    }

    float_type get (std::size_t index)
    {
        return data_[index];
    }
    /* @} */

    /* @{ */
    /** vector arithmetics */
    vec & operator+=(vec const & rhs)
    {
        for (int i = 0; i != size; ++i)
            data_[i] += rhs.data_[i];
        return *this;
    }

    /* @} */

private:
    float_type data_[size];
};

} /* namespace nova */

#endif /* VEC_GENERIC_HPP */
