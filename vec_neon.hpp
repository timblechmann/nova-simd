//  neon vector class
//
//  Copyright (c) 2010 Tim Blechmann and Dan Stowell
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

#ifndef VEC_NEON_HPP
#define VEC_NEON_HPP

#include <arm_neon.h>

#include "detail/vec_math.hpp"

#if defined(__GNUC__) && defined(NDEBUG)
#define always_inline inline  __attribute__((always_inline))
#else
#define always_inline inline
#endif

namespace nova
{

template <>
struct vec<float>
{

    static inline float32x4_t gen_sign_mask(void)
    {
        static const int sign_mask = 0x80000000;
        float * casted = (float*)(&sign_mask);
        return vdupq_n_f32(*casted);
    }

    static inline float32x4_t gen_abs_mask(void)
    {
        static const int abs_mask = 0x7fffffff;
        float * casted = (float*)(&abs_mask);
        return vdupq_n_f32(*casted);
    }

    static inline float32x4_t gen_one(void)
    {
        return vdupq_n_f32(1.f);
    }

    static inline float32x4_t gen_05(void)
    {
        return vdupq_n_f32(0.5f);
    }

    static inline float32x4_t gen_zero(void)
    {
        return vdupq_n_f32(0.f);
    }

    vec(float32x4_t const & arg):
        data_(arg)
    {}

public:
    static const int size = 4;
    static const int objects_per_cacheline = 64/sizeof(float);

    /* @{ */
    /** constructors */
    vec(void)
    {}

    explicit vec(float f)
    {
        set_vec(f);
    }

    vec(vec const & rhs)
    {
        data_ = rhs.data_;
    }
    /* @} */

    /* @{ */
    /** io */
    void load(const float * data)
    {
        data_ = vld1q_f32(data);
    }

// TODO check what's available re alignment in NEON
    void load_aligned(const float * data)
    {
        load(data);
    }

// TODO is there a good NEON for this?
// e.g. zero it all, then vld1q_lane_f32() for the first one
    void load_first(const float * data)
    {
        data_[0] = *data;

        for (int i = 1; i != size; ++i)
            data_[i] = 0;
    }

    void store(float * dest) const
    {
        vst1q_f32(dest, data_);
    }

// TODO check what's available re alignment in NEON
    void store_aligned(float * dest) const
    {
        store(dest);
    }

// TODO check what's available re alignment in NEON
    void store_aligned_stream(float * dest) const
    {
        store(dest, data_);
    }

    // no particular setzero intrinsic
    void clear(void)
    {
        set_vec(0);
    }

    /* @} */

    /* @{ */
    /** element access */
    void set (std::size_t index, float value)
    {
		data_ = vsetq_lane_f32(value, data_, index);
	}

// TODO surely we could do this in one?
    void set_vec (float value)
    {
		data_ = vsetq_lane_f32(value, data_, 0);
		data_ = vsetq_lane_f32(value, data_, 1);
		data_ = vsetq_lane_f32(value, data_, 2);
		data_ = vsetq_lane_f32(value, data_, 3);
        //for (int i = 0; i != size; ++i)
        //    data_[i] = value;
    }

	// still generic
    float set_slope(float start, float slope)
    {
        float diff = 0;
        for (int i = 0; i != size; ++i)
        {
            data_[i] = start + diff;
            diff += slope;
        }
        return diff;
    }

	// still generic
    float set_exp(float start, float curve)
    {
        float value = start;
        for (int i = 0; i != size; ++i)
        {
            data_[i] = value;
            value *= curve;
        }
        return value;
    }

    float get (std::size_t index)
    {
		return vgetq_lane_f32(data_, (const int)index);
	}
    /* @} */

    /* @{ */
    /** arithmetic operators */
#define OPERATOR_ASSIGNMENT(op, opcode) \
    vec & operator op(vec const & rhs) \
    { \
        data_ = opcode(data_, rhs.data_);\
        return *this;\
    }

