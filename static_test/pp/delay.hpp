/** 
  @copyright S. Levent Yilmaz 2016
  Distributed under the Boost Software License, Version 1.0.
  See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt.
*/

#pragma once


#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/dec.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/comma_if.hpp>
#include <boost/preprocessor/expr_if.hpp>


#ifndef BOOST_PP_VARIADICS
# error "Usage of ST_PP_DELAY requires variadic macro support"
#endif

/// Delay the application of a given macro.
/**
E.g. `ST_PP_DELAY( macro, (1,2,3) )` expands to `macro(1,2,3)` is a delayed fashion.

A naive use of chained variadic macro calls in Visual Studio is problematic:
@code

// Take a macro FOO which wants to treat the first argument special:
#define FOO(...) FOO_1(  __VA_ARGS__ )
#define FOO_1(a, ...)  a( __VA_ARGS__ )

FOO(decltype, a);   // expected: decltype(a);
FOO(get, a, b);     // expected: get(a,b);
FOO(bar, a, b, c);  // expected: bar(a,b,c);

@endcode

However, due to a what seems to be a bug in its implementation,
 the Visual C++ preprocessor expands the above to:

@code{.txt}
decltype, a(  );
get, a, b(  );
bar, a, b, c(  );
@endcode

A delayed application of the macro avoids this problem:

@code
// redefine FOO to call FOO_1 in a delayed fashion
#define FOO(...) ST_PP_DELAY(FOO_1,  (__VA_ARGS__))
// FOO_1 is the same
@endcode

Here is another slightly more interesting use case. We want a variadic macro to count the
number of arguments in the call:
@snippet tPreprocessor.cpp dox: delay-variadic-size
*/
#define ST_PP_DELAY(macro,argsTuple) ST_PP_DELAY_N(1, macro, argsTuple)


/// Delay n-times. This is useful in recursive applications of variadic macros
#define ST_PP_DELAY_N( n, macro, argsTuple ) ST_PP_DELAY_impl_ ## n (macro,argsTuple)


#define ST_PP_DELAY_impl_0( macro, arg ) macro arg
#define ST_PP_DELAY_impl_1( macro, arg ) ST_PP_DELAY_impl ## _0(macro,arg)
#define ST_PP_DELAY_impl_2( macro, arg ) ST_PP_DELAY_impl ## _1(macro,arg)
#define ST_PP_DELAY_impl_3( macro, arg ) ST_PP_DELAY_impl ## _2(macro,arg)
#define ST_PP_DELAY_impl_4( macro, arg ) ST_PP_DELAY_impl ## _3(macro,arg)
#define ST_PP_DELAY_impl_5( macro, arg ) ST_PP_DELAY_impl ## _4(macro,arg)
#define ST_PP_DELAY_impl_6( macro, arg ) ST_PP_DELAY_impl ## _5(macro,arg)
#define ST_PP_DELAY_impl_7( macro, arg ) ST_PP_DELAY_impl ## _6(macro,arg)
#define ST_PP_DELAY_impl_8( macro, arg ) ST_PP_DELAY_impl ## _7(macro,arg)
#define ST_PP_DELAY_impl_9( macro, arg ) ST_PP_DELAY_impl ## _8(macro,arg)
