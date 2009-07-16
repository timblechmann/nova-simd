set(tests
  simd_math_tests.cpp
  simd_tests.cpp
  softclip_test.cpp
)

# build benchmarks
foreach(test ${tests})
  string(REPLACE .cpp "" test_name ${test} )
  add_executable(${test_name} ${test})
  add_test(${test_name}_run ${EXECUTABLE_OUTPUT_PATH}/${test_name})
endforeach(test)
