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
    std::string value;
    uint64_t primary_key() const { return credential_code.value; }

    EOSLIB_SERIALIZE(verification, (credential_code)(value))
  };

  typedef eosio::multi_index<name("registry"), verification> registry;

    struct condition {
      name credential_code;
      std::string value;
    };

    inline std::vector<condition> validate(name provider, name account, std::vector<condition> conditions)
    {
      std::vector<condition> failed;

      for (condition condition : conditions)
      {
        registry registry_table(provider, account.value);
        auto itr = registry_table.find(condition.credential_code.value);
        if (itr == registry_table.end() || itr->value != condition.value)
        {
          failed.push_back(condition);
        }
      }
      return failed;
    }

} // namespace worbli_compliance
