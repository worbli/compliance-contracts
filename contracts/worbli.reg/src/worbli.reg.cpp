#include <worbli.reg.hpp>
#include <worbli.reg.common.hpp>

using namespace worblisystem;

ACTION reg::addcred( name attribute_code, std::string description ) {
   require_auth(_self);

   attributes attributes_table(_self, _self.value);
   auto attr_itr = attributes_table.find(attribute_code.value);
   check( attr_itr == attributes_table.end() , "attribute already exists" );

   attributes_table.emplace(_self, [&]( auto& rec ) {
          rec.attribute_code = attribute_code;
          rec.description = description;
   });
}

ACTION reg::updcred( name attribute_code, std::string description ) {
   require_auth(_self);

   attributes attributes_table(_self, _self.value);
   auto attr_itr = attributes_table.find(attribute_code.value);
   check( attr_itr != attributes_table.end() , "attribute does not exist" );

   attributes_table.modify( *attr_itr, _self, [&]( auto& rec ) {
          rec.description = description;
   });
}

ACTION reg::addprovider( name provider, std::string description ) {
   require_auth(get_self());

   providers providers_table(get_self(), get_self().value);
   auto prov_itr = providers_table.find(provider.value);
   check( prov_itr == providers_table.end() , "provider already exists" );

   providers_table.emplace(get_self(), [&]( auto& rec ) {
          rec.account = provider;
          rec.description = description;
   });
}

ACTION reg::updprovider( name provider, std::string description ) {
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
ACTION reg::addprovcred( name provider, name attribute_code ) {
   require_auth(get_self());

   // confirm credential code has been defined
   attributes attributes_table(get_self(), get_self().value);
   auto attr_itr = attributes_table.find(attribute_code.value);
   check( attr_itr != attributes_table.end() , "credential does not exist" );

   providers providers_table(get_self(), get_self().value);
   auto prov_itr = providers_table.find(provider.value);
   check( prov_itr != providers_table.end() , "provider does not exist" );

   auto prov_attr_itr = find( prov_itr->attributes.begin(), prov_itr->attributes.end(), attribute_code);
   check( prov_attr_itr == prov_itr->attributes.end(), "provider already supports attribute" );

   providers_table.modify( *prov_itr, get_self(), [&]( auto& rec ) {
        rec.attributes.emplace_back(attribute_code);
   });
}

EOSIO_DISPATCH( reg, (addcred)(updcred)(addprovider)(updprovider)(addprovcred) )