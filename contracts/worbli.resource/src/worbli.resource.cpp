#include "worbli.resource.hpp"
#include <math.h>

#include "metric.hpp"

using eosio::current_time_point;

resource::resource(name s, name code, datastream<const char *> ds)
    : contract(s, code, ds), _configs_singleton(get_self(), get_self().value)
{
  _config_state = _configs_singleton.exists() ? _configs_singleton.get() : config{};
}

resource::~resource()
{
  _configs_singleton.set(_config_state, get_self());
}

ACTION resource::settotal(name source, float total_cpu_quantity, float total_net_quantity, time_point_sec timestamp)
{
  require_auth(source);
  check(is_source(source) == true, "not authorized to execute this action");
  //check(!_config_state.open, "prior collection period is still open");

  history_table h_t(get_self(), get_self().value);
  auto itr = h_t.end();
  itr--;

  time_point_sec next = time_point_sec(86400 + itr->timestamp.sec_since_epoch());
  
  /**
   * timestamp must be 24 hours after last history entry
   * time stamp cannot be greater than previous day
   **/
  // 
  check(timestamp == next, "invalid timestamp");
  check(next <=  time_point_sec(current_time_point().sec_since_epoch() - 86400), "cannot settotal for future date");

  metrics_table m_t("eosio"_n, "eosio"_n.value);
  auto m_itr = m_t.find(uint64_t(timestamp.sec_since_epoch()));
  // This should not happen.  Metrics will be populated in onblock action
  check(m_itr != m_t.end(), "metric does not exist, please try later");

  // Initial Inflation
  float VT = 0.0185; // Initial Inflation Constant for Value Transfer
  float MP = 0.2947; // TO-DO: move these 2 constants to the config talbe for easier adjusting?

  uint64_t draglimit = _config_state.emadraglimit;
  uint64_t day_count = itr->daycount + 1;

  float previousAverageCPU = itr->ma_cpu;
  float previousAverageNET = itr->ma_net;

  float ma_cpu_total = 0.0;
  float ma_net_total = 0.0;

  itr = h_t.end();
  for (int i = 1; i < draglimit; i++)
  {
    itr--;
    ma_cpu_total += itr->use_cpu;
    ma_net_total += itr->use_net;

    if (itr == h_t.begin())
    {
      break;
    }
  }

  // calculate period for moving averages during bootstrap period
  uint8_t period = day_count < draglimit ? day_count + 1 : draglimit;

  print(" :: period: ");
  print(std::to_string(period));

  float UTIL_CPU_MA = calcMA(ma_cpu_total, period, total_cpu_quantity);
  float UTIL_NET_MA = calcMA(ma_net_total, period, total_net_quantity);

  float UTIL_CPU_EMA;
  float UTIL_NET_EMA;

  uint64_t pk = h_t.available_primary_key();

  // use simple moving average until we reach draglimit samples
  if (pk >= draglimit)
  {
    UTIL_CPU_EMA = calcEMA(previousAverageCPU, draglimit, total_cpu_quantity);
    UTIL_NET_EMA = calcEMA(previousAverageNET, draglimit, total_net_quantity);
  }
  else
  {
    UTIL_CPU_EMA = UTIL_CPU_MA;
    UTIL_NET_EMA = UTIL_NET_MA;
  }
  float UTIL_TOTAL_EMA = UTIL_CPU_EMA + UTIL_NET_EMA;

  float inflation = (1 - UTIL_TOTAL_EMA) / (1 - UTIL_TOTAL_EMA - get_c(UTIL_TOTAL_EMA) * VT) - 1;

  float BP_U = MP * get_c(UTIL_TOTAL_EMA);
  float Upaygross = pow((1 + inflation), (1 - BP_U)) - 1;
  float Bppay = inflation - Upaygross;

  print(" :: UTIL_TOTAL_EMA: ");
  print(std::to_string(UTIL_TOTAL_EMA));

  print(" :: inflation: ");
  print(std::to_string(inflation));

  print(" :: BP_U: ");
  print(std::to_string(BP_U));

  print(" :: Upaygross: ");
  print(std::to_string(Upaygross));

  print(" :: Bppay: ");
  print(std::to_string(Bppay));

  // Locking

  asset locked_total = m_itr->wbi_locked;
  asset token_supply = m_itr->wbi_supply;

  float L = static_cast<float>(locked_total.amount) / token_supply.amount;
  float Lprime = (1 - UTIL_TOTAL_EMA) * L;

  print(" :: locked_totals: ");
  print(std::to_string(locked_total.amount));

  print(" :: L: ");
  print(std::to_string(L));

  print(" :: Lprime: ");
  print(std::to_string(Lprime));

  float x = (1 - UTIL_TOTAL_EMA) * (1 - Lprime) / (Lprime + (1 - UTIL_TOTAL_EMA) * (1 - Lprime));
  float y = 1 / (1 - log(UTIL_TOTAL_EMA));
  float z = pow(2, -(abs(UTIL_TOTAL_EMA - 0.5)));

  print(" :: x: ");
  print(std::to_string(x));

  print(" :: y: ");
  print(std::to_string(y));

  print(" :: z: ");
  print(std::to_string(z));

  float inflation_Lprime = (1 - (UTIL_TOTAL_EMA + Lprime)) / (1 - (UTIL_TOTAL_EMA + Lprime) - get_c(UTIL_TOTAL_EMA + Lprime) * VT) - 1;

  print(" ::  get_c(UTIL_TOTAL_EMA + Lprime): ");
  print(std::to_string(get_c(UTIL_TOTAL_EMA + Lprime)));

  print(" :: inflation_Lprime: ");
  print(std::to_string(inflation_Lprime));

  float bp_inflation_Lprime = get_c(UTIL_TOTAL_EMA + Lprime) * MP;

  print(" :: bp_inflation_Lprime: ");
  print(std::to_string(bp_inflation_Lprime));

  float Upaygross_inflation_Lprime = pow(1 + inflation_Lprime, 1 - bp_inflation_Lprime) - 1;

  print(" :: Upaygross_inflation_Lprime: ");
  print(std::to_string(Upaygross_inflation_Lprime));

  float uy_inflation_Lprime = Upaygross_inflation_Lprime / (UTIL_TOTAL_EMA + Lprime);

  print(" :: uy_inflation_Lprime: ");
  print(std::to_string(uy_inflation_Lprime));

  float LY_UL = pow(1 + uy_inflation_Lprime, x * y * z) - 1;
  float LP_UL = LY_UL * Lprime;

  print(" :: LY_UL: ");
  print(std::to_string(LY_UL));

  print(" :: LP_UL: ");
  print(std::to_string(LP_UL));

  // Inflation waterfall
  float Min_Upaynet = inflation * UTIL_TOTAL_EMA;

  print(" :: Min_Upaynet: ");
  print(std::to_string(Min_Upaynet));

  float Waterfall_bp = inflation * (1 - UTIL_TOTAL_EMA);

  print(" :: Waterfall_bp: ");
  print(std::to_string(Waterfall_bp));

  float Bppay_final = fmin(Bppay, Waterfall_bp);

  print(" :: Bppay_final: ");
  print(std::to_string(Bppay_final));

  float Waterfall_Locking = inflation * (1 - UTIL_TOTAL_EMA) - Bppay_final;

  print(" :: Waterfall_Locking: ");
  print(std::to_string(Waterfall_Locking));

  float LP_final = fmin(LP_UL, Waterfall_Locking);

  print(" :: LP_final: ");
  print(std::to_string(LP_final));

  float Uppaynet = inflation - Bppay_final - LP_final;

  print(" :: Uppaynet: ");
  print(std::to_string(Uppaynet));

  double Daily_i_U = pow(1 + inflation, static_cast<double>(1) / 365) - 1;

  print(" :: Daily_i_U: ");
  print(std::to_string(Daily_i_U));

  float utility_daily = (Uppaynet / inflation) * Daily_i_U;				                //allocate proportionally to Utility
  float bppay_daily = (Bppay_final / inflation) * Daily_i_U;				              //allocate proportionally to BPs
  float locking_daily = (LP_final / inflation) * Daily_i_U;

  // calculate inflation amount
  auto utility_tokens = static_cast<int64_t>( (utility_daily * double(token_supply.amount)));
  auto bppay_tokens = static_cast<int64_t>( (bppay_daily * double(token_supply.amount)));
  auto locking_tokens = static_cast<int64_t>( (locking_daily * double(token_supply.amount)));

  h_t.emplace(get_self(), [&](auto &h) {
    h.history_id = pk;
    h.timestamp = timestamp;
    h.use_cpu = total_cpu_quantity;
    h.use_net = total_net_quantity;
    h.daycount = day_count;
    h.locked_total = asset(0, eosio::symbol("WBI", 4));
    h.ma_cpu = UTIL_CPU_MA;
    h.ma_net = UTIL_NET_MA;
    h.ema_cpu = UTIL_CPU_EMA;
    h.ema_net = UTIL_NET_EMA;
    h.utility_daily = utility_daily;
    h.bppay_daily = bppay_daily;
    h.locking_daily = locking_daily;
    h.inflation = inflation;
    h.inflation_daily = Daily_i_U;
    h.utility_tokens = asset(utility_tokens, eosio::symbol("WBI", 4));
    h.bppay_tokens = asset(bppay_tokens, eosio::symbol("WBI", 4));
    h.locking_tokens = asset(locking_tokens, eosio::symbol("WBI", 4));
  });

  // open contract for user stats
  _config_state.open = true;

 // reset totals for user stats
  _config_state.allocated_cpu = 0.0;
  _config_state.allocated_net = 0.0;

  // inflation table to read by eosio.system to issue inflation
  inflation_table i_t(get_self(), get_self().value);
  
  i_t.emplace(get_self(), [&](auto &i) {
    i.timestamp = timestamp;
    i.utility_daily = asset(utility_tokens, eosio::symbol("WBI", 4));
    i.bppay_daily = asset(bppay_tokens, eosio::symbol("WBI", 4));
    i.locking_daily = asset(locking_tokens, eosio::symbol("WBI", 4));
  });

}

