#pragma once

#include "securities.h"

namespace deposit_manager {

class Deposit {
 public:
  Deposit() = default;
  Deposit(double amount, double free_amount = 0.0);

  int AvailableVolume(const Securities& securities) const;

  double StopLossLevel(const Securities& securities) const;
  double TakeProfitLevel(const Securities& securities) const;

  void SetFreeAmoun(double amount) noexcept;
  void SetRiskLevel(double level_risk) noexcept;

  double Risk() const;

 private:
  double _amount = 0.0;
  double _amount_free = 0.0;
  double _level_risk = 0.0;

  double FreeDeposit() const;
};

}  // namespace deposit_manager
