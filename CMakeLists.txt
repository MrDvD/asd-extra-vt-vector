cmake_minimum_required(VERSION 3.14)
project(vt_vector)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
  GSL
  GIT_REPOSITORY https://github.com/microsoft/GSL.git
  GIT_TAG v4.2.1
  GIT_SHALLOW ON
)
FetchContent_MakeAvailable(GSL)

enable_testing()

add_executable(
  vt
  src/tests/vt.cc
  src/vt/vt.hpp
)
target_link_libraries(
  vt
  PRIVATE Microsoft.GSL::GSL
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(vt)