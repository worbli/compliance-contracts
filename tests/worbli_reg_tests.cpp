#include "worbli_compliance_tester.hpp"

BOOST_AUTO_TEST_SUITE(worblicompliance_tests)

BOOST_FIXTURE_TEST_CASE( provider_management, worblicompliance_tester ) try {

   BOOST_REQUIRE_EQUAL( success(), add_credential( N(identity), "identity verified", 0) );
   BOOST_REQUIRE_EQUAL( success(), add_credential( N(kyc), "kyc verified", 0) );
   BOOST_REQUIRE_EQUAL( success(), add_credential( N(accredited), "accredited investor", 0) );
   BOOST_REQUIRE_EQUAL( success(), add_credential( N(exchange), "exchange account", 0) );
   BOOST_REQUIRE_EQUAL( success(), add_credential( N(country), "country", 0) );

   // confirm credential validation
   BOOST_REQUIRE_EQUAL( wasm_assert_msg( "attribute does not exist" ),
      add_provider_credential( N(provider1), N(aml)));

   // confirm provider validation
   BOOST_REQUIRE_EQUAL( wasm_assert_msg( "provider does not exist" ),
      add_provider_credential( N(provider1), N(kyc)) );

   // add providers
   BOOST_REQUIRE_EQUAL( success(), add_provider( N(provider1), "provider 1") );
   BOOST_REQUIRE_EQUAL( success(), add_provider( N(provider2), "provider 2") );

   // add credentials supported by each provider
   BOOST_REQUIRE_EQUAL( success(), add_provider_credential( N(provider1), N(kyc)) );
   BOOST_REQUIRE_EQUAL( success(), add_provider_credential( N(provider1), N(accredited)) );
   BOOST_REQUIRE_EQUAL( success(), add_provider_credential( N(provider2), N(country)) );

   BOOST_REQUIRE_EQUAL( success(), add_entry( N(provider1), N(alice), N(kyc), "true") );
   BOOST_REQUIRE_EQUAL( success(), add_entry( N(provider1), N(alice), N(accredited), "true") );
   BOOST_REQUIRE_EQUAL( success(), add_entry( N(provider2), N(alice), N(country), "gb") );

   BOOST_REQUIRE_EQUAL( wasm_assert_msg( "attribute already defined for account" ),
      add_entry( N(provider1), N(alice), N(kyc), "true") );

   BOOST_REQUIRE_EQUAL( wasm_assert_msg( "provider not approved" ),
      add_entry( N(provider3), N(alice), N(kyc), "true") );

   BOOST_REQUIRE_EQUAL( wasm_assert_msg( "account does not exist" ),
      add_entry( N(provider1), N(alice12), N(kyc), "true") );

   BOOST_REQUIRE_EQUAL( wasm_assert_msg( "attribute not supported by provider" ),
      add_entry( N(provider1), N(alice), N(aml), "true") );

   produce_blocks(1);

   BOOST_REQUIRE_EQUAL( success(), client_test1( N(alice) ));

} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( test_client, worblicompliance_tester ) try {

   BOOST_REQUIRE_EQUAL( success(), add_credential( N(maxsubacct), "max subaccounts", 0) );
   BOOST_REQUIRE_EQUAL( success(), add_credential( N(kyc), "kyc verified", 0) );

   // add providers
   BOOST_REQUIRE_EQUAL( success(), add_provider( N(provider1), "provider 1") );

   // add credentials supported by each provider
   BOOST_REQUIRE_EQUAL( success(), add_provider_credential( N(provider1), N(maxsubacct)) );
   BOOST_REQUIRE_EQUAL( success(), add_provider_credential( N(provider1), N(kyc)) );

   BOOST_REQUIRE_EQUAL( success(), add_entry( N(provider1), N(alice), N(maxsubacct), "777") );
   BOOST_REQUIRE_EQUAL( success(), add_entry( N(provider1), N(bob), N(maxsubacct), "nan") );
   BOOST_REQUIRE_EQUAL( success(), add_entry( N(provider1), N(charlie), N(maxsubacct), "2.22") );

   produce_blocks(1);

   BOOST_REQUIRE_EQUAL( success(), client_testint( N(provider1), N(alice), N(maxsubacct), 777 ));

   // test text value
   BOOST_REQUIRE_EQUAL( wasm_assert_msg( "could not parse integer" ),
      client_testint( N(provider1), N(bob), N(maxsubacct), 2 ) );

   // test float value
   BOOST_REQUIRE_EQUAL( wasm_assert_msg( "could not parse integer" ),
      client_testint( N(provider1), N(charlie), N(maxsubacct), 2 ) );

   // test no attribute
   BOOST_REQUIRE_EQUAL( success(), client_testint( N(provider1), N(alice1), N(maxsubacct), -1 ));

} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
