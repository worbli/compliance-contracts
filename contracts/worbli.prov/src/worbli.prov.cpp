#include <worbli.prov.hpp>

using namespace worblisystem;

ACTION prov::addentry( name account, name attribute_code, std::string value ) {
   require_auth(get_self());

   check( is_account(account), "account does not exist");

   // confirm provider supports credential
   // TODO: put regulator account in a const
   providers providers_table(regulator_account, regulator_account.value);
   auto prov_itr = providers_table.find(get_self().value);
   check( prov_itr != providers_table.end(), "provider not approved" );

   auto cred_itr = find(prov_itr->attributes.begin(), prov_itr->attributes.end(), attribute_code);
   check( cred_itr != prov_itr->attributes.end(), "credential not supported by provider" );

   registry registry_table(get_self(), account.value);
   auto reg_itr = registry_table.find(attribute_code.value);
   check( reg_itr == registry_table.end(), "account already has credential" );

   registry_table.emplace(get_self(), [&]( auto& rec ) {
          rec.attribute_code = attribute_code;
          rec.value = value;
   });
}

ACTION prov::updentry( name account, name attribute_code, std::string value ) {
   require_auth(get_self());

   check( is_account(account), "account does not exist");

   // confirm provider supports credential
   providers providers_table(regulator_account, regulator_account.value);
   auto prov_itr = providers_table.find(get_self().value);
   check( prov_itr != providers_table.end(), "provider not approved" );

   auto cred_itr = find(prov_itr->attributes.begin(), prov_itr->attributes.end(), attribute_code);
   check( cred_itr != prov_itr->attributes.end(), "attribute not supported by provider" );

   registry registry_table(get_self(), account.value);
   auto reg_itr = registry_table.find(attribute_code.value);
   check( reg_itr != registry_table.end(), "attribute not set for this account" );

   registry_table.modify( *reg_itr, same_payer, [&]( auto& rec ) {
      rec.value = value;
   });

}

ACTION prov::rmventry( name account, name credential_code ) {
}

EOSIO_DISPATCH( prov, (addentry)(updentry)(rmventry) )