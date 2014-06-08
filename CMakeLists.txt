cmake_minimum_required(VERSION 2.8)

add_subdirectory(Dependencies)
add_subdirectory(Test)

add_library(async STATIC
  Async/Buffer.cpp)

target_include_directories(async PUBLIC .)
