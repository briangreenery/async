add_executable(runtests EXCLUDE_FROM_ALL
  BufferTest.cpp
  DataTest.cpp
  ErrorTest.cpp
  EventEmitterTest.cpp
  IntrusivePtrTest.cpp
)

target_link_libraries(runtests async gtest)

add_custom_target(test
  DEPENDS runtests
  COMMAND runtests
  COMMAND ${CMAKE_COMMAND} -E echo 'ALL TESTS PASSED'
)
