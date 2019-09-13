#pragma once
#include <cpprest/http_client.h>

namespace moex_client {

using web::http::http_response;
using web::http::methods;
using web::http::client::http_client;
using web::json::json_exception;

using HttpClientPtrU = std::unique_ptr<http_client>;

class MoexClientException : public std::runtime_error {
  int _status_code = 0;

 public:
  MoexClientException(const std::string& error_msg, int status_code = 0)
      : std::runtime_error(error_msg), _status_code(status_code) {}

  int StatusCode() const { return _status_code; }
};

class MoexClient {
 public:
  MoexClient() = default;
  MoexClient(const std::string& rest_api_url);

  MoexClient(const MoexClient&) = delete;
  MoexClient& operator=(const MoexClient&) = delete;
  MoexClient(MoexClient&& moex_client) noexcept;
  MoexClient& operator=(MoexClient&&) noexcept;

  struct Securities {
    int lot_size = 0;
    double last_price = 0.0;
  };

  Securities SecurityInformation(const std::string& securities);

  using SecuritiesMap = std::map<std::string, Securities>;

  SecuritiesMap GetSecuritisMap();

 private:
  HttpClientPtrU _client;
};

}  // namespace moex_client
