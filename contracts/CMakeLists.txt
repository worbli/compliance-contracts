cmake_minimum_required( VERSION 3.5 )

project(contracts)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_subdirectory(worbli.reg)
add_subdirectory(worbli.prov)
add_subdirectory(client)


