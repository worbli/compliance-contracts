add_contract(worbli.resource worbli.resource ${CMAKE_CURRENT_SOURCE_DIR}/src/worbli.resource.cpp)

target_include_directories(worbli.resource
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   ${CMAKE_CURRENT_SOURCE_DIR}/../worbli.resource/include)

set_target_properties(worbli.resource
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")