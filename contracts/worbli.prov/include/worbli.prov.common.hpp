#include <eosio/eosio.hpp>
using namespace eosio;

namespace worblisystem
{
    using std::string;
    using std::vector;
    struct [[ eosio::table, eosio::contract("worbli.prov") ]] account_attribute
    {
        name name;
        string value;
        uint64_t primary_key() const { return name.value; }

        EOSLIB_SERIALIZE(account_attribute, (name)(value))
    };

    typedef eosio::multi_index<name("registry"), account_attribute> registry;

    struct condition
    {
        name attribute;
        vector<string> values;
    };

    inline vector<condition> validate(name provider, name account, vector<condition> conditions)
    {
        vector<condition> failed;

        for (condition condition : conditions)
        {
            registry registry_table(provider, account.value);
            auto itr = registry_table.find(condition.attribute.value);

            if (itr == registry_table.end() ||
                find(condition.values.begin(), condition.values.end(), itr->value) == condition.values.end())
            {
                failed.push_back(condition);
            }
        }
        return failed;
    }

    inline string getattribute(name provider, name account, name attribute, int64_t *value)
    {
        registry registry_table(provider, account.value);
        auto itr = registry_table.find(attribute.value);

        if (itr == registry_table.end()) {
            value = nullptr;
            return account.to_string() + " does not have attribute " + provider.to_string()
                    + ":" + attribute.to_string();
        } else {
            int64_t attr;
            std::stoi( itr->value );
            if (0) {
                value = nullptr;
                return provider.to_string() + ":" + attribute.to_string() + " is not an int64";
            }
            *value = attr;
            return "";
        }


    }


   static constexpr eosio::name regulator_account{"worbli.reg"_n};
   static constexpr eosio::name provider_account{"worbli.prov"_n};

} // namespace worblisystem