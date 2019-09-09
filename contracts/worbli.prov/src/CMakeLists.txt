project(worbli.prov)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( worbli.prov worbli.prov worbli.prov.cpp )
target_include_directories( worbli.prov 
                            PUBLIC 
                            ${CMAKE_SOURCE_DIR}/../include
                            ${CMAKE_CURRENT_SOURCE_DIR}/../../worbli.reg/include )
target_ricardian_directory( worbli.prov ${CMAKE_SOURCE_DIR}/../ricardian )