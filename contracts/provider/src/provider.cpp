#include <provider.hpp>

using namespace worbli_compliance;

ACTION provider::addentry( name account, name credential_code, std::string value ) {
   require_auth(get_self());

   check( is_account(account), "account does not exist");

   // confirm provider supports credential
   providers providers_table("regulator"_n, "regulator"_n.value);
   auto prov_itr = providers_table.find(get_self().value);
   check( prov_itr != providers_table.end(), "provider not approved" );

   auto cred_itr = find(prov_itr->credentials.begin(), prov_itr->credentials.end(), credential_code);
   check( cred_itr != prov_itr->credentials.end(), "credential not supported by provider" );

   registry registry_table(get_self(), account.value);
   auto reg_itr = registry_table.find(credential_code.value);
   check( reg_itr == registry_table.end(), "account already has credential" );

   registry_table.emplace(get_self(), [&]( auto& rec ) {
          rec.credential_code = credential_code;
          rec.value = value;
   });

}

ACTION provider::rmventry( name account, name credential_code ) {
   /* fill in action body */

}


ACTION provider::test( name account ) {}

/**
   REQUIREMENT_SET set1 = {
            {"provider1"_n, "provider2"_n},
            {"identity"_n, "kyc"_n}
   };
   REQUIREMENT_SET set2 = {
            {"provider1"_n, "provider2"_n},
            {"identity"_n, "kyc"_n}
   };

   std::vector<REQUIREMENT_SET*> vec = {&set1, &set2};
   validate(account, vec);

}
**/
// test - remove
ACTION provider::test2( name account ) {
}

EOSIO_DISPATCH( provider, (addentry)(rmventry)(test)(test2) )