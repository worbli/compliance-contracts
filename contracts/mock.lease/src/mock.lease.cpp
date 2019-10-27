#include <mock.lease/mock.lease.hpp>

namespace eosio
{

void lease::update(name id, asset locked)
{
   require_auth(get_self());
   leasing_table l_t(get_self(), get_self().value);

   auto itr = l_t.find(id.value);
   if( itr != l_t.end() ) {
      l_t.modify(itr, same_payer, [&](auto &item) {
         item.locked = locked;
      });
   } else {
      l_t.emplace(get_self(), [&](auto &item) {
         item.id = id;
         item.locked = locked;
      });
   }
}

} // namespace eosio
