#include "worbli_resource_tester.hpp"

BOOST_AUTO_TEST_SUITE(worbliresource_tests)

BOOST_FIXTURE_TEST_CASE( test_average_calculations, worbli_resource_tester, * boost::unit_test::tolerance(1e+2) ) try {

   BOOST_REQUIRE_EQUAL( success(), addupdsource( N(worbli.admin), 1 ) );
   BOOST_REQUIRE_EQUAL( success(), updconfig( false, 1 ) );
   BOOST_REQUIRE_EQUAL( success(), init( "2019-09-18T00:00:00.000" ) );

   BOOST_REQUIRE_EQUAL( success(), setmetric( "2019-09-19T00:00:00.000", "10000.0000 WBI", "2500.0000 WBI", "100.0000 WBI" ) );
   BOOST_REQUIRE_EQUAL( success(), settotal("worbli.admin", 0.005, 0.005, "2019-09-19T00:00:00.000") );

   // test averages with 1 period: total usage 1%
   auto history = get_history(1);
   BOOST_REQUIRE_EQUAL( "0.00499999988824129", history["ma_cpu"] );
   BOOST_REQUIRE_EQUAL( "0.00499999988824129", history["ma_net"] );
   BOOST_REQUIRE_EQUAL( "0.00499999988824129", history["ema_cpu"] );
   BOOST_REQUIRE_EQUAL( "0.00499999988824129", history["ema_net"] );

   // test averages with 5 periods: total usage 10%
   BOOST_REQUIRE_EQUAL( success(), updconfig( false, 5 ) );
   BOOST_REQUIRE_EQUAL( success(), setmetric( "2019-09-20T00:00:00.000", "10000.0000 WBI", "2500.0000 WBI", "100.0000 WBI" ) );
   BOOST_REQUIRE_EQUAL( success(), setmetric( "2019-09-21T00:00:00.000", "10000.0000 WBI", "2500.0000 WBI", "100.0000 WBI" ) );
   BOOST_REQUIRE_EQUAL( success(), setmetric( "2019-09-22T00:00:00.000", "10000.0000 WBI", "2500.0000 WBI", "100.0000 WBI" ) );
   BOOST_REQUIRE_EQUAL( success(), setmetric( "2019-09-23T00:00:00.000", "10000.0000 WBI", "2500.0000 WBI", "100.0000 WBI" ) );
   BOOST_REQUIRE_EQUAL( success(), setmetric( "2019-09-24T00:00:00.000", "10000.0000 WBI", "2500.0000 WBI", "100.0000 WBI" ) );

   BOOST_REQUIRE_EQUAL( success(), settotal("worbli.admin", 0.05, 0.05, "2019-09-20T00:00:00.000") );
   BOOST_REQUIRE_EQUAL( success(), settotal("worbli.admin", 0.05, 0.05, "2019-09-21T00:00:00.000") );
   BOOST_REQUIRE_EQUAL( success(), settotal("worbli.admin", 0.05, 0.05, "2019-09-22T00:00:00.000") );
   BOOST_REQUIRE_EQUAL( success(), settotal("worbli.admin", 0.05, 0.05, "2019-09-23T00:00:00.000") );
   BOOST_REQUIRE_EQUAL( success(), settotal("worbli.admin", 0.05, 0.05, "2019-09-24T00:00:00.000") );

   history = get_history(6);

   BOOST_REQUIRE_EQUAL( "0.05000000074505806", history["ma_cpu"] );
   BOOST_REQUIRE_EQUAL( "0.05000000074505806", history["ma_net"] );

   BOOST_REQUIRE_EQUAL( success(), setmetric( "2019-09-25T00:00:00.000", "10000.0000 WBI", "2500.0000 WBI", "100.0000 WBI" ) );
   BOOST_REQUIRE_EQUAL( success(), settotal("worbli.admin", 0.05, 0.05, "2019-09-25T00:00:00.000") );

   history = get_history(7);

   BOOST_REQUIRE_EQUAL( "0.05000000074505806", history["ema_cpu"] );
   BOOST_REQUIRE_EQUAL( "0.05000000074505806", history["ema_net"] );

   auto inflation = get_inflation(6);
   std::cout << get_history(7) << std::endl;
   std::cout << inflation << std::endl;

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( test_resource_oracle, worbli_resource_tester ) try {

   // start date is January 1, 2020 12:00:54 AM

   BOOST_REQUIRE_EQUAL( success(), addupdsource( N(worbli.admin), 1 ) );
   BOOST_REQUIRE_EQUAL( success(), updconfig( false, 1 ) );
   BOOST_REQUIRE_EQUAL( success(), init( "2019-12-31T00:00:00.000" ) );
   
   BOOST_REQUIRE_EQUAL( success(), setmetric( "2020-01-01T00:00:00.000", "10000.0000 WBI", "2500.0000 WBI", "100.0000 WBI" ) );
   BOOST_REQUIRE_EQUAL( success(), settotal("worbli.admin", 0.005, 0.005, "2020-01-01T00:00:00.000") );

   BOOST_REQUIRE_EQUAL( success(), setmetric( "2020-01-02T00:00:00.000", "10000.0000 WBI", "2500.0000 WBI", "100.0000 WBI" ) );
   
   // future date should fail
   BOOST_REQUIRE_EQUAL( wasm_assert_msg( "cannot settotal for future date" ),
      settotal("worbli.admin", 0.005, 0.005, "2020-01-02T00:00:00.000") );

   BOOST_REQUIRE_EQUAL( success(), setmetric( "2019-12-15T00:00:00.000", "10000.0000 WBI", "2500.0000 WBI", "100.0000 WBI" ) );

   // past date should fail
   BOOST_REQUIRE_EQUAL( wasm_assert_msg( "invalid timestamp" ),
      settotal("worbli.admin", 0.005, 0.005, "2019-12-15T00:00:00.000") );

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( test_distributions, worbli_resource_tester ) try {
   BOOST_REQUIRE_EQUAL( success(), addupdsource( N(worbli.admin), 1 ) );
   BOOST_REQUIRE_EQUAL( success(), updconfig( false, 1 ) );
   BOOST_REQUIRE_EQUAL( success(), init( "2019-09-18T00:00:00.000" ) );

   BOOST_REQUIRE_EQUAL( success(), setmetric( "2019-09-19T00:00:00.000", "10000.0000 WBI", "2500.0000 WBI", "100.0000 WBI" ) );
   BOOST_REQUIRE_EQUAL( success(), settotal("worbli.admin", 0.005, 0.005, "2019-09-19T00:00:00.000") );


} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
