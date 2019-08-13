#include <eosiolib/eosio.hpp>
using namespace eosio;

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
  bool value; // TODO: we may not need this if we only have boolean values
  uint16_t primary_key() const { return credential_code.value; }

  EOSLIB_SERIALIZE(verification, (credential_code)(value))
};

typedef eosio::multi_index<name("registry"), verification> registry;

struct REQUIREMENT_SET
{
  std::vector<name> providers;
  std::vector<name> credential_codes;
};

// test non existing provider
// TODO: check worbli.contract that provider is approved for each credential_code.
bool validate_requirement_set(name account, std::vector<name> providers, std::vector<name> credential_codes)
{
  for (name provider : providers)
  {
    registry registry_table(provider, account.value);
    for (name code : credential_codes)
    {
      auto itr = registry_table.find(code.value);
      if (itr == registry_table.end())
        print_f("true");
      return true;
    }
  }
  print_f("false");
  return false;
}

// test non existing provider
bool validate(name account, std::vector<REQUIREMENT_SET *> requirements)
{
  for (auto requirement : requirements)
  {
    for (auto provider : requirement->providers)
    {
      validate_requirement_set(account, requirement->providers, requirement->credential_codes);
    }
  }
  return false;
}

} // namespace worbli_compliance
