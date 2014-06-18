add_executable(runtests EXCLUDE_FROM_ALL
  BufferTest.cpp
  DataTest.cpp
  ErrorTest.cpp
  EventEmitterTest.cpp
  IntrusiveListTest.cpp
  IntrusivePtrTest.cpp
)

set(COMPILE_OPTIONS
  -fcolor-diagnostics
  -Wall
)

target_compile_options(runtests PRIVATE ${COMPILE_OPTIONS})
target_link_libraries(runtests async gtest)

add_custom_target(test
  DEPENDS runtests
  COMMAND runtests --gtest_color=yes
  COMMAND ${CMAKE_COMMAND} -E echo 'ALL TESTS PASSED'
)
