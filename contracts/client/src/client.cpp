#include <client.hpp>

ACTION client::test1( name account ) {

   vector<condition> conditions_1 {
      condition{"provider1"_n, "kyc"_n, {"true"}},
      condition{"provider1"_n, "accredited"_n, {"true"}}
   };

   vector<condition> conditions_2 {
      condition{"provider2"_n, "country"_n, {"us","gb","de","jp"}}
   };

   auto result = worblisystem::validate(account, conditions_1);
   check(result.empty(), "producer 1 failed checks: " + get_msg(result));

   result = worblisystem::validate(account, conditions_2);
   check(result.empty(), "producer 2 failed checks: " + get_msg(result));

}

ACTION client::testint( name provider, name account, name attribute, uint64_t value ) {

   auto result = worblisystem::getint(provider, account, attribute);
   check(result.has_value(), "could not parse integer");
   check(*result == value, "value is is not as expected");
}

string client::get_msg( vector<condition> conditions ) {
   string msg = "";
   for( condition condition : conditions ) {
      msg = msg + " " + condition.attribute.to_string();
   }
   return msg;
}