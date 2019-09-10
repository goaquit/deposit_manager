#include "deposit.h"

namespace deposit_manager {

Deposit::Deposit(double amount, double free_amount)
    : _amount(amount), _amount_free(free_amount) {}

int Deposit::AvailableVolume(const Securities& securities) const {
  const auto volume = static_cast<int>(FreeDeposit() / securities.BuyPrice());
  const auto available = volume - volume % securities.lotSize();
  return available;
}

double Deposit::FreeDeposit() const {
  return _amount_free - (_amount * _level_risk);
}

double Deposit::StopLossLevel(const Securities& securities) const {
  const auto volume = AvailableVolume(securities);
  const auto tax_sell = 1 - securities.broker().tax();

  const auto cost = (securities.BuyPrice() - Risk() / volume) / tax_sell;

  return cost;
}

double Deposit::TakeProfitLevel(const Securities& securities) const {
  const auto volume = AvailableVolume(securities);
  const auto tax_sell = 1 - securities.broker().tax();

  const auto cost = (securities.BuyPrice() + 2 * Risk() / volume) / tax_sell;

  return cost;
}

double Deposit::Risk() const { return _level_risk * _amount; }

void Deposit::SetFreeAmoun(double amount) noexcept { _amount_free = amount; }

void Deposit::SetRiskLevel(double level_risk) noexcept {
  _level_risk = level_risk;
}

}  // namespace deposit_manager
