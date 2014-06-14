macro(add_example name file)
  add_executable(${name} EXCLUDE_FROM_ALL ${file})
  target_link_libraries(${name} async)
  list(APPEND EXAMPLES ${name})
endmacro()

add_example(netcat Netcat.cpp)

add_custom_target(examples DEPENDS ${EXAMPLES})
