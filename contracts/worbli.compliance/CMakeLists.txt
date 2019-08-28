add_contract(worbli.compliance worbli.compliance ${CMAKE_CURRENT_SOURCE_DIR}/src/worbli.compliance.cpp)

target_include_directories(worbli.compliance
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   ${CMAKE_CURRENT_SOURCE_DIR}/../worbli.compliance/include)

set_target_properties(worbli.compliance
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")