    OPERATOR_ASSIGNMENT(+=, vaddq_f32)
    OPERATOR_ASSIGNMENT(-=, vsubq_f32)
    OPERATOR_ASSIGNMENT(*=, vmulq_f32)
    vec & operator /=(vec const & rhs)
    {
    	// No fancy divide available in NEON
        for (int i = 0; i != size; ++i)
            data_[i] /= rhs.data_[i];
        return *this;
    }

#define ARITHMETIC_OPERATOR(op, opcode) \
    vec operator op(vec const & rhs) const \
    { \
        return opcode(data_, rhs.data_); \
    }

    ARITHMETIC_OPERATOR(+, vaddq_f32)
    ARITHMETIC_OPERATOR(-, vsubq_f32)
    ARITHMETIC_OPERATOR(*, vmulq_f32)
    vec operator /(vec const & rhs) const
    {
    	// No fancy divide available in NEON
        vec ret;
        for (int i = 0; i != size; ++i)
            ret.data_[i] = data_[i] / rhs.data_[i];
        return ret;
    }

//This is done similarly to the SSE version but with reinterpret casts
// since the comparison result is done on integers, and AND is integer only.
#define RELATIONAL_OPERATOR(op, opcode) \
    vec operator op(vec const & rhs) const \
    { \
        const uint32x4_t one = vreinterpretq_u32_f32(gen_one()); \
        return vreinterpretq_f32_u32(vandq_u32( \
            opcode(data_, rhs.data_), one)); \
    }

     RELATIONAL_OPERATOR(<, vcltq_f32)
     RELATIONAL_OPERATOR(<=, vcleq_f32)
     RELATIONAL_OPERATOR(>, vcgtq_f32)
     RELATIONAL_OPERATOR(>=, vcgeq_f32)
     RELATIONAL_OPERATOR(==, vceqq_f32)
	// No atomic "neq" so here we do "eq" then bitwise-not
    vec operator !=(vec const & rhs) const
    {
        const uint32x4_t one = vreinterpretq_u32_f32(gen_one());
        return vreinterpretq_f32_u32(vandq_u32(
            vmvnq_u32(vceqq_f32(data_, rhs.data_)), one));
    }

    /* @{ */
    // Again, reinterprets added here
#define BITWISE_OPERATOR(op, opcode) \
    vec operator op(vec const & rhs) const \
    { \
        return vreinterpretq_f32_u32(opcode( \
            vreinterpretq_u32_f32(data_), vreinterpretq_u32_f32(rhs.data_))); \
    }

    BITWISE_OPERATOR(&, vandq_u32)
    BITWISE_OPERATOR(|, vorrq_u32)
    BITWISE_OPERATOR(^, veorq_u32)

//TODO frankly I'm not sure what these "relational mask operators" are for so may have made a mistake
    #define RELATIONAL_MASK_OPERATOR(op, opcode) \
    friend vec mask_##op(vec const & lhs, vec const & rhs) \
    { \
        return vreinterpretq_f32_u32(opcode( \
            vreinterpretq_u32_f32(lhs.data_), vreinterpretq_u32_f32(rhs.data_))); \
    }

    RELATIONAL_MASK_OPERATOR(lt, vcltq_f32)
    RELATIONAL_MASK_OPERATOR(le, vcleq_f32)
    RELATIONAL_MASK_OPERATOR(gt, vcgtq_f32)
    RELATIONAL_MASK_OPERATOR(ge, vcgeq_f32)
    RELATIONAL_MASK_OPERATOR(eq, vceqq_f32)
	// No atomic "neq" so here we do "eq" then bitwise-not
    friend vec mask_neq(vec const & lhs, vec const & rhs) \
    { \
        return vreinterpretq_f32_u32(vmvnq_u32(vceqq_f32( \
            vreinterpretq_u32_f32(lhs.data_), vreinterpretq_u32_f32(rhs.data_)))); \
    }

    friend inline vec select(vec lhs, vec rhs, vec bitmask)
    {
        /* if bitmask is set, return value in rhs, else value in lhs */
        return vreinterpretq_f32_u32(vorrq_u32(vbicq_u32(vreinterpretq_u32_f32(bitmask.data_), vreinterpretq_u32_f32(lhs.data_)),
                        vandq_u32(vreinterpretq_u32_f32(rhs.data_), vreinterpretq_u32_f32(bitmask.data_))));
    }

