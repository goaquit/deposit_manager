#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <moex_client.h>

#include <memory>

#include <cpprest/http_client.h>

constexpr char kMoexUrl[] = "https://iss.moex.com";

namespace moex_client {

TEST(MoexClient, GetSecuritiesInfo) {
  MoexClient client(kMoexUrl);

  MoexClient::Securities securities;
  ASSERT_NO_THROW(securities = client.requestForSecurityInformation("RSTI"));

  {
    const auto actual_lot_size = securities.lot_size;
    int expected = 1000;

    ASSERT_EQ(actual_lot_size, expected);
  }

  {
    const auto actual_last_price = securities.last_price;
    double expected = 0.0;

    ASSERT_GE(actual_last_price, expected);
  }
}

TEST(MoexClient, UnsuccessGetSecuritiesInfo) {
  MoexClient client(kMoexUrl);

  ASSERT_THROW(client.requestForSecurityInformation("RSTIII"),
               MoexClientException);
}

}  // namespace moex_client

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
