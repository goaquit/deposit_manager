add_executable(rest_client main.cpp)
target_link_libraries(rest_client
   PRIVATE
      cpprestsdk::cpprest cpprestsdk::cpprestsdk_zlib_internal cpprestsdk::cpprestsdk_boost_internal cpprestsdk::cpprestsdk_openssl_internal
)
