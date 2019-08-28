#include <test.hpp>
#include <compliance-common.hpp>

ACTION test::hi( name nm ) {
   /* fill in action body */
   std::vector<name> providers = { name("provider1"), name("provider2") };
   std::vector<name> creds = { name("kyc"), name("identity") };
   auto result = worbli_compliance::validate(nm, creds, providers );

   print_f("result: %", result);

   // std::string d = name("test").to_string();
   // std::string p = "this is a tring " + d;
   // check(false, p);

}