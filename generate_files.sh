#!/bin/sh
python generator_ternary_generic.py   simd_ternary_arithmetic_generic.hpp
python generator_ternary_sse.py       simd_ternary_arithmetic_sse.hpp
python generator_ternary_fallbacks.py simd_ternary_arithmetic_fallbacks_float.hpp \
                                      simd_ternary_arithmetic_fallbacks_double.hpp

python generator_round_generic.py     simd_round_generic.hpp
python generator_round_sse.py         simd_round_sse41.hpp
