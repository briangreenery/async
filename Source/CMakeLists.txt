add_library(async STATIC
  Async/Buffer.cpp
  Async/Data.cpp
  Async/Error.cpp
  Async/EventEmitter.cpp
  Async/IntrusiveList.cpp
)

set(COMPILE_OPTIONS
  -fcolor-diagnostics
  -Wall
)

target_compile_options(async PRIVATE ${COMPILE_OPTIONS})
target_link_libraries(async PUBLIC uv)
target_include_directories(async PUBLIC .)
