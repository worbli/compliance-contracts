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

class worblicompliance_tester : public tester {
public:

   worblicompliance_tester() {
      basic_setup();
      create_core_token();

      produce_blocks( 2 );

      create_accounts( { N(worbli.reg), N(provider1), N(provider2), N(provider3), N(client),
                         N(alice), N(bob)} );
      produce_blocks( 2 );

      set_code( N(worbli.reg), contracts::worbli_reg_wasm() );
      set_abi( N(worbli.reg), contracts::worbli_reg_abi().data() );
      produce_blocks();
      {
         const auto& accnt = control->db().get<account_object,by_name>( N(worbli.reg) );
         abi_def abi;
         BOOST_REQUIRE_EQUAL(abi_serializer::to_abi(accnt.abi, abi), true);
         abi_ser.set_abi(abi, abi_serializer_max_time);
      }

      set_code( N(provider1), contracts::worbli_prov_wasm() );
      set_abi( N(provider1), contracts::worbli_prov_abi().data() );
      produce_blocks();
      {
         const auto& accnt = control->db().get<account_object,by_name>( N(provider1) );
         abi_def abi;
         BOOST_REQUIRE_EQUAL(abi_serializer::to_abi(accnt.abi, abi), true);
         provider_abi_ser.set_abi(abi, abi_serializer_max_time);
      }

      set_code( N(provider2), contracts::worbli_prov_wasm() );
      set_abi( N(provider2), contracts::worbli_prov_abi().data() );
      produce_blocks();

      set_code( N(provider3), contracts::worbli_prov_wasm() );
      set_abi( N(provider3), contracts::worbli_prov_abi().data() );
      produce_blocks();
   
      set_code( N(client), contracts::client_wasm() );
      set_abi( N(client), contracts::client_abi().data() );
      produce_blocks();
      {
         const auto& accnt = control->db().get<account_object,by_name>( N(client) );
         abi_def abi;
         BOOST_REQUIRE_EQUAL(abi_serializer::to_abi(accnt.abi, abi), true);
         client_abi_ser.set_abi(abi, abi_serializer_max_time);
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

   action_result push_action_client( const account_name& signer, const account_name& contract, const action_name &name, const variant_object &data ) {
      string action_type_name = client_abi_ser.get_action_type(name);

      action act;
      act.account = contract;
      act.name    = name;
      act.data    = client_abi_ser.variant_to_binary( action_type_name, data,abi_serializer_max_time );

      return base_tester::push_action( std::move(act), uint64_t(signer));
   }

   action_result push_action_provider( const account_name& signer, const account_name& contract, const action_name &name, const variant_object &data ) {
      string action_type_name = provider_abi_ser.get_action_type(name);

      action act;
      act.account = contract;
      act.name    = name;
      act.data    = provider_abi_ser.variant_to_binary( action_type_name, data,abi_serializer_max_time );

      return base_tester::push_action( std::move(act), uint64_t(signer));
   }

   fc::variant get_account( account_name acc, const string& symbolname)
   {
      auto symb = eosio::chain::symbol::from_string(symbolname);
      auto symbol_code = symb.to_symbol_code().value;
      vector<char> data = get_row_by_account( N(eosio.token), acc, N(accounts), symbol_code );
      return data.empty() ? fc::variant() : abi_ser.binary_to_variant( "account", data, abi_serializer_max_time );
   }


   action_result add_provider( account_name provider, string description ) {
      return push_action( N(worbli.reg), N(worbli.reg), N(addprovider), mvo()
           ( "provider", provider )
           ( "description", description )
      );
   }

   action_result update_provider( account_name provider, string description ) {
      return push_action( N(worbli.reg), N(worbli.reg), N(updprovider), mvo()
           ( "provider", provider )
           ( "description", description )
      );
   }

   action_result add_provider_credential( account_name provider, name attribute ) {
      return push_action( N(worbli.reg), N(worbli.reg), N(addprovattr), mvo()
           ( "provider", provider )
           ( "attribute", attribute )
      );
   }

   action_result add_credential( account_name attribute, string description ) {
      return push_action( N(worbli.reg), N(worbli.reg), N(addattribute), mvo()
           ( "attribute", attribute )
           ( "description", description )
      );
   }

   action_result add_entry( account_name provider, account_name account, 
                            account_name attribute, string value ) {
      return push_action_provider( provider, provider, N(addentry), mvo()
           ( "account", account )
           ( "attribute", attribute )
           ( "value", value )
      );
   }

   action_result client_test1( account_name account ) {
      return push_action_client( account, N(client), N(test1), mvo()
           ( "nm", account )
      );
   }

   fc::variant get_stats( const string& symbolname ) {
      auto symb = eosio::chain::symbol::from_string(symbolname);
      auto symbol_code = symb.to_symbol_code().value;
      vector<char> data = get_row_by_account( N(eosio.token), symbol_code, N(stat), symbol_code );
      return data.empty() ? fc::variant() : token_abi_ser.binary_to_variant( "currency_stats", data, abi_serializer_max_time );
   }

   asset get_token_supply() {
      return get_stats("4," CORE_SYM_NAME)["supply"].as<asset>();
   }

   fc::variant get_condition( name condition ) {
      vector<char> data = get_row_by_account( N(founders), N(founders), N(conditions), condition );
      return data.empty() ? fc::variant() : abi_ser.binary_to_variant( "condition", data, abi_serializer_max_time );
   }

   fc::variant get_recipient( name owner ) {
      vector<char> data = get_row_by_account( N(founders), N(founders), N(recipients), owner );
      return data.empty() ? fc::variant() : abi_ser.binary_to_variant( "recipient", data, abi_serializer_max_time );
   }

   fc::variant get_variable( name var ) {
      vector<char> data = get_row_by_account( N(founders), N(founders), N(variables), var );
      return data.empty() ? fc::variant() : abi_ser.binary_to_variant( "variable", data, abi_serializer_max_time );
   }

   uint32_t last_block_time() const {
      return time_point_sec( control->head_block_time() ).sec_since_epoch();
   }

   abi_serializer abi_ser;
   abi_serializer client_abi_ser;
   abi_serializer provider_abi_ser;
   abi_serializer token_abi_ser;
};