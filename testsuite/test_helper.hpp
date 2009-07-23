#include <boost/test/floating_point_comparison.hpp>

#include <boost/random.hpp>

template <typename float_type>
void randomize_buffer(float_type * buffer, std::size_t size)
{
    static boost::mt19937 rng;
    static boost::uniform_real<float_type> dist(-1, 1);
    static boost::variate_generator<boost::mt19937&, boost::uniform_real<float_type> >
        gen(rng, dist);

    for (std::size_t i = 0; i != size; ++i)
        buffer[i] = gen();
}

template <typename float_type>
void compare_buffers(const float_type * ref, const float_type * test, std::size_t size)
{
    for (std::size_t i = 0; i != size; ++i)
        BOOST_REQUIRE_CLOSE_FRACTION( ref[i], test[i], 1e-9 );
}
