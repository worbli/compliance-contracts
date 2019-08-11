#include <eosiolib/eosio.hpp>
using namespace eosio;

namespace worbli_compliance {
   
   struct [[eosio::table, eosio::contract("worbli.compliance")]] credential_registry
   {
      name credential_code;
      std::string description;
      uint64_t primary_key() const { return credential_code.value; }

      EOSLIB_SERIALIZE(credential_registry, (credential_code)(description))
   };

   typedef eosio::multi_index<name("credregistry"), credential_registry> credentials;

   struct [[eosio::table, eosio::contract("worbli.compliance")]] provider_registry
   {
      name provider;
      std::string description;
      std::vector<name> credentials;
      uint64_t primary_key() const { return provider.value; }

      EOSLIB_SERIALIZE(provider_registry, (provider)(description)(credentials))
   };

   /**
          * TODO: How do we handle providers that exit the business (an active flag?)
          */

   typedef eosio::multi_index<name("provregistry"), provider_registry> providers;

}