ACTION resource::adddistrib(name source, name account, float cpu_quantity, float net_quantity, time_point_sec timestamp)
{  
  require_auth(source);
  check(is_account(account), account.to_string() + " is not an account");
  check(is_source(source) == true, "not authorized to execute this action");
  check(!paused(), "this contract has been paused - please try again later");
  check(_config_state.open, "the collection period has been closed");

  history_table h_t(get_self(), get_self().value);
  auto itr_h = h_t.end();
  itr_h--;

  check(itr_h->timestamp == timestamp, "timestamp is not correct, collecting stats for: " + std::to_string(timestamp.sec_since_epoch()));
  check(_config_state.allocated_cpu + cpu_quantity <= 100, "cpu allocation greater than 100%" );
  check(_config_state.allocated_net + net_quantity <= 100, "net allocation greater than 100%" );

  _config_state.allocated_cpu += cpu_quantity;
  _config_state.allocated_net += net_quantity;

  float add_claim = 0.0;

  distribpay_table d_t(get_self(), get_self().value);
  auto itr_d = d_t.find(account.value);
  if( itr_d == d_t.end() ) {
    d_t.emplace(get_self(), [&](auto &d) {
        d.account = account;
        d.timestamp = timestamp;
        d.payout = add_claim;
    });
  } else {
    check(itr_d->timestamp != timestamp, "duplicate distribution" );
    d_t.modify(itr_d, same_payer, [&](auto &d) {
        d.timestamp = timestamp;
        d.payout += add_claim;
    });
  }
}