    /* @} */

    /* @{ */
    /** unary functions */
    friend inline vec abs(vec const & arg)
    {
        return vabsq_f32(arg.data_);
    }

    friend inline vec square(vec const & arg)
    {
        return vmulq_f32(arg.data_, arg.data_);
    }

//TODO: estimate ok?  check "reciprocal square root estimate"
//  (it's likely we'll need to use the generic implementation)
//     friend inline vec sqrt(vec const & arg)
//     {
//         return _mm_sqrt_ps(arg.data_);
//     }

    friend inline vec cube(vec const & arg)
    {
        return vmulq_f32(arg.data_, vmulq_f32(arg.data_, arg.data_));
    }
    /* @} */

    /* @{ */
    /** binary functions */
    friend inline vec max_(vec const & lhs, vec const & rhs)
    {
        return vmaxq_f32(lhs.data_, rhs.data_);
    }

    friend inline vec min_(vec const & lhs, vec const & rhs)
    {
        return vminq_f32(lhs.data_, rhs.data_);
    }
    /* @} */

    /* @{ */
    /** unary functions */
#define APPLY_UNARY(NAME, FUNCTION)                 \
    friend inline vec NAME(vec const & arg)         \
    {                                               \
        vec ret;                                    \
        detail::apply_on_vector<float, size> (ret.data_, arg.data_,                \
                                                   FUNCTION);    \
        return ret;                                 \
    }

    APPLY_UNARY(sign, detail::sign<float>)
    /* @} */

    /* @{ */
    /** rounding functions */
    APPLY_UNARY(round, detail::round<float>)
    APPLY_UNARY(frac, detail::frac<float>)
    APPLY_UNARY(floor, detail::floor<float>)
    APPLY_UNARY(ceil, detail::ceil<float>)
    /* @} */

    /* @{ */
    /** mathematical functions */

#define APPLY_BINARY(NAME, FUNCTION)                            \
    friend inline vec NAME(vec const & lhs, vec const & rhs)    \
    {                                                           \
        vec ret;                                                \
        detail::apply_on_vector<float, size> ((float*)&ret.data_,\
                                              wrap_argument((float*)&lhs.data_), \
                                              wrap_argument((float*)&rhs.data_),  \
                                              FUNCTION);   \
        return ret;                                 \
    }

    APPLY_UNARY(sin, detail::sin<float>)
    APPLY_UNARY(cos, detail::cos<float>)
    APPLY_UNARY(tan, detail::tan<float>)
    APPLY_UNARY(asin, detail::asin<float>)
    APPLY_UNARY(acos, detail::acos<float>)
    APPLY_UNARY(atan, detail::atan<float>)

    APPLY_UNARY(log, detail::log<float>)
    APPLY_UNARY(exp, detail::exp<float>)

    APPLY_BINARY(pow, detail::pow<float>)

#undef APPLY_UNARY
#undef APPLY_BINARY

    friend inline vec tanh(vec const & arg)
    {
        return detail::vec_tanh_float(arg);
    }

    friend inline vec signed_pow(vec const & lhs, vec const & rhs)
    {
        return detail::vec_signed_pow(lhs, rhs);
    }

    friend inline vec signed_sqrt(vec const & arg)
    {
        return detail::vec_signed_sqrt(arg);
    }

    friend inline vec log2(vec const & arg)
    {
        return detail::vec_log2(arg);
    }

    friend inline vec log10(vec const & arg)
    {
        return detail::vec_log10(arg);
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
    typedef union
    {
        float f[4];
        float32x4_t m;
    } cast_union;

    float32x4_t data_;
};

} /* namespace nova */


#undef OPERATOR_ASSIGNMENT
#undef ARITHMETIC_OPERATOR
#undef RELATIONAL_OPERATOR
#undef always_inline

#endif /* VEC_NEON_HPP */
