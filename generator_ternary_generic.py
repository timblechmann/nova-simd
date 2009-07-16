# -*- coding: utf-8 -*-
#
# code generation system for simd framework
# Copyright (C) 2009 Tim Blechmann
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.


from string import Template
import sys

from generator_common import *

ternary_template = Template("""
/* vector/vector/vector */
template <typename float_type>
inline void ${label}_vec(float_type * out, const float_type * arg1, const float_type * arg2,
                         const float_type * arg3, unsigned int n)
{
    do {
        *out++ = $operation(*arg1++, *arg2++, *arg3++);
    }
    while (--n);
}

/* vector/vector/scalar */
template <typename float_type>
inline void ${label}_vec(float_type * out, const float_type * arg1, const float_type * arg2,
                         const float_type arg3, unsigned int n)
{
    do {
        *out++ = $operation(*arg1++, *arg2++, arg3);
    }
    while (--n);
}

/* vector/scalar/vector */
template <typename float_type>
inline void ${label}_vec(float_type * out, const float_type * arg1, const float_type arg2,
                         const float_type * arg3, unsigned int n)
{
    do {
        *out++ = $operation(*arg1++, arg2, *arg3++);
    }
    while (--n);
}

/* vector/scalar/scalar */
template <typename float_type>
inline void ${label}_vec(float_type * out, const float_type * arg1, const float_type arg2,
                         const float_type arg3, unsigned int n)
{
    do {
        *out++ = $operation(*arg1++, arg2, arg3);
    }
    while (--n);
}


/* scalar/vector/vector */
template <typename float_type>
inline void ${label}_vec(float_type * out, const float_type arg1, const float_type * arg2,
                         const float_type * arg3, unsigned int n)
{
    do {
        *out++ = $operation(arg1, *arg2++, *arg3++);
    }
    while (--n);
}

/* scalar/vector/scalar */
template <typename float_type>
inline void ${label}_vec(float_type * out, const float_type arg1, const float_type * arg2,
                         const float_type arg3, unsigned int n)
{
    do {
        *out++ = $operation(arg1, *arg2++, arg3);
    }
    while (--n);
}

/* scalar/scalar/vector */
template <typename float_type>
inline void ${label}_vec(float_type * out, const float_type arg1, const float_type arg2,
                         const float_type * arg3, unsigned int n)
{
    do {
        *out++ = $operation(arg1, arg2, *arg3++);
    }
    while (--n);
}

/* scalar/scalar/scalar */
template <typename float_type>
inline void ${label}_vec(float_type * out, const float_type arg1, const float_type arg2,
                         const float_type arg3, unsigned int n)
{
    do {
        *out++ = $operation(arg1, arg2, arg3);
    }
    while (--n);
}


/* vector/vector/vector */
template <typename float_type>
inline void ${label}_vec_simd(float_type * out, const float_type * arg1, const float_type * arg2,
                         const float_type * arg3, unsigned int n)
{
    ${label}_vec(out, arg1, arg2, arg3, n);
}

/* vector/vector/scalar */
template <typename float_type>
inline void ${label}_vec_simd(float_type * out, const float_type * arg1, const float_type * arg2,
                         const float_type arg3, unsigned int n)
{
    ${label}_vec(out, arg1, arg2, arg3, n);
}

/* vector/scalar/vector */
template <typename float_type>
inline void ${label}_vec_simd(float_type * out, const float_type * arg1, const float_type arg2,
                         const float_type * arg3, unsigned int n)
{
    ${label}_vec(out, arg1, arg2, arg3, n);
}

/* vector/scalar/scalar */
template <typename float_type>
inline void ${label}_vec_simd(float_type * out, const float_type * arg1, const float_type arg2,
                         const float_type arg3, unsigned int n)
{
    ${label}_vec(out, arg1, arg2, arg3, n);
}


/* scalar/vector/vector */
template <typename float_type>
inline void ${label}_vec_simd(float_type * out, const float_type arg1, const float_type * arg2,
                         const float_type * arg3, unsigned int n)
{
    ${label}_vec(out, arg1, arg2, arg3, n);
}

/* scalar/vector/scalar */
template <typename float_type>
inline void ${label}_vec_simd(float_type * out, const float_type arg1, const float_type * arg2,
                         const float_type arg3, unsigned int n)
{
    ${label}_vec(out, arg1, arg2, arg3, n);
}

/* scalar/scalar/vector */
template <typename float_type>
inline void ${label}_vec_simd(float_type * out, const float_type arg1, const float_type arg2,
                         const float_type * arg3, unsigned int n)
{
    ${label}_vec(out, arg1, arg2, arg3, n);
}

/* scalar/scalar/scalar */
template <typename float_type>
inline void ${label}_vec_simd(float_type * out, const float_type arg1, const float_type arg2,
                         const float_type arg3, unsigned int n)
{
    ${label}_vec(out, arg1, arg2, arg3, n);
}




/* vector/vector/scalar */
template <typename float_type>
inline void ${label}_vec(float_type * out, const float_type * arg1,
                         const float_type * arg2,
                         const float_type arg3, const float_type arg3_slope, unsigned int n)
{
    do {
        *out++ = $operation(*arg1++, *arg2++, arg3); arg3 += arg3_slope;
    }
    while (--n);
}

/* vector/scalar/vector */
template <typename float_type>
inline void ${label}_vec(float_type * out, const float_type * arg1,
                         const float_type arg2, const float_type arg2_slope,
                         const float_type * arg3, unsigned int n)
{
    do {
        *out++ = $operation(*arg1++, arg2, *arg3++); arg2 += arg2_slope;
    }
    while (--n);
}

/* vector/scalar/scalar */
template <typename float_type>
inline void ${label}_vec(float_type * out, const float_type * arg1,
                         const float_type arg2, const float_type arg2_slope,
                         const float_type arg3, const float_type arg3_slope,
                         unsigned int n)
{
    do {
        *out++ = $operation(*arg1++, arg2, arg3); arg2 += arg2_slope; arg3 += arg3_slope;
    }
    while (--n);
}


/* scalar/vector/vector */
template <typename float_type>
inline void ${label}_vec(float_type * out, const float_type arg1, const float_type arg1_slope,
                         const float_type * arg2,
                         const float_type * arg3, unsigned int n)
{
    do {
        *out++ = $operation(arg1, *arg2++, *arg3++); arg1 += arg1_slope;
    }
    while (--n);
}

/* scalar/vector/scalar */
template <typename float_type>
inline void ${label}_vec(float_type * out, const float_type arg1, const float_type arg1_slope,
                         const float_type * arg2,
                         const float_type arg3, const float_type arg3_slope, unsigned int n)
{
    do {
        *out++ = $operation(arg1, *arg2++, arg3); arg1 += arg1_slope; arg3 += arg3_slope;
    }
    while (--n);
}

/* scalar/scalar/vector */
template <typename float_type>
inline void ${label}_vec(float_type * out, const float_type arg1, const float_type arg1_slope,
                         const float_type arg2, const float_type arg2_slope,
                         const float_type * arg3, unsigned int n)
{
    do {
        *out++ = $operation(arg1, arg2, *arg3++); arg1 += arg1_slope; arg2 += arg2_slope;
    }
    while (--n);
}

/* scalar/scalar/scalar */
template <typename float_type>
inline void ${label}_vec(float_type * out, const float_type arg1, const float_type arg1_slope,
                         const float_type arg2, const float_type arg2_slope,
                         const float_type arg3, const float_type arg3_slope,
                         unsigned int n)
{
    do {
        *out++ = $operation(arg1, arg2, arg3); arg1 += arg1_slope; arg2 += arg2_slope; arg3 += arg3_slope;
    }
    while (--n);
}



/* vector/vector/scalar */
template <typename float_type>
inline void ${label}_vec_simd(float_type * out, const float_type * arg1,
                         const float_type * arg2,
                         const float_type arg3, const float_type arg3_slope, unsigned int n)
{
    ${label}_vec_simd(out, arg1, arg2, arg3, arg3_slope, n);
}

/* vector/scalar/vector */
template <typename float_type>
inline void ${label}_vec_simd(float_type * out, const float_type * arg1,
                         const float_type arg2, const float_type arg2_slope,
                         const float_type * arg3, unsigned int n)
{
    ${label}_vec_simd(out, arg1, arg2, arg2_slope, arg3, n);
}

/* vector/scalar/scalar */
template <typename float_type>
inline void ${label}_vec_simd(float_type * out, const float_type * arg1,
                         const float_type arg2, const float_type arg2_slope,
                         const float_type arg3, const float_type arg3_slope,
                         unsigned int n)
{
    ${label}_vec_simd(out, arg1, arg2, arg2_slope, arg3, arg3_slope, n);
}


/* scalar/vector/vector */
template <typename float_type>
inline void ${label}_vec_simd(float_type * out, const float_type arg1, const float_type arg1_slope,
                         const float_type * arg2,
                         const float_type * arg3, unsigned int n)
{
    ${label}_vec_simd(out, arg1, arg1_slope, arg2, arg3, n);
}

/* scalar/vector/scalar */
template <typename float_type>
inline void ${label}_vec_simd(float_type * out, const float_type arg1, const float_type arg1_slope,
                         const float_type * arg2,
                         const float_type arg3, const float_type arg3_slope, unsigned int n)
{
    ${label}_vec_simd(out, arg1, arg1_slope, arg2, arg3, n);
}

/* scalar/scalar/vector */
template <typename float_type>
inline void ${label}_vec_simd(float_type * out, const float_type arg1, const float_type arg1_slope,
                         const float_type arg2, const float_type arg2_slope,
                         const float_type * arg3, unsigned int n)
{
    ${label}_vec_simd(out, arg1, arg1_slope, arg2, arg2_slope, arg3, n);
}

/* scalar/scalar/scalar */
template <typename float_type>
inline void ${label}_vec_simd(float_type * out, const float_type arg1, const float_type arg1_slope,
                         const float_type arg2, const float_type arg2_slope,
                         const float_type arg3, const float_type arg3_slope,
                         unsigned int n)
{
    ${label}_vec_simd(out, arg1, arg1_slope, arg2, arg2_slope, arg3, arg3_slope, n);
}

""")

functions = """
namespace detail
{

template <typename float_type>
inline float_type clip(float_type value, float_type low, float_type high)
{
    return std::max(std::min(value, high),
                    low);
}

template <typename float_type>
inline float_type muladd(float_type value, float_type mul, float_type add)
{
    return value * mul + add;
}

} /* namespace */
"""

body = functions + \
       ternary_template.substitute(label='clip', operation = 'detail::clip<float_type>') + \
       ternary_template.substitute(label='muladd',  operation = 'detail::muladd<float_type>')

ret = generate_file(body, "templated arithmetic simd functions",
                    "SIMD_TERNARY_ARITHMETIC_GENERIC_HPP", ["<algorithm>"],
                    'nova')

if (len(sys.argv) > 1):
    open(sys.argv[1], 'w').write(ret)
else:
    print ret