ACTION resource::closedistrib(name source, time_point_sec timestamp) {
  require_auth(source);
  check(is_source(source) == true, "not authorized to execute this action");
  check(_config_state.open, "distribution is closed");

  history_table h_t(get_self(), get_self().value);
  auto itr_h = h_t.end();
  itr_h--;

  check(itr_h->timestamp == timestamp, "timestamp is not correct, collecting stats for: " + std::to_string(timestamp.sec_since_epoch()));
  _config_state.open = false;
}

ACTION resource::claimdistrib(name account)
{
  require_auth(account);
  check(!paused(), "this contract has been paused - please try again later");
  check(!_config_state.open, "cannot claim while inflation calculation is running");

  distribpay_table d_t(get_self(), get_self().value);
  auto itr = d_t.find(account.value);
  check(itr != d_t.end(), "account not found");
  check(itr->payout != 0, "zero balance to claim");
  float fquantity = round(itr->payout);
  asset quantity(fquantity, eosio::symbol("WBI", 4));

  // x(15000, eosio::symbol("EOS",4));

  itr = d_t.erase(itr);

  //**********************************************************************************************************
  // The code below is simply copied from the eosio.token issue action
  //**********************************************************************************************************
  auto sym = quantity.symbol;
  check(sym.is_valid(), "invalid symbol name");
  stats statstable(get_self(), sym.code().raw());
  auto existing = statstable.find(sym.code().raw());
  check(existing != statstable.end(), "token with symbol does not exist, create token before issue");
  const auto &st = *existing;
  //require_auth(st.issuer);
  auto memo = "Distribution of Worbli resource usage rewards";
  check(quantity.is_valid(), "invalid quantity");
  check(quantity.amount > 0, "must issue positive quantity");
  check(quantity.symbol == st.supply.symbol, "symbol precision mismatch");
  check(quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");
  statstable.modify(st, same_payer, [&](auto &s) {
    s.supply += quantity;
  });
  add_balance(st.issuer, quantity, st.issuer);
  if (account != st.issuer)
  {
    SEND_INLINE_ACTION(*this, transfer, {{st.issuer, "active"_n}}, {st.issuer, account, quantity, memo});
  }
}

ACTION resource::updconfig(bool paused, uint32_t emadraglimit)
{
  require_auth(get_self());
  check(emadraglimit > 0, "ema drag must be greater than 0");
  check(emadraglimit < 20, "ema drag must be less than 20");
  _config_state.paused = paused;
  _config_state.emadraglimit = emadraglimit;
}

ACTION resource::addupdsource(name account, uint8_t in_out)
{
  require_auth(get_self());
  sourceauth_table s_t(get_self(), get_self().value);
  if (in_out == 0)
  {
    auto itr = s_t.find(account.value);
    check(itr != s_t.end(), "authorized source account not found during removal");
    itr = s_t.erase(itr);
  }
  else
  {
    auto itr2 = s_t.find(account.value);
    check(itr2 == s_t.end(), "authorized source account already exists in sourceauths table");
    s_t.emplace(get_self(), [&](auto &s) {
      s.account = account;
    });
  }
}

ACTION resource::transfer(name from, name to, asset quantity, string memo)
{
  require_auth(from);
  check(!paused(), "this contract has been paused - please try again later");
  check(from != to, "cannot transfer to self");
  check(is_account(to), "to account does not exist");
  auto sym = quantity.symbol.code();
  stats statstable(get_self(), sym.raw());
  const auto &st = statstable.get(sym.raw());
  require_recipient(from);
  require_recipient(to);
  check(quantity.is_valid(), "invalid quantity");
  check(quantity.amount > 0, "must transfer positive quantity");
  check(quantity.symbol == st.supply.symbol, "symbol precision mismatch");
  check(memo.size() <= 256, "memo has more than 256 bytes");
  auto payer = has_auth(to) ? to : from;
  sub_balance(from, quantity);
  add_balance(to, quantity, payer);
}

ACTION resource::init(time_point_sec start)
{
  require_auth(get_self());
  history_table h_t(get_self(), get_self().value);

  check(h_t.begin() == h_t.end(), "init already called");

  uint64_t pk = h_t.available_primary_key();
  h_t.emplace(get_self(), [&](auto &h) {
    h.history_id = pk;
    h.timestamp = start;
    h.use_cpu = 0;
    h.use_net = 0;
    h.daycount = 0;
    h.locked_total = asset(0, eosio::symbol("WBI", 4));
    h.ma_cpu = 0;
    h.ma_net = 0;
    h.ema_cpu = 0;
    h.ema_net = 0;
    h.utility_daily = 0;
    h.bppay_daily = 0;
    h.locking_daily = 0;
  });
}

EOSIO_DISPATCH(resource, (settotal)(adddistrib)(claimdistrib)(closedistrib)(updconfig)(addupdsource)(transfer)(init));
