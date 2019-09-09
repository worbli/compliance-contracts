project(worbli.reg)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( worbli.reg worbli.reg worbli.reg.cpp )
target_include_directories( worbli.reg PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( worbli.reg ${CMAKE_SOURCE_DIR}/../ricardian )