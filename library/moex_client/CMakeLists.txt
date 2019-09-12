set(src
   "src/moex_client.cpp"
)

add_library(moex_client_library STATIC ${src})
target_link_libraries(moex_client_library
   PUBLIC
      cpprestsdk::cpprest cpprestsdk::cpprestsdk_zlib_internal cpprestsdk::cpprestsdk_boost_internal cpprestsdk::cpprestsdk_openssl_internal
)
target_include_directories(moex_client_library PUBLIC "include")
