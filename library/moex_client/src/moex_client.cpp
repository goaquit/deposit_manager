#include "moex_client.h"

namespace moex_client {

MoexClient::MoexClient(const std::string& rest_api_url)
    : _client(std::make_unique<http_client>(rest_api_url.c_str())) {}

bool MoexClient::requestForSecurityInformation(const std::string& securities) {
  rest();

  auto result_request = false;

  auto callback_response = [this, &result_request](http_response response) {
    _status_code = response.status_code();

    try {
      auto json_response = response.extract_json(true).get();

      auto Value = [&json_response](const std::string& key, size_t index) {
        const auto field = json_response.at(U(key)).as_object();
        const auto data = field.at(U("data")).as_array().begin();

        return data->at(index);
      };

      _lot_size = Value("securities", 4).as_number().to_int32();
      _last_price = Value("marketdata", 12).as_double();

      result_request = true;
    } catch (json_exception& e) {
      std::cerr << "Json parsin fail " << e.what() << std::endl;
    }
  };

  auto request = "/iss/engines/stock/markets/shares/boards/TQBR/securities/" +
                 securities + ".json";

  web::uri_builder uri_builder(U(request));

  try {
    auto task_status = _client->request(methods::GET, uri_builder.to_string())
                           .then(callback_response)
                           .wait();

    if (task_status == pplx::task_status::not_complete) {
      std::cerr << "Task is not completed" << std::endl;
    }

  } catch (std::exception&) {
    result_request = false;
  }

  return result_request;
}

int MoexClient::GetLotSize() const { return _lot_size; }

double MoexClient::GetLastPrice() const { return _last_price; }

void MoexClient::rest() {
  _lot_size = 0;
  _last_price = 0.0;
  _status_code = 0;
}

}  // namespace moex_client
