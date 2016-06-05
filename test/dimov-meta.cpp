//#define ST_STATIC_EXPECT_CONFIG_STATIC_ASSERT 0

#include <static_test/static_expect.hpp>

#include <boost/mpl/lambda.hpp>
#include <utility>


namespace mpl = boost::mpl;
//using namespace mpl::placeholders;
using namespace st::static_matchers;

// http://pdimov.com/cpp2/simple_cxx11_metaprogramming.html
template<class...> struct mp_list;

template <class T> using mp_add_pointer = T *;

template <class... T>
using mp_length = std::integral_constant<std::size_t, sizeof...(T)>;

//STATIC_EXPECT_THAT( (mp_length<int,double>), Eq<3>);

namespace detail {

template<class A, template<class...> class B> struct mp_rename;

template<template<class...> class A, class... T, template<class...> class B>
struct mp_rename<A<T...>, B>
{
    using type = B<T...>;
};
}


template<class A, template<class...> class B>
using mp_rename = typename detail::mp_rename<A, B>::type;


STATIC_EXPECT_THAT(
                   (mp_rename<std::pair<int, float>, std::tuple>),
                   (Is<std::tuple<int, float>>)
                   );




template<class L> struct mp_size_impl;

template<template<class...> class L,  class... T> struct mp_size_impl<L<T...>>
{
    using type = std::integral_constant<std::size_t, sizeof...(T)>;
};

template<class L> using mp_size = typename mp_size_impl<L>::type;

