cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_CXX_FLAGS
  -fcolor-diagnostics
)

add_subdirectory(Dependencies)
add_subdirectory(Examples)
add_subdirectory(Source)
add_subdirectory(Test)
