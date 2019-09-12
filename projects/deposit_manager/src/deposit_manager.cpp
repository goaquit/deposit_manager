#include "deposit_manager.h"

#include <iostream>

#include <securities.h>

namespace deposit_manager {

constexpr char kMoexApiUri[] = "https://iss.moex.com";

Deposit GetDeposit();
Broker GetBroker();
std::string GetSecurities();

using moex_client::MoexClient;

DepositManager::DepositManager()
    : _moex_client(std::make_unique<MoexClient>(kMoexApiUri)) {}

void DepositManager::Run() {
  try {
    _deposit = GetDeposit();
    _broker = GetBroker();

    for (;;) {
      const auto securities = GetSecurities();

      GetSecuritiesInformation(securities);
    }

  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

Deposit GetDeposit() {
  double deposit = 0.0, free_deposit = 0.0;

  std::cout << "Deposit: ";

  if (!(std::cin >> deposit) || deposit <= 0.0) {
    throw std::runtime_error(
        "Incorrect deposit value. Deposit must by greater than 0");
  }

  std::cout << "Free deposit: ";

  if (!(std::cin >> free_deposit) || free_deposit < 0.0 ||
      free_deposit > deposit) {
    throw std::runtime_error(
        "Incorrect free deposit value. Free deposit cannot be negative or more "
        "than "
        "deposit.");
  }

  double risk = 0.0;
  std::cout << "Available risk for deposit ( from 0.0 to 1.0 ): ";
  if (!(std::cin >> risk) || (risk < 0.0 || risk > 1.0)) {
    throw std::runtime_error("Incorrect risk value. Risk Level Out of Range.");
  }

  Deposit depo(deposit, free_deposit);
  depo.SetRiskLevel(risk);

  return depo;
}

Broker GetBroker() {
  double tax_broker = 0.003;
  double tax_exchange = 0.0001;

  std::cout << "Broker tax: " << tax_broker
            << "(default); Exchange tax: " << tax_exchange << "(default);"
            << std::endl;

  return Broker(tax_broker, tax_exchange);
}

std::string GetSecurities() {
  std::cout << "\nSecurities (or `q` for quit from programm): ";
  std::string securities = "";

  if (!(std::cin >> securities) || securities.empty()) {
    throw std::runtime_error("Incorrect securities.");
  }

  if (securities == std::string("q") || securities == std::string("Q")) {
    throw std::runtime_error("Programm stop.");
  }

  return securities;
}

void DepositManager::GetSecuritiesInformation(const std::string& securities) {
  if (!_moex_client->requestForSecurityInformation(securities)) {
    const auto error_message =
        "Request to moex api not done.\n" + _moex_client->error();
    std::cerr << error_message << std::endl;
    return;
  }

  Securities sec(_moex_client->GetLastPrice(), _broker);
  sec.SetLotSize(_moex_client->GetLotSize());

  ShowInformation(sec);
}

void DepositManager::ShowInformation(const Securities& securities) const {
  const auto risk = _deposit.Risk();
  const auto volume = _deposit.AvailableVolume(securities);
  const auto stop_loss = _deposit.StopLossLevel(securities);
  const auto take_profit = _deposit.TakeProfitLevel(securities);
  const auto price = securities.Price();

  const auto profit = (take_profit - securities.BuyPrice()) * volume;

  std::cout << "Price: " << price << "\nVolume: " << volume
            << "\nStop loss: " << stop_loss << "\nTake profit: " << take_profit
            << "\nRisk: " << risk << "\nProfit: " << profit << std::endl;
}

}  // namespace deposit_manager
