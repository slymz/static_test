/** 
  @copyright S. Levent Yilmaz 2016
  Distributed under the Boost Software License, Version 1.0.
  See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt.
*/

#pragma once
#include "delay.hpp"

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/comparison/less_equal.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/cat.hpp>
#include <boost/preprocessor/seq/rest_n.hpp>
#include <boost/preprocessor/seq/first_n.hpp>
#include <boost/preprocessor/seq/enum.hpp>

/// Removes any number of nested parenthesis from expression `x`
/**

The following expressions all expand to `a,b,c`
@code
ST_PP_REMOVE_PAREN(a,b,c)
ST_PP_REMOVE_PAREN((a,b,c))
ST_PP_REMOVE_PAREN(((a,b,c)))
ST_PP_REMOVE_PAREN((((a,b,c))))
@endcode

This sort of thing is useful in implementing multi-argument macros with natural user-end syntax,
where individual arguments to the macro allows parenthesized argument to protect against commas.
The motivation is similar to  BOOST_IDENTITY_TYPE macro
(\see http://www.boost.org/doc/libs/1_60_0/libs/utility/identity_type/doc/html/index.html).
BOOST_IDENTITY_TYPE macro suffers from the following limitations:

-# It can not be used for non-type arguments
-# It does not distinguished between `const` and non-`const` value types. This looks like a bug
   in its implementation (which is a hack over function argument types).
*/

// Accept any number of arguments or one parenthesized argument, then
// 1. "eat away" the parenthesis with recursive call to DEPAREN
// 2. Delayed call to EATDEPAREN_AND_ENUM. Delayed call is required to circumvent
//    visual studio bug.
#define ST_PP_REMOVE_PAREN(...) \
    ST_PP_DELAY( ST_PP_REMOVE_PAREN_impl_EATDEPAREN_AND_ENUM, (ST_PP_REMOVE_PAREN_impl_DEPAREN_0 __VA_ARGS__) )

// Arguments are the tail of recursive DEPAREN and one or more comma separated user arguments to REMOVE_PAREN
#define ST_PP_REMOVE_PAREN_impl_EATDEPAREN_AND_ENUM(...) \
    ST_PP_REMOVE_PAREN_impl_CAT_FIRST_TWO_ENUM_REST( (EAT_)BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__) )

// "Recursive" remove nested parenthesis
#define ST_PP_REMOVE_PAREN_impl_DEPAREN_0(...)  ST_PP_REMOVE_PAREN_impl_DEPAREN_1  __VA_ARGS__
#define ST_PP_REMOVE_PAREN_impl_DEPAREN_1(...)  ST_PP_REMOVE_PAREN_impl_DEPAREN_2  __VA_ARGS__
#define ST_PP_REMOVE_PAREN_impl_DEPAREN_2(...)  ST_PP_REMOVE_PAREN_impl_DEPAREN_3  __VA_ARGS__
#define ST_PP_REMOVE_PAREN_impl_DEPAREN_3(...)  ST_PP_REMOVE_PAREN_impl_DEPAREN_4  __VA_ARGS__
#define ST_PP_REMOVE_PAREN_impl_DEPAREN_4(...)  ST_PP_REMOVE_PAREN_impl_DEPAREN_5  __VA_ARGS__
#define ST_PP_REMOVE_PAREN_impl_DEPAREN_5(...)  ST_PP_REMOVE_PAREN_impl_DEPAREN_6  __VA_ARGS__
#define ST_PP_REMOVE_PAREN_impl_DEPAREN_6(...)  ST_PP_REMOVE_PAREN_impl_DEPAREN_7  __VA_ARGS__
#define ST_PP_REMOVE_PAREN_impl_DEPAREN_7(...)  ST_PP_REMOVE_PAREN_impl_DEPAREN_8  __VA_ARGS__
#define ST_PP_REMOVE_PAREN_impl_DEPAREN_8(...)  ST_PP_REMOVE_PAREN_impl_DEPAREN_9  __VA_ARGS__
#define ST_PP_REMOVE_PAREN_impl_DEPAREN_9(...)  ST_PP_REMOVE_PAREN_impl_DEPAREN_LIMIT_BREACH

// EAT the tail of recursion
#define EAT_ST_PP_REMOVE_PAREN_impl_DEPAREN_0
#define EAT_ST_PP_REMOVE_PAREN_impl_DEPAREN_1
#define EAT_ST_PP_REMOVE_PAREN_impl_DEPAREN_2
#define EAT_ST_PP_REMOVE_PAREN_impl_DEPAREN_3
#define EAT_ST_PP_REMOVE_PAREN_impl_DEPAREN_4
#define EAT_ST_PP_REMOVE_PAREN_impl_DEPAREN_5
#define EAT_ST_PP_REMOVE_PAREN_impl_DEPAREN_6
#define EAT_ST_PP_REMOVE_PAREN_impl_DEPAREN_7
#define EAT_ST_PP_REMOVE_PAREN_impl_DEPAREN_8
#define EAT_ST_PP_REMOVE_PAREN_impl_DEPAREN_9
#define EAT_ST_PP_REMOVE_PAREN_impl_DEPAREN_LIMIT_BREACH static_assert( false, "Too many nested parenthesis, recusion supports up to 9 levels." )

/// @def ST_PP_LIMIT_REMOVE_PAREN
/// The ST_PP_LIMIT_REMOVE_PAREN macro defines the maximum number of nested
/// parentheses that can be removed. This is currently 9.
#define ST_PP_LIMIT_REMOVE_PAREN 9


// Given seq of (EAT_)(tail-of-DEPAREN a1)(a2)(a3).. where `(a2)(a3)...` is only present if there were
// commas in the original set of arguments, cat the first two elements in seq, and enum the rest
// if any:
#define ST_PP_REMOVE_PAREN_impl_CAT_FIRST_TWO_ENUM_REST( seq ) \
   BOOST_PP_IIF(                                                \
       BOOST_PP_LESS_EQUAL(BOOST_PP_SEQ_SIZE(seq), 2),          \
       BOOST_PP_SEQ_CAT,                                        \
       ST_PP_REMOVE_PAREN_impl_CAT_FIRST_TWO_ENUM_REST_n       \
       )(seq)                                                   \
/**/

#define ST_PP_REMOVE_PAREN_impl_CAT_FIRST_TWO_ENUM_REST_n(seq) \
   BOOST_PP_SEQ_CAT( BOOST_PP_SEQ_FIRST_N(2, seq) )             \
   ,                                                            \
   BOOST_PP_SEQ_ENUM( BOOST_PP_SEQ_REST_N(2, seq) )             \
/**/
