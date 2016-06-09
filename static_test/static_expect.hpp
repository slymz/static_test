/** 
  @copyright S. Levent Yilmaz 2016
  Distributed under the Boost Software License, Version 1.0.
  See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt.
*/

#pragma once

#include <static_test/config.hpp>
#include <static_test/pp/misc.hpp>
#include <static_test/pp/remove_paren.hpp>

#include <boost/utility/identity_type.hpp>
#include <boost/preprocessor/variadic.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/pop_back.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/dec.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/control/expr_iif.hpp>
#include <boost/preprocessor/for.hpp>

#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/accumulate.hpp>
#include <type_traits>


ST_CONFIG_NAMESPACE_OPEN

///
/**
STATIC_EXPECT_* test-macros provide compile time checks. The implementation is basically a
`static_assert`, with extra bells and whistles. One such bell is that it displays data-types upon
failure, especially useful for not only verification of your meta-programs, but also in debugging.

The arguments to the macros are one of:

-# Type
-# Meta-function
-# Integral Constant
-# Compile-time constant

Here is one that takes two types:

@code
   STATIC_EXPECT_SAME( type1, type2 );
@endcode

and it does what you expect, barfs if `std::is_same<type1, type2>::value` is not `true`.

There are macros for value comparison also, again, does what you expect:

@code
    int const one = 1;
    int const two = 2;
    int const three = 3;
    STATIC_EXPECT_EQ_C( two+one, three );
@endcode

The name of the macro has suffix `_C` to indicate that it works with compile-time constants.

In meta-programs there is the concept of an "Integral Constant", which is more useful than compile-time constants for type computations.
The non-suffix version of the macros are reserved for that purpose:

@code
    // Equivalent of the above using integral constants
    using One   = std::integral_constant<int, 1>;
    using Two   = std::integral_constant<int, 2>;
    using Three = std::integral_constant<int, 3>;

    STATIC_EXPECT_EQ( boost::mpl::plus<One,Two>,  Three );
@endcode

The more common integral constants are `std::true_type` and `std::false_type`, and that is what
most type-support meta-functions in the C++ standard library return:

@code
   STATIC_EXPECT_TRUE( std::is_unsigned<char> );
   STATIC_EXPECT_FALSE( std::is_integral<float> );

   double arr[5];
   STATIC_EXPECT_EQ( std::extent< decltype(arr) >,  5 );
@endcode

Finally, the meta-function

*/


//     +-------------------------+
//     |  Implementation detail  |
//     +-------------------------+


//     +---------------+
//     |  Test Macros  |
//     +---------------+

