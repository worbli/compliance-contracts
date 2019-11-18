#pragma once

#include <eosio/asset.hpp>
#include <math.h>
#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <eosio/singleton.hpp>
#include <eosio/transaction.hpp>
#include <string>

using namespace eosio;
using eosio::const_mem_fun;
using std::string;

struct [[eosio::table("configs"),eosio::contract("worbli.resource")]] config
{
  bool paused;
  uint32_t emadraglimit = 2;
  float allocated_cpu = 0.0;
  float allocated_net = 0.0;
  float allocated_total = 0.0;
  float unetpay = 0.0;
  bool open = false;
};
typedef singleton<"configs"_n, config> configs_singleton;

class [[eosio::contract("worbli.resource")]] resource : public contract
{
  using contract::contract;

private:
  configs_singleton _configs_singleton;
  config _config_state;

public:

  // constructor
  resource(name s, name code, datastream<const char *> ds);
  ~resource();
  // DISTRIBUTION functions
  ACTION settotal(name source, float total_cpu_quantity, float total_net_quantity, time_point_sec timestamp);
  ACTION adddistrib(name source, name account, float cpu_quantity, float net_quantity, time_point_sec timestamp);
  ACTION closedistrib(name source, time_point_sec timestamp);
  ACTION claimdistrib(name account);
  //***************************************************************************************************
  // CONFIGURATION functions
  ACTION updconfig(bool paused, uint32_t emadraglimit);
  ACTION addupdsource(name account, uint8_t in_out);
  //***************************************************************************************************

  // Init action
  ACTION init(time_point_sec start);

  TABLE daily_inflation
  {
    time_point_sec timestamp;
    asset utility_daily;
    asset bppay_daily;
    asset locking_daily;

    uint64_t primary_key() const { return (timestamp.sec_since_epoch()); }
  };
  typedef multi_index<"inflation"_n, daily_inflation> inflation_table;

  TABLE distribpay
  {
    name account; //Worbli account consuming the resource
    float payout; //WBI asset to pay for this period
    time_point_sec timestamp;
    uint64_t primary_key() const { return (account.value); }
  };
  typedef multi_index<"distribpays"_n, distribpay> distribpay_table;

  TABLE history
  {
    uint64_t history_id;
    uint32_t daycount;
    asset locked_total;
    float use_cpu;
    float use_net;
    float ma_cpu;
    float ma_net;
    float ema_cpu;
    float ema_net;
    float utility_daily;
    float bppay_daily;
    float locking_daily;
    float inflation_daily;
    float inflation;
    asset issue_amount;
    asset utility_tokens;
    asset bppay_tokens;
    asset locking_tokens;
    time_point_sec timestamp;
    uint64_t primary_key() const { return (history_id); }
  };
  typedef multi_index<"historys"_n, history> history_table;

  TABLE sourceauth
  {
    name account;
    uint64_t primary_key() const { return (account.value); }
  };
  typedef multi_index<"sourceauths"_n, sourceauth> sourceauth_table;

  //*****************************************
  //          HELPER FUNCTIONS
  //*****************************************

  bool paused()
  {
    return _config_state.paused;
  }

  bool is_source(name source)
  {
    sourceauth_table s_t(get_self(), get_self().value);
    auto itr = s_t.find(source.value);
    if (itr == s_t.end())
    {
      return false;
    }
    else
    {
      return true;
    }
  }

  // calculate a moving average
  float calcMA(float sum, uint8_t timeperiod, float newVal)
  {
    auto rslt = sum + newVal;
    return rslt / (timeperiod);
  }

  // calculate an exponential moving average
  float calcEMA(float previousAverage, int timePeriod, float newVal)
  {
    auto mult = 2.0 / (timePeriod + 1.0);
    auto rslt = (newVal - previousAverage) * mult + previousAverage;
    return rslt;
  }

  inline float get_c(float x)
  { // model C[x] = -x * ln(x) * exp(1)
    float p1 = -x;
    float p2 = log(float(x));
    float p3 = exp(1);
    return p1 * p2 * p3;
  }

  float round(float var)
  {
    //array of chars to store number as a string.
    char str[40];
    // Print in string the value of var with 4 decimal points
    sprintf(str, "%.4f", var);
    // scan string value in var
    sscanf(str, "%f", &var);
    return var;
  }
  //************************************************
  // EOSIO DELIVERED
  //************************************************

  TABLE account
  {
    asset balance;
    uint64_t primary_key() const { return balance.symbol.code().raw(); }
  };
  typedef multi_index<"accounts"_n, account> accounts;

  TABLE currency_stats
  {
    asset supply;
    asset max_supply;
    name issuer;

    uint64_t primary_key() const { return supply.symbol.code().raw(); }
  };
  typedef multi_index<"stat"_n, currency_stats> stats;

  void sub_balance(name owner, asset value)
  {
    accounts from_acnts(_self, owner.value);
    const auto &from = from_acnts.get(value.symbol.code().raw(), "no balance object found");
    check(from.balance.amount >= value.amount, "overdrawn balance");
    if (from.balance.amount == value.amount)
    {
      from_acnts.erase(from);
    }
    else
    {
      from_acnts.modify(from, owner, [&](auto &a) {
        a.balance -= value;
      });
    }
  }
  void add_balance(name owner, asset value, name ram_payer)
  {
    accounts to_acnts(_self, owner.value);
    auto to = to_acnts.find(value.symbol.code().raw());
    if (to == to_acnts.end())
    {
      to_acnts.emplace(ram_payer, [&](auto &a) {
        a.balance = value;
      });
    }
    else
    {
      to_acnts.modify(to, same_payer, [&](auto &a) {
        a.balance += value;
      });
    }
  }
};
/// namespace eosio
