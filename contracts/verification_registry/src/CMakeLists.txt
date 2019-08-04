project(verification_registry)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( verification_registry verification_registry verification_registry.cpp )
target_include_directories( verification_registry PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( verification_registry ${CMAKE_SOURCE_DIR}/../ricardian )