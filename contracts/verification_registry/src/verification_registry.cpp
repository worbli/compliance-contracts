#include <verification_registry.hpp>

ACTION verification_registry::addcred( name credential_code, std::string description ) {
   require_auth(_self);

   credentials credentials_table(_self, _self.value);
   auto cred_itr = credentials_table.find(credential_code.value);
   check( cred_itr == credentials_table.end() , "credential already exists" );

   credentials_table.emplace(_self, [&]( auto& rec ) {
          rec.credential_code = credential_code;
          rec.description = description;
   });
}

ACTION verification_registry::updcred( name credential_code, std::string description ) {
   require_auth(_self);

   credentials credentials_table(_self, _self.value);
   auto cred_itr = credentials_table.find(credential_code.value);
   check( cred_itr != credentials_table.end() , "credential does not exist" );

   credentials_table.modify( *cred_itr, _self, [&]( auto& rec ) {
          rec.description = description;
   });
}

ACTION verification_registry::addprovider( name provider, std::string description ) {
   require_auth(_self);

   providers providers_table(_self, _self.value);
   auto prov_itr = providers_table.find(provider.value);
   check( prov_itr == providers_table.end() , "provider already exists" );

   providers_table.emplace(_self, [&]( auto& rec ) {
          rec.provider = provider;
          rec.description = description;
   });
}

ACTION verification_registry::updprovider( name provider, std::string description ) {
   require_auth(_self);

   providers providers_table(_self, _self.value);
   auto prov_itr = providers_table.find(provider.value);
   check( prov_itr != providers_table.end() , "provider does not exist" );

   providers_table.modify( *prov_itr, _self, [&]( auto& rec ) {
          rec.description = description;
   });

}

/** *
 * TODO: how do we handle a provider that stops performing a particular validation after being active
*/
ACTION verification_registry::addprovcred( name provider, name credential_code ) {
   require_auth(_self);

   providers providers_table(_self, _self.value);
   auto prov_itr = providers_table.find(provider.value);
   check( prov_itr != providers_table.end() , "provider does not exist" );

   providers_table.modify( *prov_itr, _self, [&]( auto& rec ) {
        rec.credentials.emplace_back(credential_code);
   });
}

EOSIO_DISPATCH( verification_registry, (addcred)(updcred)(addprovider)(updprovider)(addprovcred) )