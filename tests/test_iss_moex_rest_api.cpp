#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <moex_client.h>

#include <memory>

#include <cpprest/http_client.h>

constexpr char kMoexUrl[] = "https://iss.moex.com";

using web::http::http_response;
using web::http::methods;
using web::http::client::http_client;
using web::json::json_exception;

using HttpClientPtrU = std::unique_ptr<http_client>;

class MoexRestClient : public ::testing::Test {
  // Test interface
 protected:
  void SetUp() override;

  HttpClientPtrU _client;
};

void MoexRestClient::SetUp() {
  web::uri uri(U(kMoexUrl));
  _client = std::make_unique<http_client>(uri);
}

TEST_F(MoexRestClient, GetSecuritiesInfo) {
  auto callback_response = [](http_response response) {
    {
      auto actual_status_code = response.status_code();
      int expected = 200;

      ASSERT_EQ(actual_status_code, expected);
    }

    try {
      auto json_response = response.extract_json(true).get();

      auto securities = json_response.at(U("securities")).as_object();

      {
        auto securities_data = securities.at(U("data")).as_array();

        auto first = securities_data.begin();

        auto actual_lot_size = first->at(4).as_number().to_int32();
        int expected = 1000;

        ASSERT_EQ(actual_lot_size, expected);
      }

      auto marketdata = json_response.at((U("marketdata"))).as_object();
      {
        auto data = marketdata.at(U("data")).as_array();

        auto first = data.begin();

        size_t index_last_price = 12;
        ASSERT_NO_THROW(first->at(index_last_price).as_double());
      }

    } catch (json_exception& e) {
      FAIL() << "Json parse fail with message: " << e.what();
    }
  };

  constexpr char request[] =
      "/iss/engines/stock/markets/shares/boards/TQBR/securities/rsti.json";

  web::uri_builder uri_builder(U(request));

  _client->request(methods::GET, uri_builder.to_string())
      .then(callback_response)
      .wait();
}

namespace moex_client {

TEST(MoexClient, GetSecuritiesInfo) {
  MoexClient client(kMoexUrl);

  ASSERT_TRUE(client.requestForSecurityInformation("RSTI"));

  {
    const auto actual_lot_size = client.GetLotSize();
    int expected = 1000;

    ASSERT_EQ(actual_lot_size, expected);
  }

  {
    const auto actual_last_price = client.GetLastPrice();
    double expected = 0.0;

    ASSERT_GE(actual_last_price, expected);
  }
}

}  // namespace moex_client

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
