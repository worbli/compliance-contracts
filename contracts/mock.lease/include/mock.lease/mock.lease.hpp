#pragma once

#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>

#include <string>

namespace eosiosystem {
   class system_contract;
}

namespace eosio {

   using std::string;

   class [[eosio::contract("mock.lease")]] lease : public contract {
      public:
         using contract::contract;

         [[eosio::action]]
         void update( name id, asset locked );
         
      private:

         struct [[eosio::table]] lease_info
         {
            name  id;
            asset locked;

            uint64_t primary_key() const { return (id.value); }
         };
         typedef eosio::multi_index<"leasing"_n, lease_info> leasing_table;

   };
   
} /// namespace eosio
