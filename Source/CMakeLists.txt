add_library(async STATIC
  Async/Buffer.cpp
  Async/Data.cpp
  Async/DataPipe.cpp
  Async/Error.cpp
  Async/EventEmitter.cpp
  Async/IntrusiveList.cpp
  Async/UVHandle.cpp
  Async/UVStream.cpp
  Async/UVStreamReader.cpp
  Async/UVStreamWriter.cpp
  Async/UVTCPSocket.cpp
  Async/UVTTY.cpp
)

set(COMPILE_OPTIONS
  -fcolor-diagnostics
  -Wall
)

target_compile_options(async PRIVATE ${COMPILE_OPTIONS})
target_link_libraries(async PUBLIC uv)
target_include_directories(async PUBLIC .)
