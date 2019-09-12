add_executable(test_deposit_manager
		"deposit_manager_test.cpp"
)
target_link_libraries(test_deposit_manager
   PRIVATE
      ${GTEST_BOTH_LIBRARIES}
   PUBLIC
      deposit_manager_library
)

add_test(RunTest test_deposit_manager)


add_executable(test_iss_moex_rest_api
   "test_iss_moex_rest_api.cpp"
)

target_link_libraries(test_iss_moex_rest_api
   PRIVATE
      ${GTEST_BOTH_LIBRARIES}
      cpprestsdk::cpprest cpprestsdk::cpprestsdk_zlib_internal cpprestsdk::cpprestsdk_boost_internal cpprestsdk::cpprestsdk_openssl_internal
      moex_client_library
)


add_test(RunTest test_iss_moex_rest_api)


