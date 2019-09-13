#include "deposit_manager.h"

#include <iostream>

#include <securities.h>

namespace deposit_manager {

constexpr char kMoexApiUri[] = "https://iss.moex.com";

std::string GetSecurities();
bool GetSecuritieslist();

using moex_client::MoexClient;
using moex_client::MoexClientException;

DepositManager::DepositManager()
    : _moex_client(std::make_unique<MoexClient>(kMoexApiUri)) {}

void DepositManager::Run(const Config& config) {
  try {
    _deposit = Deposit(config.deposit, config.deposit_free);
    _deposit.SetRiskLevel(config.risk_level);
    _broker = Broker(config.tax_broker, config.tax_exchange);

    if (config.show_list) {
      ShowListAllSecurities(config.securities);
    }

    for (;;) {
      const auto securities = GetSecurities();

      GetSecuritiesInformation(securities);
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}  // namespace deposit_manager

void DepositManager::ShowListAllSecurities(
    const std::vector<std::string>& filter) {
  const auto securities_map = _moex_client->GetSecuritisMap(filter);

  for (const auto& [sec_id, securities] : securities_map) {
    Securities sec(securities.last_price, _broker);
    sec.SetLotSize(securities.lot_size);

    ShowInformation(sec, sec_id);
  }
}

bool GetSecuritieslist() {
  std::cout << "\nShow all list? (y/n) : ";

  std::string command;
  if (!(std::cin >> command)) {
    throw std::runtime_error("Incorrect input.");
  }

  return (command == std::string("y") || command == std::string("Y"));
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

void DepositManager::GetSecuritiesInformation(const std::string& sec_id) {
  try {
    const auto [lot_size, last_price] =
        _moex_client->SecurityInformation(sec_id);

    Securities sec(last_price, _broker);
    sec.SetLotSize(lot_size);

    ShowInformation(sec, sec_id);

  } catch (MoexClientException& e) {
    std::cerr << "Request to moex api not done.\n" << e.what() << std::endl;
  }
}

void DepositManager::ShowInformation(const Securities& securities,
                                     const std::string& sec_id) const {
  const auto risk = _deposit.Risk();
  const auto volume = _deposit.AvailableVolume(securities);

  if (volume == 0) {
    std::cout << "\nSecurity " << sec_id << " not available for purchase."
              << std::endl;

    return;
  }

  const auto stop_loss = _deposit.StopLossLevel(securities);
  const auto take_profit = _deposit.TakeProfitLevel(securities);
  const auto price = securities.Price();

  const auto profit = (take_profit - securities.BuyPrice()) * volume;

  std::cout << "\nSecurities: " << sec_id << "\nPrice: " << price
            << "\nVolume: " << volume << "\nStop loss: " << stop_loss
            << "\nTake profit: " << take_profit << "\nRisk: " << risk
            << "\nProfit: " << profit << std::endl;
}

}  // namespace deposit_manager
