project(client)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( client client client.cpp )
target_include_directories( client PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( client ${CMAKE_SOURCE_DIR}/../ricardian )