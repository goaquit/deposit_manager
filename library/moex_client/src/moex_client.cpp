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

MoexClient::Securities MoexClient::requestForSecurityInformation(
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
}  // namespace moex_client
