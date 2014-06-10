cmake_minimum_required(VERSION 2.8)

add_subdirectory(Dependencies)

add_library(async STATIC
  Async/Buffer.cpp)

target_link_libraries(async PUBLIC uv)
target_include_directories(async PUBLIC .)

add_executable(runtests EXCLUDE_FROM_ALL
  Test/BufferTest.cpp
  Test/IntrusivePtrTest.cpp
)

target_link_libraries(runtests async gtest)

add_custom_target(test
  DEPENDS runtests
  COMMAND runtests
  COMMAND ${CMAKE_COMMAND} -E echo 'ALL TESTS PASSED'
)
