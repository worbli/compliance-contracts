add_contract(test test ${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp)

target_include_directories(test
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(test
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")