#define STATIC_EXPECT_TRUE_C( boolExpr )                          \
    ST_STATIC_EXPECT_assert( (boolExpr),               \
        ST_STATIC_EXPECT_quote_( #boolExpr ) " is false" \
)

#define STATIC_EXPECT_FALSE_C( boolExpr )                        \
    ST_STATIC_EXPECT_assert( !(boolExpr),             \
        ST_STATIC_EXPECT_quote_( #boolExpr ) " is true" \
)


#define STATIC_EXPECT_TRUE( boolConstant )                                     \
    ST_STATIC_EXPECT_assert(                                             \
       ST_PP_REMOVE_PAREN(boolConstant)::value,                               \
       ST_STATIC_EXPECT_quote_(#boolConstant) " is not true_type"        \
    )                                                                          \


#define STATIC_EXPECT_FALSE( boolConstant )                                     \
    ST_STATIC_EXPECT_assert(                                         \
       !BOOST_IDENTITY_TYPE((boolConstant))::value  ,                           \
       ST_STATIC_EXPECT_quote_(#boolConstant) " is not false_type"    \
)


#define STATIC_EXPECT_SAME( expectedType, actualType )                  \
    static_assert(                                                      \
        std::is_same                                                    \
        <  (expectedType)                                               \
        ,  (metaFunction)                                               \
        >::value,                                                       \
        "static failure: `"                                             \
        #expectedType "' and `" #actualType "' are not the same type"   \
        )                                                               \
        /**/

#define STATIC_EXPECT_DIFFERENT(expectedType, actualType )              \
    static_assert(                                                      \
        !std::is_same                                                   \
        <  (expectedType)                                               \
        ,  (metaFunction)                                               \
        >::value,                                                       \
        "static failure: `"                                             \
        #expectedType "' and `" #actualType "' are the same type"       \
        )                                                               \
        /**/


//     +-----------------------+
//     |  MetaFunction macros  |
//     +-----------------------+



#define STATIC_EXPECT_EVAL_SAME( expectedType,  nullaryMF )             \
    static_assert(                                                      \
        std::is_same                                                    \
        <  (expectedType)                                               \
        ,  st::detail::t_<nullaryMF>                                    \
        >::value,                                                       \
        "static failure: `"                                             \
        #expectedType "' is not what  `" #nullaryMF"' evaluates to."    \
        )                                                               \
        /**/

#define STATIC_EXPECT_EVAL_DIFFERENT( expectedType,  nullaryMF )        \
    static_assert(                                                      \
        !std::is_same                                                   \
        <  (expectedType)                                               \
        ,  st::detail::t_<nullaryMF>                                    \
        >::value,                                                       \
        "static failure: `"                                             \
        #expectedType "' is not what  `" #nullaryMF"' evaluates to."    \
        )                                                               \
        /**/


#define ST_STATIC_EXPECT_msg_false_type(x)  \
    ST_STATIC_EXPECT_msg( x " evaluates false" )

#define ST_STATIC_EXPECT_str_tplcall(x, ...) \
    BOOST_PP_STRINGIZE x "<" BOOST_PP_STRINGIZE (BOOST_PP_SEQ_ENUM( __VA_ARGS__ )) ">"

#define ST_STATIC_EXPECT_assert_true(x)                      \
    static_assert( x::value, ST_STATIC_EXPECT_msg( #x " evaluates false" ) )

#define ST_STATIC_EXPECT_assert_false(x)                      \
    static_assert( x::value, ST_STATIC_EXPECT_msg( x " evaluates true." ))




namespace detail {

template<class MetaFcn, class ... T>
struct InvokeMatcher
        : boost::mpl::apply<MetaFcn, T...>
          ::type // call meta-predicate
          ::type // become a bool_constant
{};

} // namespace detail



//     +-----------------------+
//     |  STATIC_EXPECT_THAT*  |
//     +-----------------------+

#define STATIC_EXPECT_THAT( ... )                                             \
     ST_StaticExpectThatImpl_do_(                                             \
        ST_StaticExpectThatImpl_ctor_( BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__), \
            ST_STATIC_EXPECT_THAT_idType_ )                                   \
)                                                                             \
/**/

#define STATIC_EXPECT_THAT_EXPR( ... )                                                            \
     ST_StaticExpectThatImpl_do_(                                                        \
        ST_StaticExpectThatImpl_ctor_( BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__), decltype ) \
     )                                                                                            \
/**/

//     +-----------------------------------------------+
//     |  Implementation of STATIC_EXPECT_THAT macros  |
//     +-----------------------------------------------+

// The implementation is using a "well known" programming technique: Preprocessor Object
// Oriented Programming, aka, POOP :)
//
// There is an implied class `StaticExpectThatImpl`. It has methods and a state. The state is a
// Boost.Preprocessor Tuple. The methods the consistently named ST_StaticExpectThatImpl_*
// macro-functions and take as argument this tuple, also consistently named `This'.
//
// There is a special method that creates the state, the "constructor". It is called from
// STATIC_EXPECT_THAT user macros. The arguments of the constructor are some wrapped form of
// the user arguments to these macros.


//===-  [Constructor]  -===//
// The "class" state is the following triplet:
// 1. The Matcher (last argument of STATIC_EXPECT_THAT macros)
// 2. The arguments "container" (remaining of the arguments to STATIC_EXPECT_THAT macros converted to a Boost.PP Sequence)
// 3. Specific to each STATIC_EXPECT_THAT macro, the "callable" which specifies how to convert an
//    argument to a type. Callable is some expression which is valid when followed by `(arg)`, where
//    `arg` is an element of the arguments container.
#define ST_StaticExpectThatImpl_initTuple_( userArgsSeq, arg2type )  ( \
    ST_PP_SEQ_BACK (userArgsSeq),                                      \
    BOOST_PP_SEQ_POP_BACK(userArgsSeq),                                \
    arg2type                                                           \
)                                                                      \
/**/

#define ST_StaticExpectThatImpl_ctor_( userArgsSeq, arg2type ) \
ST_StaticExpectThatImpl_initTuple_                             \
    BOOST_PP_IF(                                               \
       BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(userArgsSeq)),           \
       (userArgsSeq, arg2type),                                \
       (BOOST_PP_SEQ_HEAD(userArgsSeq), arg2type )             \
    )                                                          \
/**/


//===-  [Member accessors] -===//

// The Matcher
#define ST_StaticExpectThatImpl_matcher_( This ) BOOST_PP_TUPLE_ELEM(4, 0, This )

// The arguments container: Boost.Preprocessor sequence of arguments before matcher to STATIC_EXPECT_THAT macros
#define ST_StaticExpectThatImpl_args_( This )  BOOST_PP_TUPLE_ELEM(4, 1, This )

// The "callable" which converts the arguments to types (callable is C will be expanded to C(arg) for each argument)
#define ST_StaticExpectThatImpl_toType_( This ) BOOST_PP_TUPLE_ELEM(4, 2, This )


//===-  [Number of arguments]  -===//
#define ST_StaticExpectThatImpl_numArgs_(This)              \
   BOOST_PP_SEQ_SIZE( ST_StaticExpectThatImpl_args_(This) ) \
/**/

//===-  [Invoke Matcher]  -===//
#define ST_StaticExpectThatImpl_invokeMatcher_(This)                           \
  using matcherResult =                                                        \
      st::detail::InvokeMatcher</* remove parenthesis guarding Matcher with    \
                                   commas in it */                             \
                                ST_PP_REMOVE_PAREN(                            \
                                    ST_StaticExpectThatImpl_matcher_(This))    \
                                    ST_StaticExpectThatImpl_ttypeList_(This)>; \
/**/

//===-  [ typeList ]  -===//
// A comma separated list of each type, with a leading comma
#define ST_StaticExpectThatImpl_ttypeList_(This)                               \
  BOOST_PP_SEQ_FOR_EACH_I(ST_StaticExpectThatImpl_ttypeList_m_,                \
                          ST_StaticExpectThatImpl_toType_(This),               \
                          ST_StaticExpectThatImpl_args_(This))                 \
/**/
#define ST_StaticExpectThatImpl_ttypeList_m_(r, toType, i, arg) , toType(arg)

//===-  [Static assert]  -===//
// Expand to a static_assert
#if defined(ST_STATIC_EXPECT_CONFIG_STATIC_ASSERT) && (ST_STATIC_EXPECT_CONFIG_STATIC_ASSERT == 1)
#define ST_StaticExpectThatImpl_staticAssert_( This )  \
    ST_STATIC_EXPECT_assert_(                             \
        matcherResult::value,                                   \
        ST_StaticExpectThatImpl_assertMsg_(This)            \
    );                                                          
#else
#define ST_StaticExpectThatImpl_staticAssert_(This)
#endif
/**/


//===-  [Assert message]  -===//

// Generate a string that contains the list of arguments.
#define ST_StaticExpectThatImpl_assertMsg_(This)                                  \
    ST_PP_STRINGIZE(                                                                 \
      ST_PP_REMOVE_PAREN( ST_StaticExpectThatImpl_matcher_(This) )          \
        < BOOST_PP_ENUM_PARAMS(ST_StaticExpectThatImpl_numArgs_(This),       \
                               ST_STATIC_EXPECT_CONFIG_TYPEDISPLAY_PREFIX)      \
        >                                                                             \
        )                                                                             \
    " fails with:"                                                                    \
    BOOST_PP_SEQ_FOR_EACH_I(                                                          \
        ST_STATIC_EXPECT_listElem_m_,                                           \
        ~,                                                                            \
        ST_StaticExpectThatImpl_args_(This)                                  \
        )                                                                             \
/**/

// "List" spec per argument:
#ifdef ST_STATIC_EXPECT_CONFIG_USE_NEWLINES_IN_MESSAGE
#   define ST_STATIC_EXPECT_listElem_m_(r, _, i, arg) "\n" BOOST_PP_STRINGIZE(ST_STATIC_EXPECT_CONFIG_TYPEDISPLAY_PREFIX) #i ": " BOOST_PP_STRINGIZE(arg)
#else
#   define ST_STATIC_EXPECT_listElem_m_(r, _, i, arg) ", " BOOST_PP_STRINGIZE(ST_STATIC_EXPECT_CONFIG_TYPEDISPLAY_PREFIX) #i ": " BOOST_PP_STRINGIZE(arg)
#endif


//===-  [Forge Error]  -===//
// Forge (fake out) an error into the compiler such that the error message includes in clear-text
// the resolved type of the argument.
// The forged error is an invalid access to a non-existent member type.
#define ST_StaticExpectThatImpl_forgeError_(This) \
    BOOST_PP_SEQ_FOR_EACH_I_R(1,                      \
       ST_StaticExpectThatImpl_forgeError_m_,     \
       This,                                          \
       ST_StaticExpectThatImpl_args_(This)   \
)
/**/

#if 0
#define ST_StaticExpectThatImpl_forgeError_m_(r, This,  i, arg)                         \
   using t ## i =                                                                           \
      st::detail::extractForgedErrorSentinel<                                  \
          st::detail::                                                         \
          BOOST_PP_CAT(ST_STATIC_EXPECT_CONFIG_TYPEDISPLAY_PREFIX, i) <               \
              st::detail::t_<std::conditional<                                 \
                  matcherResult::value,                                                     \
                  st::detail::void_,  /* when-true */                          \
                  ST_StaticExpectThatImpl_toType_(This)(arg)  /* when-false */     \
              >> /* conditional select void_ or the type, when void_ no errors generated */ \
          > /* The type is passed as specialization of Ti for i-th argument */              \
      >; /* access the ::Sentinel to force the error */                                     \
/**/
#else
#define ST_StaticExpectThatImpl_forgeError_m_(r, This,  i, arg)                         \
   using t ## i =                                                                           \
          st::detail::                                                         \
          BOOST_PP_CAT(ST_STATIC_EXPECT_CONFIG_TYPEDISPLAY_PREFIX, i) <               \
              st::detail::t_<std::conditional<                                 \
                  matcherResult::value,                                                     \
                  st::detail::void_,  /* when-true */                          \
                  ST_StaticExpectThatImpl_toType_(This)(arg)  /* when-false */     \
              >> /* conditional select void_ or the type, when void_ no errors generated */ \
          > /* The type is passed as specialization of Ti for i-th argument */              \
      ::ST_STATIC_EXPECT_CONFIG_FORGED_ERROR_SENTINEL ; /* access the ::Sentinel to force the error */                   \
/**/

#endif

//===-  [do]  -===//

// Finally: the top level call that brings everything together
#define ST_StaticExpectThatImpl_do_(This)                            \
    struct BOOST_PP_CAT( _ST_test_static_expect_local_str_ , __LINE__ ) {    \
        ST_StaticExpectThatImpl_invokeMatcher_(This)                     \
        ST_StaticExpectThatImpl_staticAssert_(This)                       \
        ST_StaticExpectThatImpl_forgeError_(This)                         \
    }                                                                         \
/**/


//     +----------------------------+
//     |  "STATIC" macro functions  |
//     +----------------------------+

// The following are static equivalent, macro functions for tasks not specific to the

// Given a possibly comma-separated and parenthesized type expression `x`,
// convert to a type expression valid as a template argument
#define ST_STATIC_EXPECT_THAT_idType_(x) ST_PP_REMOVE_PAREN(x)
//
#define ST_STATIC_EXPECT_assert_(x , msg )     \
    static_assert( x, "static failure: " msg "." )   \
    /**/
#define ST_STATIC_EXPECT_quote_(x) "`" x "'"



namespace detail {

// a placeholder type to represent none of the user types
struct void_;

// Extract type member (aka, call nullary meta-function)
// This has the benefit of being independent of dependent-context, meaning, if it wasn't for the
// alias template, we'd have to know whether or not to use typename to reach for the ::type, which
// is required to be present and not-present for depenedent and non-dependent contexts, respectively.
template< class T >
using t_ = typename T::type;


// Similarly, get dependent-context independent sentinel type in forged error
template< class NullMF >
using extractForgedErrorSentinel = typename NullMF::ST_STATIC_EXPECT_CONFIG_FORGED_ERROR_SENTINEL;




// Generate T0, T1, T2, .. structs, which generates some artificial error
// Expands to  (repeat for T1, T2.. TN where N is ST_STATIC_EXPECT_CONFIG_NUM_MATCHER_ARGS):
//
//    template<class>
//    struct Type0 {
//        // when reached into for sentinel member, it will generate a failure
//    };
//    template<>
//    struct Type0<void_> {
//        // the actual type is unimportant, this is the specialization used when condition is true
//        // and no errors are generated
//        using Show_Me_The_Type = void;
//    };
//
#define ST_STATIC_EXPECT_CONFIG_FORGED_ERROR_TYPE 0
#if (ST_STATIC_EXPECT_CONFIG_FORGED_ERROR_TYPE == 1)

#define ST_STAMP_(z,i,_)                                                                \
   template<class>                                                                       \
   struct BOOST_PP_CAT(ST_STATIC_EXPECT_CONFIG_TYPEDISPLAY_PREFIX,i) {};           \
                                                                                         \
   template<>                                                                            \
   struct BOOST_PP_CAT(ST_STATIC_EXPECT_CONFIG_TYPEDISPLAY_PREFIX, i)< void_ > {   \
       using ST_STATIC_EXPECT_CONFIG_FORGED_ERROR_SENTINEL = void;                 \
   };                                                                                    \
/**/

#else

#define ST_STAMP_(z,i,_)                                                                \
template<class>                                                                       \
struct BOOST_PP_CAT(ST_STATIC_EXPECT_CONFIG_TYPEDISPLAY_PREFIX,i) { \
private: \
using ST_STATIC_EXPECT_CONFIG_FORGED_ERROR_SENTINEL = void; \
};           \
\
template<>                                                                            \
struct BOOST_PP_CAT(ST_STATIC_EXPECT_CONFIG_TYPEDISPLAY_PREFIX, i)< void_ > {   \
using ST_STATIC_EXPECT_CONFIG_FORGED_ERROR_SENTINEL = void;                 \
};                                                                                    \

#endif

BOOST_PP_REPEAT(ST_STATIC_EXPECT_CONFIG_NUM_MATCHER_ARGS, ST_STAMP_, ~)

#undef ST_STAMP_



} // namespace detail


namespace static_matchers_impl {

    //using namespace boost::mpl::placeholders;

template<class T>
struct Is {
    template<class Arg>
    struct apply
        : std::is_same<T, Arg>
    {};
};



template<class T>
struct EvalsTo {
    template<class Arg>
    struct apply
        : std::is_same<T, typename Arg::type>
    {};
};


namespace detail {

template<class T> struct Always {
    template<class...>
    struct apply {
        using type = T;
    };
};
}

using AlwaysFalse = detail::Always<std::false_type>;
using AlwaysTrue  = detail::Always<std::true_type>;


template<class MF>
struct Not {
    template<class... T>
    struct apply
        : std::integral_constant< bool,
           !boost::mpl::apply< MF, T...>::type::value
        >
    {};
};

template<class T>
using IsNot = Not<Is<T>>;

//
// struct AreSameType {
//     template<class...T>
//     struct apply {
//         using tlist = boost::mpl::vector<T...>;
//         using firstType = boost::mpl::front<tlist>;
//         using type = typename boost::mpl::fold < tlist, boost::mpl::true_,
//             boost::mpl::and_< _1, typename std::is_same<firstType, boost::mpl::_2>::type >
//         >::type
//     {};
// };



struct IsTrue {
    template<class T>
    struct apply : std::integral_constant<bool, T::value> {
    };
};



    template<int N>
    struct Eq
    {
        template<class T>
        struct apply : std::integral_constant<bool, static_cast<int>(T::value) == N>
        {};
    };

} // namespace static_matchers_impl

namespace static_matchers {
    using static_matchers_impl::Is;
    using static_matchers_impl::Eq;
}



//
// template<class T>
// struct Identityb
// {
//     template<class Arg>
//     struct apply
//         : boost::mpl::apply< T, Arg >

//     {
//     };
// };


ST_CONFIG_NAMESPACE_CLOSE