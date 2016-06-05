#include <static_test/pp/remove_paren.hpp>

#include <vector>
#include <type_traits>

// Test in a context where parenthesis would be a syntax error
// E.g. template parameter
//   using t = std::vector<  int >;  // correct
//   using t = std::vector< (int) >; // syntax error

// No comma, no paren
using T1 = std::vector< ST_PP_REMOVE_PAREN(int) >;
static_assert( std::is_same< T1, std::vector<int> >::value , "t1" );

// Multi comma, no paren
using T2 = std::vector< ST_PP_REMOVE_PAREN(std::pair<int,double>) >;
static_assert(std::is_same< T2, std::vector<std::pair<int,double>> >::value, "t2");

// No comma, multi paren
using T3 = std::vector< ST_PP_REMOVE_PAREN((((int)))) >;
static_assert(std::is_same< T3, std::vector<int> >::value, "t3");

// Multi comma, multi paren
using T4 = std::vector< ST_PP_REMOVE_PAREN(((std::pair<int, double>))) >;
static_assert(std::is_same< T4, std::vector<std::pair<int, double>> >::value, "t4");
