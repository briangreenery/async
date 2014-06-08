add_executable(runtests EXCLUDE_FROM_ALL
  IntrusivePtrTest.cpp
)

target_link_libraries(runtests async gtest)

add_custom_target(test
  DEPENDS runtests
  COMMAND runtests
  COMMAND ${CMAKE_COMMAND} -E echo 'ALL TESTS PASSED'
)
