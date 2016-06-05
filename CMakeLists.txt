cmake_minimum_required(VERSION 3.5)
project(static_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(${PROJECT_SOURCE_DIR})

#  +---------+
#  |  BOOST  |
#  +---------+

set(BOOST_ROOT ~/opt/boost/1.60)
find_package( Boost REQUIRED )
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

#  +---------------+
#  |  Google Test  |
#  +---------------+

find_library( GMOCK_LIB gmock HINTS ~/opt/gtest/lib )
find_library( GTEST_LIB gtest HINTS ~/opt/gtest/lib )
find_path( GTEST_INCLUDE gtest/gtest.h HINTS ~/opt/gtest/include )
include_directories( ${GTEST_INCLUDE} )

#  +------------+
#  |  Range-v3  |
#  +------------+

find_path( RANGEV3_INCLUDE meta/meta.hpp HINTS ~/pkg/range-v3/include )
include_directories( ${RANGEV3_INCLUDE} )


#  +--------------+
#  |  Test Subdir |
#  +--------------+

add_subdirectory(test)
