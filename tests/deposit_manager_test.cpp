#include <gtest/gtest.h>

// library
#include <broker.h>
#include <deposit.h>
#include <securities.h>

namespace deposit_manager {

TEST(DepositManager, TaxCalculate) {
  Broker broker(0.003, 0.0001);

  Securities securities(700, broker);

  const int volume = 100;

  const double actual_cost = securities.CalculateCost(volume);

  const double expected = 70217.0;

  ASSERT_DOUBLE_EQ(actual_cost, expected);
}

TEST(DepositManager, CalculateAvailableVolumeWith) {
  Deposit deposit(9000, 7000);

  Securities securities(0.5);
  securities.SetLotSize(100);

  const auto actual_volume = deposit.AvailableVolume(securities);

  const int expected = 14000;

  ASSERT_EQ(actual_volume, expected);
}
TEST(DepositManager, CalculateAvailableVolumeWithRisk) {
  Deposit deposit(9000, 7000);
  deposit.SetRiskLevel(0.01);

  Securities securities(0.5);
  securities.SetLotSize(100);

  const auto actual_volume = deposit.AvailableVolume(securities);

  const int expected = 13800;

  ASSERT_EQ(actual_volume, expected);
}

TEST(DepositManager, CalculateAvailableVolumeWithBroker) {
  Deposit deposit(9000, 7000);

  Broker broker(0.01, 0.005);
  Securities securities(1.5, broker);
  securities.SetLotSize(100);

  const auto actual_volume = deposit.AvailableVolume(securities);

  const int expected = 4500;

  ASSERT_EQ(actual_volume, expected);
}

}  // namespace deposit_manager

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
