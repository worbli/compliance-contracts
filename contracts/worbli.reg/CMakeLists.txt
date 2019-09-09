add_contract(worbli.reg worbli.reg ${CMAKE_CURRENT_SOURCE_DIR}/src/worbli.reg.cpp)

target_include_directories(worbli.reg
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   ${CMAKE_CURRENT_SOURCE_DIR}/../worbli.reg/include)

set_target_properties(worbli.reg
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")