#include <eosiolib/eosio.hpp>
using namespace eosio;

#include <cmath>

namespace worbli_compliance
{

struct [[ eosio::table, eosio::contract("worbli.compliance") ]] credential_registry
{
  name credential_code;
  std::string description;
  uint64_t primary_key() const { return credential_code.value; }

  EOSLIB_SERIALIZE(credential_registry, (credential_code)(description))
};

typedef eosio::multi_index<name("credregistry"), credential_registry> credentials;

struct [[ eosio::table, eosio::contract("worbli.compliance") ]] provider_registry
{
  name provider;
  std::string description;
  std::vector<name> credentials;
  uint64_t primary_key() const { return provider.value; }

  EOSLIB_SERIALIZE(provider_registry, (provider)(description)(credentials))
};

/**
          * TODO: How do we handle providers that exit the business (an active flag?)
          */

typedef eosio::multi_index<name("provregistry"), provider_registry> providers;

struct [[ eosio::table, eosio::contract("provider") ]] verification
{
  name credential_code;
  bool value;
  uint64_t primary_key() const { return credential_code.value; }

  EOSLIB_SERIALIZE(verification, (credential_code)(value))
};

typedef eosio::multi_index<name("registry"), verification> registry;

/**
 *  All validation of credentials and providers are done during upd / add operations
 *  Each credential only needs to be satisifed by a single provider * 
 * */
bool validate(name account, std::vector<name> credential_codes, std::vector<name> providers )
{
  uint8_t passed = 0;

  for (name cred : credential_codes)
  {
    for (name provider : providers)
    {
      registry registry_table(provider, account.value);
      auto itr = registry_table.find(cred.value);
      if (itr != registry_table.end()) {
        passed++;
        break;
      }
    }
  }  
  return passed == credential_codes.size();
}

} // namespace worbli_compliance
