
#pragma once
#include <eosiolib/eosio.hpp>

using namespace eosio;

CONTRACT provider : public contract {
   public:
      using contract::contract;

      // change name to credential
      TABLE verification {
         name            credential_code; 
         bool            value; // TODO: we may not need this if we only have boolean values
         uint16_t primary_key()const { return credential_code.value; }

         EOSLIB_SERIALIZE( verification, (credential_code)(value) )
      };

      typedef eosio::multi_index<name("registry"), verification> registry;

      ACTION addentry( name account, name credential_code, std::string value );
      ACTION rmventry( name account, name credential_code );

      using addentry_action = action_wrapper<"addentry"_n, &provider::addentry>;
      using rmventry_action = action_wrapper<"rmventry"_n, &provider::addentry>;

      // test - remove 
      ACTION test( name account );
      ACTION test2( name account );
};