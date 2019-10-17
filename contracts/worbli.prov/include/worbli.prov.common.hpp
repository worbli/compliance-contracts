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

/**
    Returns an int value from a registry

    @param provider account hosting the provider contract.
    @param account account the attribute is associated to
    @param attribute attribute to lookup
    @return optional containg an int64_t.
            nullopt if error
            -1 if value doesn't exists
*/
    inline const std::optional<int64_t> getint(name provider, name account, name attribute)
    {
        registry registry_table(provider, account.value);
        auto itr = registry_table.find(attribute.value);

        if (itr != registry_table.end()) {
            print(itr->value);
            char *c = new char[itr->value.size() + 1];
            std::copy(itr->value.begin(), itr->value.end(), c);
            char* end;
            long number = std::strtol(c, &end, 0);
            if (*end == '\0')
                return std::optional<int64_t>{number};

            return std::nullopt;
        }

        return std::optional<int64_t>{-1};
    }

   static constexpr eosio::name regulator_account{"worbli.reg"_n};
   static constexpr eosio::name provider_account{"worbli.prov"_n};

} // namespace worblisystem