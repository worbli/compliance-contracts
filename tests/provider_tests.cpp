#include "worbli_compliance_tester.hpp"

BOOST_AUTO_TEST_SUITE(provider_tests)

BOOST_FIXTURE_TEST_CASE( regulator_tests, worblicompliance_tester ) try {

   BOOST_REQUIRE_EQUAL( success(), add_credential( N(identity), "identity verified") );
   BOOST_REQUIRE_EQUAL( success(), add_credential( N(kyc), "kyc verified") );
   BOOST_REQUIRE_EQUAL( success(), add_credential( N(accredited), "accredited investor") );
   BOOST_REQUIRE_EQUAL( success(), add_credential( N(exchange), "exchange account") );

   // confirm credential validation
   BOOST_REQUIRE_EQUAL( wasm_assert_msg( "attribute does not exist" ),
      add_provider_credential( N(provider1), N(aml)));   

   produce_blocks(1);

} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
