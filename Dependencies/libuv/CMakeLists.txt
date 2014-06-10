if(${WIN32})
  set(UV_SOURCES
    src/win/async.c
    src/win/core.c
    src/win/dl.c
    src/win/error.c
    src/win/fs-event.c
    src/win/fs.c
    src/win/getaddrinfo.c
    src/win/handle.c
    src/win/loop-watcher.c
    src/win/pipe.c
    src/win/poll.c
    src/win/process-stdio.c
    src/win/process.c
    src/win/req.c
    src/win/signal.c
    src/win/stream.c
    src/win/tcp.c
    src/win/thread.c
    src/win/threadpool.c
    src/win/timer.c
    src/win/tty.c
    src/win/udp.c
    src/win/util.c
    src/win/winapi.c
    src/win/winsock.c
  )
else()
  set(APPEND UV_SOURCES
    src/unix/async.c
    src/unix/core.c
    src/unix/dl.c
    src/unix/error.c
    src/unix/fs.c
    src/unix/getaddrinfo.c
    src/unix/loop.c
    src/unix/loop-watcher.c
    src/unix/pipe.c
    src/unix/poll.c
    src/unix/process.c
    src/unix/signal.c
    src/unix/stream.c
    src/unix/tcp.c
    src/unix/thread.c
    src/unix/threadpool.c
    src/unix/timer.c
    src/unix/tty.c
    src/unix/udp.c
  )

  set(UV_COMPILE_OPTIONS
    --std=c89
    -Wall
    -Wextra
    -Wno-unused-parameter
    -g
    -O3
  )

  set(UV_COMPILE_DEFINITIONS
    _LARGEFILE_SOURCE
    _FILE_OFFSET_BITS=64
  )

  set(UV_LINK_LIBRARIES
    "-lm"
    "-lpthread"
  )
endif()

list(APPEND UV_SOURCES
  src/fs-poll.c
  src/uv-common.c
  src/inet.c
  src/version.c
)

if(${CMAKE_SYSTEM_NAME} STREQUAL Darwin)
  list(APPEND UV_SOURCES
    src/unix/darwin.c
    src/unix/kqueue.c
    src/unix/fsevents.c
    src/unix/proctitle.c
    src/unix/darwin-proctitle.c
  )

  list(APPEND UV_COMPILE_OPTIONS
    -Wno-dollar-in-identifier-extension
  )

  list(APPEND UV_COMPILE_DEFINITIONS
    _DARWIN_USE_64_BIT_INODE=1
  )

  list(APPEND UV_LINK_LIBRARIES
    "-framework Foundation"
    "-framework CoreServices"
    "-framework ApplicationServices"
  )
endif()

add_library(uv STATIC ${UV_SOURCES})

target_include_directories(uv
  PRIVATE src include/uv-private
  PUBLIC include
)

target_compile_options(uv PRIVATE ${UV_COMPILE_OPTIONS})
target_compile_definitions(uv PRIVATE ${UV_COMPILE_DEFINITIONS})
target_link_libraries(uv ${UV_LINK_LIBRARIES})
