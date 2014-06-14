add_library(async STATIC
  Async/Buffer.cpp
  Async/Data.cpp
)

target_link_libraries(async PUBLIC uv)
target_include_directories(async PUBLIC .)
