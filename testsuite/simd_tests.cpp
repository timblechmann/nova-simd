#include <iostream>
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <cmath>

#include "../benchmarks/cache_aligned_array.hpp"
#include "../simd_peakmeter.hpp"

using namespace nova;
using namespace std;

static const int size = 64;

#if 0
/* test range: -25, 25 */
BOOST_AUTO_TEST_CASE( sign_test )
{
    aligned_array<float, size> in;
    aligned_array<float, size> out;
    in[0] = -2;
    in[1] = in[2] = 0;
    in[3] = 2;

    nova::sgn_vec_simd(out.begin(), in.begin(), size);

    BOOST_REQUIRE_EQUAL( out[0], -1 );
    BOOST_REQUIRE_EQUAL( out[1], 0 );
    BOOST_REQUIRE_EQUAL( out[2], 0 );
    BOOST_REQUIRE_EQUAL( out[3], 1 );
}
#endif

template <typename F>
void run_peak(void)
{
    aligned_array<F, size> in;
    in.assign(0);

    in[63] = -0.5;
    in[40] = 1;
    {
        F peak = 0;
        F last = nova::peak_vec_simd(in.begin(), &peak, size);

        BOOST_REQUIRE_EQUAL( peak, 1 );
        BOOST_REQUIRE_EQUAL( last, 0.5 );
    }

    {
        F peak = 0;
        F last = nova::peak_vec(in.begin(), &peak, size);

        BOOST_REQUIRE_EQUAL( peak, 1 );
        BOOST_REQUIRE_EQUAL( last, 0.5 );
    }
}

BOOST_AUTO_TEST_CASE( peak_test_float )
{
    run_peak<float>();
}

BOOST_AUTO_TEST_CASE( peak_test_double )
{
    run_peak<double>();
}
