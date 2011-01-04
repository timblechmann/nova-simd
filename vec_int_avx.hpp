//  avx int vector class
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

#ifndef VEC_AVX_INT_HPP
#define VEC_AVX_INT_HPP


namespace nova   {
namespace detail {

struct int_vec_avx
{
    __m256i data_;

    /* cast */
    explicit int_vec_avx(int arg):
        data_(_mm256_set1_epi32(arg))
    {}

    int_vec_avx(__m256i arg):
        data_(arg)
    {}

    int_vec_avx(__m256 arg):
        data_(_mm256_castps_si256(arg))
    {}

    int_vec_avx(int_vec_avx const & arg):
        data_(arg.data_)
    {}

    int_vec_avx(void)
    {}

    #define APPLY_M128_OPCODE(op, opcode) \
    friend int_vec_avx op(int_vec_avx const & lhs, int_vec_avx const & rhs) \
    { \
        __m256 lhs_data = _mm256_castsi256_ps(lhs.data_);       \
        __m256 rhs_data = _mm256_castsi256_ps(rhs.data_);       \
        __m128 lhs_low =  _mm256_castps256_ps128(lhs_data);    \
        __m128 lhs_hi =   _mm256_extractf128_ps(lhs_data, 1);  \
        __m128 rhs_low =  _mm256_castps256_ps128(rhs_data);    \
        __m128 rhs_hi =   _mm256_extractf128_ps(rhs_data, 1);  \
\
        __m128 newlow = (__m128)opcode((__m128i)lhs_low, (__m128i)rhs_low); \
        __m128 newhi  = (__m128)opcode((__m128i)lhs_hi,  (__m128i)rhs_hi);  \
\
        __m256 result = _mm256_castps128_ps256(newlow);  \
        result = _mm256_insertf128_ps(result,  newhi, 1);   \
        return result;   \
    }

private:
    APPLY_M128_OPCODE(plus, _mm_add_epi32)
    APPLY_M128_OPCODE(minus, _mm_sub_epi32)

public:
    int_vec_avx & operator+(int_vec_avx const & rhs)
    {
        *this = plus(*this, rhs);
        return *this;
    }

    int_vec_avx & operator-(int_vec_avx const & rhs)
    {
        *this = minus(*this, rhs);
        return *this;
    }

    APPLY_M128_OPCODE(mask_lt, _mm_cmplt_epi32)
    APPLY_M128_OPCODE(mask_gt, _mm_cmpgt_epi32)
    APPLY_M128_OPCODE(mask_eq, _mm_cmpeq_epi32)

    #undef APPLY_M128_OPCODE

    friend int_vec_avx operator&(int_vec_avx const & lhs, int_vec_avx const & rhs)
    {
        return int_vec_avx(_mm256_and_ps(_mm256_castsi256_ps(lhs.data_),
                                        _mm256_castsi256_ps(rhs.data_)));
    }

    friend inline int_vec_avx andnot(int_vec_avx const & lhs, int_vec_avx const & rhs)
    {
        return int_vec_avx(_mm256_andnot_ps(_mm256_castsi256_ps(lhs.data_),
                                        _mm256_castsi256_ps(rhs.data_)));
    }


    // shift in zeros
    friend inline int_vec_avx slli(int_vec_avx const & arg, int count)
    {
        __m256 arg_data = _mm256_castsi256_ps(arg.data_);
        __m128 arg_low =  _mm256_castps256_ps128(arg_data);
        __m128 arg_hi =   _mm256_extractf128_ps(arg_data, 1);

        __m128 newlow = (__m128)_mm_slli_epi32((__m128i)arg_low, count);
        __m128 newhi  = (__m128)_mm_slli_epi32((__m128i)arg_hi,  count);

        __m256 result = _mm256_castps128_ps256(newlow);
        result = _mm256_insertf128_ps(result,  newhi, 1);
        return result;
    }

    // shift in zeros
    friend inline int_vec_avx srli(int_vec_avx const & arg, int count)
    {
        __m256 arg_data = _mm256_castsi256_ps(arg.data_);
        __m128 arg_low =  _mm256_castps256_ps128(arg_data);
        __m128 arg_hi =   _mm256_extractf128_ps(arg_data, 1);

        __m128 newlow = (__m128)_mm_srli_epi32((__m128i)arg_low, count);
        __m128 newhi  = (__m128)_mm_srli_epi32((__m128i)arg_hi,  count);

        __m256 result = _mm256_castps128_ps256(newlow);
        result = _mm256_insertf128_ps(result,  newhi, 1);
        return result;
    }

    __m256 convert_to_float(void) const
    {
        return _mm256_cvtepi32_ps(data_);
    }
};


}
}

#endif /* VEC_AVX_INT_HPP */
