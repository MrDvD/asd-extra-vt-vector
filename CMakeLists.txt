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

FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG origin/main
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(benchmark)

enable_testing()

add_executable(
  vt_tests
  src/vt/vt.hpp
  src/tests/module.cc
)
target_link_libraries(
  vt_tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(vt_tests)

add_executable(
  vt_bench
  src/vt/vt.hpp
  src/tests/bench.cc
)
target_link_libraries(
  vt_bench
  PRIVATE benchmark::benchmark
)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  set(
    CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} -Werror -fsanitize=undefined -fsanitize=address"
  )
  target_link_options(vt_tests
    BEFORE PUBLIC -fsanitize=undefined PUBLIC -fsanitize=address
  )
endif()