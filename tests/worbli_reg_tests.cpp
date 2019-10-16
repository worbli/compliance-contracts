#include "worbli_compliance_tester.hpp"

BOOST_AUTO_TEST_SUITE(worblicompliance_tests)

BOOST_FIXTURE_TEST_CASE( provider_management, worblicompliance_tester ) try {

   BOOST_REQUIRE_EQUAL( success(), add_credential( N(identity), "identity verified") );
   BOOST_REQUIRE_EQUAL( success(), add_credential( N(kyc), "kyc verified") );
   BOOST_REQUIRE_EQUAL( success(), add_credential( N(accredited), "accredited investor") );
   BOOST_REQUIRE_EQUAL( success(), add_credential( N(exchange), "exchange account") );
   BOOST_REQUIRE_EQUAL( success(), add_credential( N(country), "country") );

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
      add_entry( N(provider1), N(alice1), N(kyc), "true") );

   BOOST_REQUIRE_EQUAL( wasm_assert_msg( "attribute not supported by provider" ),
      add_entry( N(provider1), N(alice), N(aml), "true") );

   produce_blocks(1);

   BOOST_REQUIRE_EQUAL( success(), client_test1( N(alice) ));


} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
