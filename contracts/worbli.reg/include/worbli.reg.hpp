#include <eosio/eosio.hpp>
using namespace eosio;

class [[eosio::contract("worbli.reg")]] reg : public contract {
   public:
      using contract::contract;

      ACTION addattribute( name attribute, std::string description, uint8_t type );
      ACTION updattribute( name attribute, std::string description, uint8_t type );

      ACTION addprovider( name provider, std::string description );
      ACTION updprovider( name provider, std::string description );
      ACTION addprovattr( name provider, name attribute );

      using addcred_action = action_wrapper<"addattribute"_n, &reg::addattribute>;
      using updcred_action = action_wrapper<"updattribute"_n, &reg::updattribute>;
      
      using addprovider_action = action_wrapper<"addprovider"_n, &reg::addprovider>;
      using updprovider_action = action_wrapper<"updprovider"_n, &reg::updprovider>;
      using addprovattr_action = action_wrapper<"addprovattr"_n, &reg::addprovattr>;

};