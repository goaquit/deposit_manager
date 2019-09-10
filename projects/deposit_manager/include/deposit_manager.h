#pragma once

#include <broker.h>
#include <deposit.h>

namespace deposit_manager {

class DepositManager {
 public:
  DepositManager() = default;
  ~DepositManager() = default;

  DepositManager(const DepositManager&) = delete;
  DepositManager(DepositManager&&) = delete;
  DepositManager& operator=(const DepositManager&) = delete;
  DepositManager& operator=(DepositManager&&) = delete;

  void Run();

 private:
  Deposit _deposit;
  Broker _broker;

  void ShowInformation(const Securities& securities);
};

}  // namespace deposit_manager
