#include <eosio/eosio.hpp>
using namespace eosio;

namespace worblisystem
{
    using std::string;
    using std::vector;
    struct [[ eosio::table, eosio::contract("worbli.prov") ]] account_attribute
    {
        name attribute_code;
        string value;
        uint64_t primary_key() const { return attribute_code.value; }

        EOSLIB_SERIALIZE(account_attribute, (attribute_code)(value))
    };

    typedef eosio::multi_index<name("registry"), account_attribute> registry;

    struct condition
    {
        name credential_code;
        vector<string> values;
    };

    inline vector<condition> validate(name provider, name account, vector<condition> conditions)
    {
        vector<condition> failed;

        for (condition condition : conditions)
        {
            registry registry_table(provider, account.value);
            auto itr = registry_table.find(condition.credential_code.value);

            if (itr == registry_table.end() ||
                find(condition.values.begin(), condition.values.end(), itr->value) == condition.values.end())
            {
                failed.push_back(condition);
            }
        }
        return failed;
    }

} // namespace worblisystem