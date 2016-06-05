/** 
  @copyright S. Levent Yilmaz 2016
  Distributed under the Boost Software License, Version 1.0.
  See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt.
*/

#pragma once

#include <boost/config.hpp>

//     +------------------------+
//     |  Installation Options  |
//     +------------------------+

#ifndef ST_CONFIG_NAMESPACE_OPEN
#  define ST_CONFIG_NAMESPACE_OPEN namespace st {
#endif

#ifndef ST_CONFIG_NAMESPACE_CLOSE
#  define ST_CONFIG_NAMESPACE_CLOSE }
#endif

//     +------------------------+
//     |  Installation Options  |
//     +------------------------+

// Number of maximum arguments to be support by static matchers
#ifndef ST_STATIC_EXPECT_CONFIG_NUM_MATCHER_ARGS
#define ST_STATIC_EXPECT_CONFIG_NUM_MATCHER_ARGS 5
#endif

#ifndef ST_STATIC_EXPECT_CONFIG_USE_NEWLINES_IN_MESSAGE
#  // VS converts \n in static_assert messages into actual new-lines in compiler output:
#  ifdef BOOST_MSVC
#    define ST_STATIC_EXPECT_CONFIG_USE_NEWLINES_IN_MESSAGE
#  endif
#endif


// Type arguments are enumerated with this prefix in the static failure and other error messages
#ifndef ST_STATIC_EXPECT_CONFIG_TYPEDISPLAY_PREFIX
#   define ST_STATIC_EXPECT_CONFIG_TYPEDISPLAY_PREFIX  Type
#endif

// Generate a static_assert upon failure (set to 0 if not needed; you can choose to keep the
// forged-error instead)
#ifndef ST_STATIC_EXPECT_CONFIG_STATIC_ASSERT
#  define ST_STATIC_EXPECT_CONFIG_STATIC_ASSERT 1
#endif

// Upon test failure, "forge" (fake out) a compiler error which would show the type argument as part
// of the resolved expression.
#ifndef ST_STATIC_EXPECT_CONFIG_FORGED_ERROR
#  define ST_STATIC_EXPECT_CONFIG_FORGED_ERROR 1
#endif

// An identifiable symbol that shows up in forged-errors
#ifndef ST_STATIC_EXPECT_CONFIG_FORGED_ERROR_SENTINEL
#   define ST_STATIC_EXPECT_CONFIG_FORGED_ERROR_SENTINEL Show_Me_The_Type
#endif

