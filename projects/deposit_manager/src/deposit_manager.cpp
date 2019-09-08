#include "deposit_manager.h"

#include <iostream>

const double kBrokerTax = 0.003;
const double kExchangeTax = 0.0001;
const double kMaxRisk = 0.01;

void DepositManager::Run() {
	try {
		std::cout << "Set current deposit: ";

		std::cin >> _deposit;

		std::cout << "Set free volume cash: ";
		std::cin >> _free_cash;

		if (_deposit == 0 || _free_cash > _deposit) {
			throw std::exception("Free cash bigger than deposit value.");
		}

		CalculateRisk();
		std::cout << "Available risk value: " << _available_risk << std::endl;

		std::cout << "Set lot amoun (1, 10, 100, 1 000, 10 000): ";
		std::cin >> _lot;

		std::cout << "Enter close price: ";
		std::cin >> _close_price;

		if (_close_price == 0.0)
		{
			throw std::exception("Price cannot be equal to 0");
		}

		Process();


		std::cout << "Amount: " << _amount_lot << "\nStop lose: " << _stop_lose << "\nTake profit: " << _take_profit << std::endl; 
		
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

void DepositManager::CalculateRisk() {
	_available_risk = _deposit * kMaxRisk;
}

void DepositManager::Process() {
	const auto available_cash = _free_cash - _available_risk;

	if (available_cash <= _available_risk) {
		throw std::exception("Risk exceeds available cash");
	}

	if (_lot == 0)
	{
		throw std::exception("Amount lot not set");
	}

	const auto available_amount = available_cash / (_close_price * (1 + kBrokerTax) * (1 + kExchangeTax));
	_amount_lot = (available_amount - (static_cast<int>(available_amount) % _lot)) / _lot;

	const auto count_item = _amount_lot * _lot;

	_stop_lose = (count_item * _close_price - _available_risk) / count_item;
	_take_profit = 2 * (_close_price - _stop_lose) + _close_price;
}