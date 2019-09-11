#include <gtest/gtest.h>

#include <cpprest/http_client.h>

TEST(Integration_CppRestSdk, GettingStartHttpClient) {
  web::uri uri(U("https://reqres.in"));

  web::http::client::http_client client(uri);

  web::uri_builder builder(U("/api/users"));

  builder.append_query(U("page"), U("2"));

  auto callback_process_response = [](web::http::http_response response) {
    {
      const auto actual_status_code = response.status_code();
      int expected = 200;

      ASSERT_EQ(actual_status_code, expected);
    }

    try {
      auto json_obj = response.extract_json(true).get();

      {
        const auto actual_page = json_obj.at(U("page"));
        int expected = 2;

        ASSERT_EQ(actual_page, expected);
      }

      {
        const auto actual_total_page = json_obj.at(U("total_pages"));
        int expected = 2;

        ASSERT_EQ(actual_total_page, expected);
      }

      auto data = json_obj.at(U("data")).as_array();

      {
        const auto actual_size = data.size();

        int expected = 6;

        ASSERT_EQ(actual_size, expected);
      }

      auto first_element = data.at(0).as_object();

      bool is_finded = first_element.find(U("id")) != first_element.end();

      ASSERT_TRUE(is_finded);

      ASSERT_THROW(first_element.at(U("bad_key")), web::json::json_exception);

      std::cout << std::endl;
    } catch (web::json::json_exception& e) {
      FAIL() << "Json incorrect field" << e.what();
    }
  };

  try {
    client.request(web::http::methods::GET, builder.to_string())
        .then(callback_process_response)
        .wait();

  } catch (std::exception& e) {
    FAIL() << "Fail in http client request task " << e.what();
  }
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
