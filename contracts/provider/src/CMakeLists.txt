project(provider)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( provider provider provider.cpp )
target_include_directories( provider PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( provider ${CMAKE_SOURCE_DIR}/../ricardian )