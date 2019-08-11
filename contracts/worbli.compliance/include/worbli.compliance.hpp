#include <eosiolib/eosio.hpp>
using namespace eosio;

class [[eosio::contract("worbli.compliance")]] compliance : public contract {
   public:
      using contract::contract;

      ACTION addcred( name credential_code, std::string description );
      ACTION updcred( name credential_code, std::string description );

      ACTION addprovider( name provider, std::string description );
      ACTION updprovider( name provider, std::string description );
      ACTION addprovcred( name provider, name credential_code );

      using addcred_action = action_wrapper<"addcred"_n, &compliance::addcred>;
      using updcred_action = action_wrapper<"updcred"_n, &compliance::updcred>;
      using addprovider_action = action_wrapper<"addprovider"_n, &compliance::addprovider>;
      using updprovider_action = action_wrapper<"updprovider"_n, &compliance::updprovider>;
      using addprovcred_action = action_wrapper<"addprovcred"_n, &compliance::addprovcred>;

};