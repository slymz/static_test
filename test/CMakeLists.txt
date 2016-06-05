file(GLOB testSrc *.cpp)
add_executable( positive-test ${testSrc} )

target_link_libraries( positive-test ${GTEST_LIB} ${GMOCK_LIB} )
#  target_link_libraries(hanatut.exe ${Boost_LIBRARIES})

