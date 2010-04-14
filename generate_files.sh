#!/bin/sh
python generator_ternary_generic.py   simd_ternary_arithmetic_generic.hpp
python generator_ternary_sse.py       simd_ternary_arithmetic_sse.hpp
python generator_ternary_fallbacks.py simd_ternary_arithmetic_fallbacks_float.hpp \
                                      simd_ternary_arithmetic_fallbacks_double.hpp
