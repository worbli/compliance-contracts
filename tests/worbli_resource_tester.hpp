#include <boost/test/unit_test.hpp>
#include <eosio/testing/tester.hpp>
#include <eosio/chain/abi_serializer.hpp>
#include "eosio.system_tester.hpp"

#include "Runtime/Runtime.h"

#include <fc/variant_object.hpp>

using namespace eosio::testing;
using namespace eosio;
using namespace eosio::chain;
using namespace eosio::testing;
using namespace fc;
using namespace std;

using mvo = fc::mutable_variant_object;

class worbli_resource_tester : public tester {
public:

   worbli_resource_tester() {
      basic_setup();
      create_core_token();

      produce_blocks( 2 );

      create_accounts( { N(resource), N(alice), N(bob)} );
      produce_blocks( 2 );

      set_code( N(resource), contracts::worbli_resource_wasm() );
      set_abi( N(resource), contracts::worbli_resource_abi().data() );
      produce_blocks();
      {
         const auto& accnt = control->db().get<account_object,by_name>( N(resource) );
         abi_def abi;
         BOOST_REQUIRE_EQUAL(abi_serializer::to_abi(accnt.abi, abi), true);
         abi_ser.set_abi(abi, abi_serializer_max_time);
      }

   }

   void basic_setup() {
      produce_blocks( 2 );

      create_accounts({ N(eosio.token), N(worbli.admin)});

      produce_blocks( 100 );
      set_code( N(eosio.token), contracts::util::token_wasm());
      set_abi( N(eosio.token), contracts::util::token_abi().data() );
      {
         const auto& accnt = control->db().get<account_object,by_name>( N(eosio.token) );
         abi_def abi;
         BOOST_REQUIRE_EQUAL(abi_serializer::to_abi(accnt.abi, abi), true);
         token_abi_ser.set_abi(abi, abi_serializer_max_time);
      }
   }

   void create_core_token( symbol core_symbol = symbol{CORE_SYM} ) {
      FC_ASSERT( core_symbol.precision() != 4, "create_core_token assumes precision of core token is 4" );
      create_currency( N(eosio.token), config::system_account_name, asset(100000000000000, core_symbol) );
      issue(config::system_account_name, asset(10000000000000, core_symbol) );
      BOOST_REQUIRE_EQUAL( asset(10000000000000, core_symbol), get_balance( "eosio", core_symbol ) );
   }

   asset get_balance( const account_name& act, symbol balance_symbol = symbol{CORE_SYM} ) {
      vector<char> data = get_row_by_account( N(eosio.token), act, N(accounts), balance_symbol.to_symbol_code().value );
      return data.empty() ? asset(0, balance_symbol) : token_abi_ser.binary_to_variant("account", data, abi_serializer_max_time)["balance"].as<asset>();
   }

   void issue( name to, const asset& amount, name manager = config::system_account_name ) {
      base_tester::push_action( N(eosio.token), N(issue), manager, mutable_variant_object()
                                ("to",      to )
                                ("quantity", amount )
                                ("memo", "")
                                );
   }

   void transfer( account_name from,
                  account_name to,
                  asset        quantity,
                  string       memo ) {
      base_tester::push_action( N(eosio.token), N(transfer), config::system_account_name, mutable_variant_object()
           ( "from", from)
           ( "to", to)
           ( "quantity", quantity)
           ( "memo", memo)
      );
   }


   void create_currency( name contract, name manager, asset maxsupply ) {
      auto act =  mutable_variant_object()
         ("issuer",       manager )
         ("maximum_supply", maxsupply );

      base_tester::push_action(contract, N(create), contract, act );
   }

   action_result push_action( const account_name& signer, const account_name& contract, const action_name &name, const variant_object &data ) {
      string action_type_name = abi_ser.get_action_type(name);

      action act;
      act.account = contract;
      act.name    = name;
      act.data    = abi_ser.variant_to_binary( action_type_name, data,abi_serializer_max_time );

      return base_tester::push_action( std::move(act), uint64_t(signer));
   }


   fc::variant get_account( account_name acc, const string& symbolname)
   {
      auto symb = eosio::chain::symbol::from_string(symbolname);
      auto symbol_code = symb.to_symbol_code().value;
      vector<char> data = get_row_by_account( N(eosio.token), acc, N(accounts), symbol_code );
      return data.empty() ? fc::variant() : abi_ser.binary_to_variant( "account", data, abi_serializer_max_time );
   }

   action_result addupdsource( account_name account, uint8_t in_out ) {
      return push_action( N(resource), N(resource), N(addupdsource), mvo()
           ( "account", account )
           ( "in_out", in_out )
      );
   }

   action_result updconfig( bool paused, uint32_t emadraglimit ) {
      return push_action( N(resource), N(resource), N(updconfig), mvo()
           ( "paused", paused )
           ( "emadraglimit", emadraglimit )
      );
   }

   action_result init( string start ) {
      return push_action( N(resource), N(resource), N(init), mvo()
           ( "start", start )
      );
   }

   action_result setmetric(string timestamp, string wbi_supply, string wbi_locked, string wbi_locked_unused) {
      return push_action( N(resource), N(resource), N(setmetric), mvo()
           ( "timestamp", timestamp )
           ( "wbi_supply", wbi_supply )
           ( "wbi_locked", wbi_locked )
           ( "wbi_locked_unused", wbi_locked_unused )
      );
   }

   action_result settotal(name source, float total_cpu_quantity, float total_net_quantity, string timestamp) {
      return push_action( N(worbli.admin), N(resource), N(settotal), mvo()
           ( "source", source )
           ( "total_cpu_quantity", total_cpu_quantity )
           ( "total_net_quantity", total_net_quantity )
           ( "timestamp", timestamp )
      );
   }   

   fc::variant get_stats( const string& symbolname ) {
      auto symb = eosio::chain::symbol::from_string(symbolname);
      auto symbol_code = symb.to_symbol_code().value;
      vector<char> data = get_row_by_account( N(eosio.token), symbol_code, N(stat), symbol_code );
      return data.empty() ? fc::variant() : token_abi_ser.binary_to_variant( "currency_stats", data, abi_serializer_max_time );
   }

   fc::variant get_history(uint64_t index) {
      vector<char> data = get_row_by_account( N(resource), N(resource), N(historys), name(index) );
      return data.empty() ? fc::variant() : abi_ser.binary_to_variant( "history", data, abi_serializer_max_time );
   }

   asset get_token_supply() {
      return get_stats("4," CORE_SYM_NAME)["supply"].as<asset>();
   }

   uint32_t last_block_time() const {
      return time_point_sec( control->head_block_time() ).sec_since_epoch();
   }

   abi_serializer abi_ser;
   abi_serializer token_abi_ser;
};