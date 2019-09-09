#pragma once

#include "securities.h"

namespace deposit_manager {

class Deposit {
 public:
  Deposit(double amount, double free_amount = 0.0);

  int AvailableVolume(const Securities &securities);

  void SetFreeAmoun(double amount) noexcept;
  void SetRiskLevel(double level_risk) noexcept;

 private:
  double _amount = 0.0;
  double _amount_free = 0.0;
  double _level_risk = 0.0;

  double FreeDeposit() const;
};

}  // namespace deposit_manager
