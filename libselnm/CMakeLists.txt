add_library(libselnm INTERFACE)
set_target_properties(libselnm PROPERTIES PUBLIC_HEADER matrix.hpp)
target_include_directories(libselnm INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
  )
install(TARGETS libselnm
  PUBLIC_HEADER
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libselnm
  )

add_executable(matrix.test matrix.test.cpp)
target_link_libraries(matrix.test libselnm gtest gtest_main)
add_test(matrix.test matrix.test)
