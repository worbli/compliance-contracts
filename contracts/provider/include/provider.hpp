
#include <eosiolib/eosio.hpp>
#include <compliance-common.hpp>

using namespace eosio;

CONTRACT provider : public contract
{
   public:
      using contract::contract;

      // change name to credential

      ACTION addentry(name account, name credential_code, std::string value);
      ACTION rmventry(name account, name credential_code);

      using addentry_action = action_wrapper<"addentry"_n, &provider::addentry>;
      using rmventry_action = action_wrapper<"rmventry"_n, &provider::addentry>;

      // test - remove
      ACTION test(name account);
      ACTION test2(name account);
};