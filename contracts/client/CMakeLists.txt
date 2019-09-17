add_contract(client client ${CMAKE_CURRENT_SOURCE_DIR}/src/client.cpp)

target_include_directories(client
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   ${CMAKE_CURRENT_SOURCE_DIR}/../worbli.prov/include)

set_target_properties(client
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")