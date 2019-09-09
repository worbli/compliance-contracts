add_contract(worbli.prov worbli.prov ${CMAKE_CURRENT_SOURCE_DIR}/src/worbli.prov.cpp)

target_include_directories(worbli.prov
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   ${CMAKE_CURRENT_SOURCE_DIR}/../worbli.reg/include)

set_target_properties(worbli.prov
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")