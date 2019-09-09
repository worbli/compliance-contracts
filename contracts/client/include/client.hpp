#include <eosiolib/eosio.hpp>
#include <worbli.reg.common.hpp>

using namespace eosio;
using std::vector;
using std::string;
using worbli_compliance::condition;

CONTRACT client : public contract {
   public:
      using contract::contract;

      ACTION test1( name nm );
      ACTION test2( name nm );
      string get_msg( vector<condition> conditions );

      using booltest_action = action_wrapper<"test1"_n, &client::test1>;
};