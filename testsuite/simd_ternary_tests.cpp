#include <iostream>
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <cmath>

#include "../benchmarks/cache_aligned_array.hpp"
#include "../simd_ternary_arithmetic.hpp"

using namespace nova;
using namespace std;

static const int size = 64;
