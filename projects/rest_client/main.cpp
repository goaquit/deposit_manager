#include <cpprest/filestream.h>
#include <cpprest/http_client.h>
#include <iostream>

using namespace utility;            // Common utilities like string conversions
using namespace web;                // Common features like URIs.
using namespace web::http;          // Common HTTP functionality
using namespace web::http::client;  // HTTP client features
using namespace concurrency::streams;  // Asynchronous streams

class ProcessResponse {
 public:
  void operator()(http_response response) {
    std::cout << "Status code: " << response.status_code() << std::endl;

    try {
      auto json_obj = response.extract_json(true).get();

      std::cout << "Page: " << json_obj.at(U("page"))
                << "\nTotal: " << json_obj.at(U("total_pages"));

      {
        auto data = json_obj.at(U("data")).as_array();

        for (const auto& element : data) {
          std::cout << "\n\nId: " << element.at(U("id"))
                    << "\nemail: " << element.at(U("email"))
                    << "\nFirst name: " << element.at(U("first_name"))
                    << "\nLast name: " << element.at(U("last_name"));
        }
      }

      std::cout << std::endl;
    } catch (json::json_exception& e) {
      std::cerr << e.what() << std::endl;
    }
  }
};

int main(int /*argc*/, char* /*argv*/[]) {
  try {
    // Create http_client to send the request.
    http_client client(U("https://reqres.in"));

    // Build request URI and start the request.
    uri_builder builder(U("/api/users"));
    builder.append_query(U("page"), U("1"));

    ProcessResponse process_response;
    client.request(methods::GET, builder.to_string())
        .then(process_response)
        .wait();

  } catch (std::exception& e) {
    std::cerr << "Request exception: " << e.what() << std::endl;
  }

  return 0;
}
