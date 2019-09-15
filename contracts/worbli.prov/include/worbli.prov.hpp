
#include <eosio/eosio.hpp>
#include <worbli.reg.common.hpp>

using namespace eosio;

class [[eosio::contract("worbli.prov")]] prov : public contract {
   public:
      using contract::contract;

      // change name to credential

      ACTION addentry(name account, name credential_code, std::string value);
      ACTION updentry( name account, name credential_code, std::string value );
      ACTION rmventry(name account, name credential_code);

      using addentry_action = action_wrapper<"addentry"_n, &prov::addentry>;
      using updentry_action = action_wrapper<"updentry"_n, &prov::updentry>;
      using rmventry_action = action_wrapper<"rmventry"_n, &prov::rmventry>;

};