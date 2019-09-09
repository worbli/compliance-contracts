#include <client.hpp>

ACTION client::booltest( name nm ) {

   vector<condition> conditions_1 {
      condition{"kyc"_n, "true"},
      condition{"accredited"_n, "true"}
   };

   auto result = worbli_compliance::validate(name("provider1"), nm, conditions_1);
   check(result.empty(), "failed checks: " + get_msg(result));
}

string client::get_msg( vector<condition> conditions ) {
   string msg = "";
   for( condition condition : conditions ) {
      msg = msg + " " + condition.credential_code.to_string();
   }
   return msg;
}