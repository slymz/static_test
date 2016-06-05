#include <static_test/pp/misc.hpp>
#include <static_test/pp/delay.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/dec.hpp>
#include <boost/preprocessor/variadic/size.hpp>

#include <vector>
#include <type_traits>
#include <utility>
#include <string>


#include <gtest/gtest.h>


//     +---------+
//     |  DELAY  |
//     +---------+

namespace {
    // dummy function used in macro expansion examples below
    int bar(int, int, int) { return 11; }
}

// no runtime thing but just for the show
TEST( Preprocessor, DELAY ) {

    #define FOO(...)  BOOST_PP_CAT ( FOO_, 1( __VA_ARGS__ ) )
    #define FOO_1(op, ...) op( __VA_ARGS__ )

    // some silly expressions, if not for ST_PP_DELAY would be invalid
    FOO(decltype, 1) i = 3;
    EXPECT_EQ( i, 3 );

    i = FOO(sizeof, std::pair< int, std::string> );
    EXPECT_EQ(i, sizeof(std::pair<int, std::string>) );

    i = FOO(bar, 1, 2, 3);
    EXPECT_EQ( i, bar(1,2,3) );

    #undef FOO


    //! [dox: delay-variadic-size]
    // Attempt 1: There is actually already a Boost.Preprocessor macro for that, so let's give it a go:
    #define VARIADIC_SIZE(...) BOOST_PP_VARIADIC_SIZE( __VA_ARGS__ )

    // sanity checks, as expected:
    static_assert(3 == VARIADIC_SIZE(a, b, c), "three arguments");
    static_assert(2 == VARIADIC_SIZE(a, b),    "two arguments");
    static_assert(1 == VARIADIC_SIZE(x),       "one argument");

    // oops! BOOST_PP_VARIADIC_SIZE isn't accurate for empty arg list
    static_assert(1 == VARIADIC_SIZE(), "no argument - but result is not zero!" );

    #undef VARIADIC_SIZE

    // Ok, let's try again with a dirty tactic: Augment the variadic list with one extra argument,
    // compute its size, and decrement:
    #define VARIADIC_SIZE(...) BOOST_PP_DEC( BOOST_PP_VARIADIC_SIZE(x,__VA_ARGS__ ) )

    // sanity checks, as expected:
    static_assert(3 == VARIADIC_SIZE(a, b, c), "three arguments");
    static_assert(2 == VARIADIC_SIZE(a, b),    "two arguments");
    static_assert(1 == VARIADIC_SIZE(x),       "one argument");

    // oops, again! For similar reasons, there is no escaping the empty argument
    static_assert(1 == VARIADIC_SIZE(), "no argument - but result is not zero!");

    // Actually, same as VARIADIC_SIZE goes through the following chain of expansions:
    // BOOST_PP_DEC( BOOST_PP_VARIADIC_SIZE(x) __VA_ARGS__ ) : the bug kicks in
    // BOOST_PP_DEC( BOOST_PP_VARIADIC_SIZE(x) )  :

    #undef VARIADIC_SIZE

    // Let's try one more time, this time, we delay the application of BOOST_PP_VARIADIC_SIZE
    // Note that we also have to delay BOOST_PP_DEC (decrement), otherwise it gets called straight
    // away on the inner ST_PP_DELAY before it's expansion
    #define VARIADIC_SIZE(...) ST_PP_DELAY( BOOST_PP_DEC, ( ST_PP_DELAY( BOOST_PP_VARIADIC_SIZE,(x,__VA_ARGS__ ) ) ))

    // sanity checks, as expected:
    static_assert(3 == VARIADIC_SIZE(a, b, c), "three arguments");
    static_assert(2 == VARIADIC_SIZE(a, b),    "two arguments");
    static_assert(1 == VARIADIC_SIZE(x),       "one argument");
    // voila! :( only on windoze
#ifdef BOOST_MSVC
    static_assert(0 == VARIADIC_SIZE(),        "no argument");
#else
    static_assert(1 == VARIADIC_SIZE(),        "no argument");
#endif
    //! [dox: delay-variadic-size]

}

