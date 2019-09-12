#pragma once
#include <cpprest/http_client.h>

namespace moex_client {

using web::http::http_response;
using web::http::methods;
using web::http::client::http_client;
using web::json::json_exception;

using HttpClientPtrU = std::unique_ptr<http_client>;

class MoexClient {
 public:
  MoexClient(const std::string& rest_api_url);

  bool requestForSecurityInformation(const std::string& securities);

  int GetLotSize() const;
  double GetLastPrice() const;

 private:
  HttpClientPtrU _client;

  int _lot_size = 0;
  double _last_price = 0.0;
  int _status_code = 0;

  void rest();
};

}  // namespace moex_client
