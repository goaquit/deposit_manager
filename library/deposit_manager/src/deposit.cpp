#include "deposit.h"

namespace deposit_manager {

Deposit::Deposit(double amount, double free_amount)
    : _amount(amount), _amount_free(free_amount) {}

int Deposit::AvailableVolume(const Securities &securities) const {
  const auto volume = static_cast<int>(FreeDeposit() / securities.price());
  const auto available = volume - volume % securities.lotSize();
  return available;
}


double Deposit::FreeDeposit() const {
  return _amount_free - (_amount * _level_risk);
}


double Deposit::AvailableStopLose(const Securities& securities) const
{
	const auto volume = AvailableVolume(securities);

	const auto cost_securities = volume * securities.price();

	const auto cost = (volume * securities.price() - RiskAmount()) / volume;
	
	return cost;
}

double Deposit::RiskAmount() const
{
	return _level_risk * _amount;
}

void Deposit::SetFreeAmoun(double amount) noexcept { _amount_free = amount; }

void Deposit::SetRiskLevel(double level_risk) noexcept {
  _level_risk = level_risk;
}

}  // namespace deposit_manager
