#include <eosio/eosio.hpp>
using namespace eosio;

class [[eosio::contract("worbli.reg")]] reg : public contract {
   public:
      using contract::contract;

      ACTION addcred( name credential_code, std::string description );
      ACTION updcred( name credential_code, std::string description );

      ACTION addprovider( name provider, std::string description );
      ACTION updprovider( name provider, std::string description );
      ACTION addprovcred( name provider, name credential_code );

      using addcred_action = action_wrapper<"addcred"_n, &reg::addcred>;
      using updcred_action = action_wrapper<"updcred"_n, &reg::updcred>;
      
      using addprovider_action = action_wrapper<"addprovider"_n, &reg::addprovider>;
      using updprovider_action = action_wrapper<"updprovider"_n, &reg::updprovider>;
      using addprovcred_action = action_wrapper<"addprovcred"_n, &reg::addprovcred>;

};