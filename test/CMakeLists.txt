### Tests

include(../cmake/deps-test.cmake)

macro (make_test test_name)
  add_executable(test_${test_name} ${ARGN})
  target_link_libraries(test_${test_name} PRIVATE scope_exit ${LIBRARIES} ${TEST_LIBRARIES})
  add_test(NAME test_${test_name} COMMAND test_${test_name})
endmacro ()

make_test(scope_exit
  scope_exit.t.cpp)
