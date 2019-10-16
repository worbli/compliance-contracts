#include <client.hpp>

ACTION client::test1( name nm ) {

   vector<condition> conditions_1 {
      condition{"kyc"_n, {"true"}},
      condition{"accredited"_n, {"true"}}
   };

   vector<condition> conditions_2 {
      condition{"country"_n, {"us","gb","de","jp"}}
   };

   auto result = worblisystem::validate(name("provider1"), nm, conditions_1);
   check(result.empty(), "producer 1 failed checks: " + get_msg(result));

   result = worblisystem::validate(name("provider2"), nm, conditions_2);
   check(result.empty(), "producer 2 failed checks: " + get_msg(result));

}

ACTION client::test2( name nm ) {

   vector<condition> conditions_1 {
      condition{"kyc"_n, {"true"}},
      condition{"country"_n, {"us","gb","de","jp"}}
   };

   auto result = worblisystem::validate(name("provider1"), nm, conditions_1);
   check(result.empty(), "producer 1 failed checks: " + get_msg(result));



}

string client::get_msg( vector<condition> conditions ) {
   string msg = "";
   for( condition condition : conditions ) {
      msg = msg + " " + condition.attribute.to_string();
   }
   return msg;
}