#include "moex_client.h"

namespace moex_client {

using string = utility::string_t;

MoexClient::MoexClient(const std::string& rest_api_url)
    : _client(std::make_unique<http_client>(
          string(rest_api_url.begin(), rest_api_url.end()))) {}

MoexClient& MoexClient::operator=(MoexClient&& moex_client) noexcept {
  _client = std::move(moex_client._client);

  return *this;
}

MoexClient::MoexClient(MoexClient&& moex_client) noexcept {
  *this = std::move(moex_client);
}

void CheckSuccessRequest(int status_code) {
  const auto success_response_code = 200;
  if (status_code != success_response_code) {
    throw MoexClientException("Request failed", status_code);
  }
}

MoexClient::Securities MoexClient::SecurityInformation(
    const std::string& requested_securities) {
  Securities securities;

  auto callback_response = [&securities](http_response response) {
    CheckSuccessRequest(response.status_code());

    auto json_response = response.extract_json(true).get();

    auto Value = [&json_response](const string& key, size_t index) {
      const auto field = json_response.at(key).as_object();

      if (field.at(U("data")).as_array().size() == 0) {
        throw MoexClientException(
            "It is impossible to obtain data on the specified security.");
      }

      const auto data = field.at(U("data")).as_array().begin();

      return data->at(index);
    };

    securities.lot_size = Value(U("securities"), 4).as_number().to_int32();
    securities.last_price = Value(U("marketdata"), 12).as_double();
  };

  const auto request =
      "/iss/engines/stock/markets/shares/boards/TQBR/securities/" +
      requested_securities + ".json";

  web::uri_builder uri_builder(string(request.begin(), request.end()));

  try {
    _client->request(methods::GET, uri_builder.to_string())
        .then(callback_response)
        .wait();

  } catch (std::exception& e) {
    const std::string error_msg("MoexClient terminated with an error: ");

    throw MoexClientException(error_msg + e.what());
  }

  return securities;
}

MoexClient::SecuritiesMap MoexClient::GetSecuritisMap() {
  SecuritiesMap securities_map;

  auto callback_response = [&securities_map](http_response response) {
    CheckSuccessRequest(response.status_code());

    try {
      auto json_response = response.extract_json(true).get();
      const auto securities_data =
          json_response["securities"]["data"].as_array();
      const auto marketdata_data =
          json_response["marketdata"]["data"].as_array();

      for (auto sec_itr = securities_data.begin(),
                market_itr = marketdata_data.begin();
           sec_itr != securities_data.end() &&
           market_itr != marketdata_data.end();
           ++sec_itr, ++market_itr) {
        const auto sec_id = sec_itr->at(0).as_string();

        const auto lot_size_value = sec_itr->at(4);
        const auto last_price_value = market_itr->at(12);
        if (lot_size_value.is_null() || last_price_value.is_null()) {
          continue;
        }

        const std::string key(sec_id.begin(), sec_id.end());
        auto& securities = securities_map[key];
        securities.lot_size = lot_size_value.as_number().to_int32();
        securities.last_price = last_price_value.as_double();
      }

      //      {
      //        for (const auto& data : securities_data) {
      //          const auto sec_id = data.at(0).as_string();
      //          const std::string key(sec_id.begin(), sec_id.end());

      //          auto& securities = securities_map[key];

      //          securities.lot_size = data.at(4).as_number().is_int32();
      //        }
      //      }

      //      {
      //        for (const auto& data : marketdata_data) {
      //          const auto sec_id = data.at(0).as_string();
      //          const std::string key(sec_id.begin(), sec_id.end());

      //          auto& securities = securities_map[key];

      //          securities.last_price = data.at(12).as_double();

      //          if (securities.last_price <= 0.0) {
      //            securities_map.erase(key);
      //          }
      //        }
      //    }
    } catch (json_exception& e) {
      std::cerr << "Json parse error:  " << e.what() << std::endl;
    }
  };

  const string request =
      "/iss/engines/stock/markets/shares/boards/TQBR/securities.json";

  web::uri_builder uri_builder(request);

  try {
    _client->request(methods::GET, uri_builder.to_string())
        .then(callback_response)
        .wait();

  } catch (std::exception& e) {
    const std::string error_msg("MoexClient terminated with an error: ");

    throw MoexClientException(error_msg + e.what());
  }

  return securities_map;
}  // namespace moex_client
}  // namespace moex_client
