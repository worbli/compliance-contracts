#include <eosiolib/eosio.hpp>
#include <compliance-common.hpp>

using namespace eosio;
using std::vector;
using std::string;
using worbli_compliance::condition;

CONTRACT client : public contract {
   public:
      using contract::contract;

      ACTION booltest( name nm );
      string get_msg( vector<condition> conditions );

      using booltest_action = action_wrapper<"booltest"_n, &client::booltest>;
};