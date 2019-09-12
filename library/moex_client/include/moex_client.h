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
  MoexClient() = default;
  MoexClient(const std::string& rest_api_url);

  MoexClient(const MoexClient&) = delete;
  MoexClient& operator=(const MoexClient&) = delete;
  MoexClient(MoexClient&& moex_client) noexcept;
  MoexClient& operator=(MoexClient&&) noexcept;

  bool requestForSecurityInformation(const std::string& securities);

  int GetLotSize() const;
  double GetLastPrice() const;

  std::string error() const;

 private:
  HttpClientPtrU _client;

  int _lot_size = 0;
  double _last_price = 0.0;
  int _status_code = 0;

  void rest();

  std::string _error = "";
};

}  // namespace moex_client
