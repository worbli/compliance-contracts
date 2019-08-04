#include <eosiolib/eosio.hpp>
using namespace eosio;

CONTRACT verification_registry : public contract {
   public:
      using contract::contract;

      TABLE credential_registry {
         name            credential_code; 
         std::string     description;
         uint64_t primary_key()const { return credential_code.value; }

         EOSLIB_SERIALIZE( credential_registry, (credential_code)(description) )
      };

      typedef eosio::multi_index<name("credregistry"), credential_registry> credentials;

      TABLE provider_registry {
         name               provider; 
         std::string        description;
         std::vector<name>  credentials;
         uint64_t primary_key()const { return provider.value; }

         EOSLIB_SERIALIZE( provider_registry, (provider)(description)(credentials) )
      };

      /**
       * TODO: How do we handle providers that exit the business (an active flag?)
       */

      typedef eosio::multi_index<name("provregistry"), provider_registry> providers;

      ACTION addcred( name credential_code, std::string description );
      ACTION updcred( name credential_code, std::string description );

      ACTION addprovider( name provider, std::string description );
      ACTION updprovider( name provider, std::string description );
      ACTION addprovcred( name provider, name credential_code );

      using addcred_action = action_wrapper<"addcred"_n, &verification_registry::addcred>;
      using updcred_action = action_wrapper<"updcred"_n, &verification_registry::updcred>;
      using addprovider_action = action_wrapper<"addprovider"_n, &verification_registry::addprovider>;
      using updprovider_action = action_wrapper<"updprovider"_n, &verification_registry::updprovider>;
      using addprovcred_action = action_wrapper<"addprovcred"_n, &verification_registry::addprovcred>;

};