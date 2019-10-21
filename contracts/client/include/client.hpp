#include <eosio/eosio.hpp>
#include <worbli.prov.common.hpp>

using namespace eosio;
using std::vector;
using std::string;
using worblisystem::condition;

CONTRACT client : public contract {
   public:
      using contract::contract;

      ACTION test1( name account );
      ACTION testint( name provider, name account, name attribute, uint64_t value );
      string get_msg( vector<condition> conditions );

      using booltest_action = action_wrapper<"test1"_n, &client::test1>;
};