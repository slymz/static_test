/** 
  @copyright S. Levent Yilmaz 2016
  Distributed under the Boost Software License, Version 1.0.
  See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt.
*/

#pragma once


///@file
/// The missing bits from the Boost Preprocessor library

#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/dec.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/comma_if.hpp>
#include <boost/preprocessor/expr_if.hpp>


#ifndef BOOST_PP_VARIADICS
# error "Usage of ST_PP facility requires variadic macro support"
#endif


/// Returns the last element of the seq
/// E.g. `ST_PP_SEQ_BACK( (1)(2)(3) )` will be `3`
#define ST_PP_SEQ_BACK( seq ) \
    BOOST_PP_SEQ_ELEM(BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(seq)), seq)


/// Stringize variable number of arguments, including the comma
/// E.g. `ST_PP_STRINGIZE( a,b,c )` becomes  `"a,b,c"`
#define ST_PP_STRINGIZE(...) \
   BOOST_PP_SEQ_FOR_EACH_I( ST_PP_STRINGIZE_m_, ~ , BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__) )

/// The fast re-entrant version of ST_PP_STRINGIZE (if you need to use this efficiently in a pp-loop context)
#define ST_PP_STRINGIZE_R(r, ...) \
   BOOST_PP_SEQ_FOR_EACH_I_R(r, ST_PP_STRINGIZE_m_, ~ , BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__) )

#define ST_PP_STRINGIZE_m_(r,_,i,elem) BOOST_PP_EXPR_IF(i, "," ) BOOST_PP_STRINGIZE(elem)


/// Returns the arguments verbatim, including any commas
/// E.g. `ST_PP_IDENTITY( pair<int,double> )` expands to `pair<int,double>`
#define ST_PP_IDENTITY(...) __VA_ARGS__

