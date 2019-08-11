
#pragma once
#include <eosiolib/eosio.hpp>

using namespace eosio;

struct [[eosio::table, eosio::contract("provider")]] verification
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