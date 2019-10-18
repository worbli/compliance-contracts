#include <eosio/eosio.hpp>
using namespace eosio;

#include <cmath>

namespace worblisystem
{

  using std::string;
  using std::vector;
  using std::find;

  struct [[ eosio::table, eosio::contract("worbli.reg") ]] attribute
  {
    name name;
    uint32_t type;
    string description;
    uint64_t primary_key() const { return name.value; }

    EOSLIB_SERIALIZE(attribute, (name)(type)(description))
  };

  typedef eosio::multi_index<name("attributes"), attribute> attributes;

  struct [[ eosio::table, eosio::contract("worbli.reg") ]] provider
  {
    name account;
    string description;
    vector<name> attributes;
    uint64_t primary_key() const { return account.value; }

    EOSLIB_SERIALIZE(provider, (account)(description)(attributes))
  };

  /**
   * TODO: How do we handle providers that exit the business (an active flag?)
  */

  typedef eosio::multi_index<name("providers"), provider> providers;

} // namespace worbli_compliance