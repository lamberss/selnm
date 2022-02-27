add_library(libselnm INTERFACE)
set_target_properties(libselnm PROPERTIES PUBLIC_HEADER "mapped_list.hpp;matrix.hpp")
target_include_directories(libselnm INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
  )
install(TARGETS libselnm
  PUBLIC_HEADER
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libselnm
  )

foreach(_name mapped_list matrix)
  add_executable(${_name}.test ${_name}.test.cpp)
  target_link_libraries(${_name}.test libselnm gtest gtest_main)
  add_test(${_name}.test ${_name}.test)
endforeach()