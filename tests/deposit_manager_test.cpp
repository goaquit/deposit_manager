#include <gtest/gtest.h>

class Broker
{
public:
	Broker() = default;
	Broker(double tax_broker, double tax_exchange) : _tax_broker(tax_broker),_tax_exchange(tax_exchange){}

	double tax() const
	{
		return _tax_broker + _tax_exchange;
	}

private:
	double _tax_broker = 0.0;
	double _tax_exchange = 0.0;
};

class Securities
{
public:
	Securities(double price) : _price(price) {};
	Securities(double price, const Broker &broker) : _price(price), _broker(broker){}

	double CalculateCost(int volume) const
	{
		return price() * volume;
	};


	double price() const
	{
		return _price * (1 + _broker.tax());
	}

	int lotSize() const
	{
		return _lot_size;
	}

	void SetLotSize(int size)
	{
		_lot_size = size;
	}

private:
	double _price = 0.0;
	Broker _broker;
	int _lot_size = 1;
};

class Deposit
{
public:
	Deposit(double amount, double free_amount = 0.0) : _amount(amount) , _amount_free(free_amount == 0.0 ? amount : free_amount)	{}

	void SetFreeAmoun(double amount)
	{
		_amount_free = amount;
	}

	int AvailableVolume(const Securities& securities)
	{
		const auto volume = static_cast<int>(FreeDeposit() / securities.price());
		const auto available = volume - volume % securities.lotSize();
		return available;
	}

	void SetRiskLevel(double level_risk)
	{
		_level_risk = level_risk;
	}

private:
	double _amount = 0.0;
	double _amount_free = 0.0;
	double _level_risk = 0.0;

	double FreeDeposit() const
	{
		return _amount_free - (_amount * _level_risk);
	}
};

TEST(DepositManager, TaxCalculate)
{
	Broker broker(0.003, 0.0001);

	Securities securities(700, broker);	

	const int volume = 100;

	const double actual_cost = securities.CalculateCost(volume);

	const double expected = 70217.0;

	ASSERT_DOUBLE_EQ(actual_cost, expected);
}

TEST(DepositManager, CalculateAvailableVolumeWith)
{
	Deposit deposit(9000, 7000);

	Securities securities(0.5);
	securities.SetLotSize(100);

	const auto actual_volume = deposit.AvailableVolume(securities);

	const int expected = 14000;


	ASSERT_EQ(actual_volume, expected);
}

TEST(DepositManager, CalculateAvailableVolumeWithRisk)
{
	Deposit deposit(9000, 7000);
	deposit.SetRiskLevel(0.01); 

	Securities securities(0.5);
	securities.SetLotSize(100);

	const auto actual_volume = deposit.AvailableVolume(securities);

	const int expected = 13800;


	ASSERT_EQ(actual_volume, expected);
}

TEST(DepositManager, CalculateAvailableVolumeWithBroker)
{
	Deposit deposit(9000, 7000);

	Broker broker(0.01, 0.005);
	Securities securities(1.5, broker);
	securities.SetLotSize(100);

	const auto actual_volume = deposit.AvailableVolume(securities);

	const int expected = 4500;


	ASSERT_EQ(actual_volume, expected);
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	
	return RUN_ALL_TESTS();
}