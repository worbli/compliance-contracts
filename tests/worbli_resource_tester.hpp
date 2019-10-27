#include <boost/test/unit_test.hpp>
#include <eosio/testing/tester.hpp>
#include <eosio/chain/abi_serializer.hpp>
#include "contracts.hpp"
#include "test_symbol.hpp"
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

      create_accounts( { N(worbli.admin), N(eosio.token), N(eosio.stake), N(eosio.usage),
                        N(eosio.saving), N(eosio.ppay), N(eosio.rex), N(eosio.ram), 
                        N(resource), N(user1), N(user2), N(user3), N(user4), N(user5), N(leasing)} );
      produce_blocks( 2 );

      basic_setup();
      create_core_token();

      produce_blocks( 2 );

      set_code( config::system_account_name, contracts::util::system_wasm() );
      set_abi( config::system_account_name, contracts::util::system_abi().data() );
      if( true ) {
         base_tester::push_action(config::system_account_name, N(init),
                                               config::system_account_name,  mutable_variant_object()
                                               ("version", 0)
                                               ("core", "4,TST")
         );
      }
      {
         const auto& accnt = control->db().get<account_object,by_name>( config::system_account_name );
         abi_def abi;
         BOOST_REQUIRE_EQUAL(abi_serializer::to_abi(accnt.abi, abi), true);
         abi_ser.set_abi(abi, abi_serializer_max_time);
      }

      set_code( N(resource), contracts::worbli_resource_wasm() );
      set_abi( N(resource), contracts::worbli_resource_abi().data() );
      produce_blocks(2);
      {
         const auto& accnt = control->db().get<account_object,by_name>( N(resource) );
         abi_def abi;
         BOOST_REQUIRE_EQUAL(abi_serializer::to_abi(accnt.abi, abi), true);
         resource_abi_ser.set_abi(abi, abi_serializer_max_time);
      }

      set_code( N(leasing), contracts::mock_lease_wasm() );
      set_abi( N(leasing), contracts::mock_lease_abi().data() );
      produce_blocks(2);
      {
         const auto& accnt = control->db().get<account_object,by_name>( N(leasing) );
         abi_def abi;
         BOOST_REQUIRE_EQUAL(abi_serializer::to_abi(accnt.abi, abi), true);
         mock_abi_ser.set_abi(abi, abi_serializer_max_time);
      }
      produce_blocks( 2 );
   }

   void basic_setup() {
      produce_blocks( 2 );

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

   transaction_trace_ptr create_account_with_resources( account_name a, account_name creator, uint32_t ram_bytes = 8000 ) {
      signed_transaction trx;
      set_transaction_headers(trx);

      authority owner_auth;
      owner_auth =  authority( get_public_key( a, "owner" ) );

      trx.actions.emplace_back( vector<permission_level>{{creator,config::active_name}},
                                newaccount{
                                   .creator  = creator,
                                   .name     = a,
                                   .owner    = owner_auth,
                                   .active   = authority( get_public_key( a, "active" ) )
                                });

      trx.actions.emplace_back( get_action( config::system_account_name, N(buyrambytes), vector<permission_level>{{creator,config::active_name}},
                                            mvo()
                                            ("payer", creator)
                                            ("receiver", a)
                                            ("bytes", ram_bytes) )
                              );

      trx.actions.emplace_back( get_action( config::system_account_name, N(delegatebw), vector<permission_level>{{creator,config::active_name}},
                                            mvo()
                                            ("from", creator)
                                            ("receiver", a)
                                            ("stake_net_quantity", core_sym::from_string("10.0000") )
                                            ("stake_cpu_quantity", core_sym::from_string("10.0000") )
                                            ("transfer", 0 )
                                          )
                                );

      set_transaction_headers(trx);
      trx.sign( get_private_key( creator, "active" ), control->get_chain_id()  );
      return push_transaction( trx );
   }

   action_result regproducer( const account_name& acnt, int params_fixture = 1 ) {
      action_result r =  push_action( N(worbli.admin), config::system_account_name, N(addproducer), mvo()
                          ("producer",  acnt )
      );
      BOOST_REQUIRE_EQUAL( success(), r);
      r = push_action( acnt, config::system_account_name, N(regproducer), mvo()
                          ("producer",  acnt )
                          ("producer_key", get_public_key( acnt, "active" ) )
                          ("url", "" )
                          ("location", 0 )
      );
      BOOST_REQUIRE_EQUAL( success(), r);
      return r;
   }

   void activate_chain() {
      push_action( N(eosio), config::system_account_name, N(togglesched), mvo()
                          ("is_active",  1 )
      ); 
   }

   action_result setwgstate( string last_inflation_print, string last_metric_read ) {
      return push_action( config::system_account_name, config::system_account_name, N(setwgstate), mvo()
           ( "last_inflation_print", last_inflation_print )
           ( "last_metric_read", last_metric_read )
      );
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

   action_result setmetric(string timestamp, string wbi_supply, string wbi_locked) {
      return push_action( N(resource), N(resource), N(setmetric), mvo()
           ( "timestamp", timestamp )
           ( "wbi_supply", wbi_supply )
           ( "wbi_locked", wbi_locked )
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

   fc::variant get_inflation(uint64_t timestamp) {
      vector<char> data = get_row_by_account( N(resource), N(resource), N(inflation), timestamp );
      return data.empty() ? fc::variant() : abi_ser.binary_to_variant( "daily_inflation", data, abi_serializer_max_time );
   }

   asset get_token_supply() {
      return get_stats("4," CORE_SYM_NAME)["supply"].as<asset>();
   }

   uint32_t last_block_time() const {
      return time_point_sec( control->head_block_time() ).sec_since_epoch();
   }

   abi_serializer abi_ser;
   abi_serializer token_abi_ser;
   abi_serializer mock_abi_ser;
   abi_serializer resource_abi_ser;

};