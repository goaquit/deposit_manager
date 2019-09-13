#pragma once

#include <broker.h>
#include <deposit.h>
#include <moex_client.h>

namespace deposit_manager {

using MoexClientPtrU = std::unique_ptr<moex_client::MoexClient>;

class DepositManager {
 public:
  DepositManager();
  ~DepositManager() = default;

  DepositManager(const DepositManager&) = delete;
  DepositManager(DepositManager&&) = delete;
  DepositManager& operator=(const DepositManager&) = delete;
  DepositManager& operator=(DepositManager&&) = delete;

  struct Config {
    double deposit = 0.0;
    double deposit_free = 0.0;
    double tax_broker = 0.0;
    double tax_exchange = 0.0;
    double risk_level = 1.0;
    bool show_list = false;
    std::vector<std::string> securities;
  };

  void Run(const Config& config);

 private:
  Deposit _deposit;
  Broker _broker;

  MoexClientPtrU _moex_client = nullptr;

  void ShowListAllSecurities(const std::vector<std::string>& filter);

  void GetSecuritiesInformation(const std::string& securities);
  void ShowInformation(const Securities& securities,
                       const std::string& sec_id) const;
};

}  // namespace deposit_manager
