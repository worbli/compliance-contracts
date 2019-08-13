#include <provider.hpp>

using namespace worbli_compliance;

ACTION provider::addentry( name account, name credential_code, std::string value ) {
   require_auth(_self);

   // confirm account exists
   credentials credentials_table("providers"_n, "providers"_n.value);
   auto cred_itr = credentials_table.find(credential_code.value);
   print("credential: ", credential_code);
   check( cred_itr != credentials_table.end(), "credential not defined in registry" );

   registry registry_table(_self, account.value);
   auto reg_itr = registry_table.find(account.value);
   check( reg_itr == registry_table.end() , "Account already has credential" );

   registry_table.emplace(_self, [&]( auto& rec ) {
          rec.credential_code = credential_code;
          rec.value = true;
   });

}

ACTION provider::rmventry( name account, name credential_code ) {
   /* fill in action body */

}

// test - remove
ACTION provider::test( name account ) {

   REQUIREMENT_SET set1 = {
            {"provider1"_n, "provider2"_n},
            {"identity"_n, "kyc"_n}
   };

   REQUIREMENT_SET set2 = {
            {"provider1"_n, "provider2"_n},
            {"identity"_n, "kyc"_n}
   };

   
/**
   REQUIREMENT_SET* set;
   *set = {
            {"provider1"_n, "provider2"_n},
            {"identity"_n, "kyc"_n}
   };
   **/

   std::vector<REQUIREMENT_SET*> vec = {&set1, &set2};

   validate(account, vec);

}

// test - remove
ACTION provider::test2( name account ) {


}

EOSIO_DISPATCH( provider, (addentry)(rmventry)(test)(test2) )