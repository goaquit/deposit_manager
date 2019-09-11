#include <gtest/gtest.h>

#include <fstream>
#include <iostream>

#include <cpprest/filestream.h>
#include <cpprest/http_client.h>
#include <cpprest/json.h>

TEST(CppRestSdk, GettingStartHttpClient) { ASSERT_TRUE(true); }

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
