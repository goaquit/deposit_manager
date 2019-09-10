#pragma once

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
	void CalculateRisk();
	void Process();

	double _deposit = 0.0;
	double _free_cash = 0.0;
	double _available_risk = 0.0;
	int _lot = 0;

	double _close_price;

	int _amount_lot = 0;
	double _stop_lose = 0.0;
	double _take_profit = 0.0;

};
