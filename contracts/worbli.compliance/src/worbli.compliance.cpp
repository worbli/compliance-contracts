#include <worbli.compliance.hpp>
#include <compliance-common.hpp>

using namespace worbli_compliance;

ACTION compliance::addcred( name credential_code, std::string description ) {
   require_auth(_self);

   credentials credentials_table(_self, _self.value);
   auto cred_itr = credentials_table.find(credential_code.value);
   check( cred_itr == credentials_table.end() , "credential already exists" );

   credentials_table.emplace(_self, [&]( auto& rec ) {
          rec.credential_code = credential_code;
          rec.description = description;
   });
}

ACTION compliance::updcred( name credential_code, std::string description ) {
   require_auth(_self);

   credentials credentials_table(_self, _self.value);
   auto cred_itr = credentials_table.find(credential_code.value);
   check( cred_itr != credentials_table.end() , "credential does not exist" );

   credentials_table.modify( *cred_itr, _self, [&]( auto& rec ) {
          rec.description = description;
   });
}

ACTION compliance::addprovider( name provider, std::string description ) {
   require_auth(get_self());

   providers providers_table(get_self(), get_self().value);
   auto prov_itr = providers_table.find(provider.value);
   check( prov_itr == providers_table.end() , "provider already exists" );

   providers_table.emplace(get_self(), [&]( auto& rec ) {
          rec.provider = provider;
          rec.description = description;
   });
}

ACTION compliance::updprovider( name provider, std::string description ) {
   require_auth(get_self());

   providers providers_table(get_self(), _self.value);
   auto prov_itr = providers_table.find(provider.value);
   check( prov_itr != providers_table.end() , "provider does not exist" );

   providers_table.modify( *prov_itr, same_payer, [&]( auto& rec ) {
          rec.description = description;
   });

}

/** *
 * TODO: how do we handle a provider that stops performing a particular validation after being active
*/
ACTION compliance::addprovcred( name provider, name credential_code ) {
   require_auth(get_self());

   // confirm credential code has been defined
   credentials credentials_table(get_self(), get_self().value);
   auto cred_itr = credentials_table.find(credential_code.value);
   check( cred_itr != credentials_table.end() , "credential does not exist" );

   providers providers_table(get_self(), get_self().value);
   auto prov_itr = providers_table.find(provider.value);
   check( prov_itr != providers_table.end() , "provider does not exist" );

   auto check_itr = find( prov_itr->credentials.begin(), prov_itr->credentials.end(), credential_code);
   check( check_itr == prov_itr->credentials.end(), "provider already supports credential" );

   providers_table.modify( *prov_itr, get_self(), [&]( auto& rec ) {
        rec.credentials.emplace_back(credential_code);
   });
}

EOSIO_DISPATCH( compliance, (addcred)(updcred)(addprovider)(updprovider)(addprovcred) )