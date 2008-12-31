# -*- coding: utf-8 -*-
#
# code generation system for simd framework
# Copyright (C) 2008 Tim Blechmann
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

binary_template = Template("""
template <typename float_type>
inline void ${label}_vec(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = $operation(*arg1++, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void ${label}_vec(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    do {
        *out++ = $operation(*arg1++, arg2);
    }
    while (--n);
}

template <typename float_type>
inline void ${label}_vec(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    do {
        *out++ = $operation(arg1, *arg2++);
    }
    while (--n);
}

template <typename float_type>
inline void ${label}_vec(float_type * out, const float_type * arg1, float_type arg2,
                  const float_type arg2_slope, unsigned int n)
{
    do {
        *out++ = $operation(*arg1++, arg2);
        arg2 += arg2_slope;
    }
    while (--n);
}

template <typename float_type>
inline void ${label}_vec(float_type * out, float_type arg1, const float_type arg1_slope,
                  const float_type * arg2, unsigned int n)
{
    do {
        *out++ = $operation(arg1, *arg2++);
        arg1 += arg1_slope;
    }
    while (--n);
}

template <typename float_type>
inline void ${label}_vec_simd(float_type * out, const float_type * arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = $operation(*arg1++, *arg2++);
        *out++ = $operation(*arg1++, *arg2++);
        *out++ = $operation(*arg1++, *arg2++);
        *out++ = $operation(*arg1++, *arg2++);
        *out++ = $operation(*arg1++, *arg2++);
        *out++ = $operation(*arg1++, *arg2++);
        *out++ = $operation(*arg1++, *arg2++);
        *out++ = $operation(*arg1++, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void ${label}_vec_simd(float_type * out, const float_type * arg1, const float_type arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = $operation(*arg1++, arg2);
        *out++ = $operation(*arg1++, arg2);
        *out++ = $operation(*arg1++, arg2);
        *out++ = $operation(*arg1++, arg2);
        *out++ = $operation(*arg1++, arg2);
        *out++ = $operation(*arg1++, arg2);
        *out++ = $operation(*arg1++, arg2);
        *out++ = $operation(*arg1++, arg2);
    }
    while (--loops);
}

template <typename float_type>
inline void ${label}_vec_simd(float_type * out, const float_type arg1, const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = $operation(arg1, *arg2++);
        *out++ = $operation(arg1, *arg2++);
        *out++ = $operation(arg1, *arg2++);
        *out++ = $operation(arg1, *arg2++);
        *out++ = $operation(arg1, *arg2++);
        *out++ = $operation(arg1, *arg2++);
        *out++ = $operation(arg1, *arg2++);
        *out++ = $operation(arg1, *arg2++);
    }
    while (--loops);
}

template <typename float_type>
inline void ${label}_vec_simd(float_type * out, const float_type * arg1, float_type arg2,
                              float_type arg2_slope, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = $operation(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = $operation(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = $operation(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = $operation(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = $operation(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = $operation(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = $operation(*arg1++, arg2); arg2 += arg2_slope;
        *out++ = $operation(*arg1++, arg2); arg2 += arg2_slope;
    }
    while (--loops);
}

template <typename float_type>
inline void ${label}_vec_simd(float_type * out, float_type arg1, const float_type arg1_slope,
                              const float_type * arg2, unsigned int n)
{
    unsigned int loops = n / 8;
    do {
        *out++ = $operation(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = $operation(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = $operation(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = $operation(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = $operation(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = $operation(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = $operation(arg1, *arg2++); arg1 += arg1_slope;
        *out++ = $operation(arg1, *arg2++); arg1 += arg1_slope;
    }
    while (--loops);
}
""")

body = binary_template.substitute(label='plus',  operation = 'std::plus<float_type>()') + \
    binary_template.substitute(label='minus', operation = 'std::minus<float_type>()') + \
    binary_template.substitute(label='times', operation = 'std::multiplies<float_type>()') + \
    binary_template.substitute(label='over', operation = 'std::divides<float_type>()') + \
    binary_template.substitute(label='min', operation = 'std::min<float_type>') + \
    binary_template.substitute(label='max', operation = 'std::max<float_type>') + \
    binary_template.substitute(label='less', operation = 'std::less<float_type>()') + \
    binary_template.substitute(label='less_equal', operation = 'std::less_equal<float_type>()') + \
    binary_template.substitute(label='greater', operation = 'std::greater<float_type>()') + \
    binary_template.substitute(label='greater_equal', operation = 'std::greater_equal<float_type>()') + \
    binary_template.substitute(label='equal', operation = 'std::equal_to<float_type>()') + \
    binary_template.substitute(label='notequal', operation = 'std::not_equal_to<float_type>()')

ret = generate_file(body, "templated arithmetic simd functions",
                    "SIMD_BINARY_ARITHMETIC_GENERIC_HPP", ["<functional>", "<algorithm>"],
                    'nova')

if (len(sys.argv) > 1):
    open(sys.argv[1], 'w').write(ret)
else:
    print ret
