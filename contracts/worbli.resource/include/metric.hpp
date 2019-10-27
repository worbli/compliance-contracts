#pragma once

#include <eosio/asset.hpp>
#include <math.h>
#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <eosio/singleton.hpp>
#include <eosio/transaction.hpp>
#include <string>

#include "worbli.resource.hpp"

using namespace eosio;

struct metric
{
  eosio::time_point_sec timestamp;
  asset wbi_supply;
  asset wbi_locked;
  uint64_t primary_key() const { return (timestamp.sec_since_epoch()); }
};
typedef eosio::multi_index<"metrics"_n, metric> metrics_table;
