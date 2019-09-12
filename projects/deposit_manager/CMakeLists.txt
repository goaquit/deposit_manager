set(target_name "deposit_manager")

set(src
    "src/main.cpp"
   "src/deposit_manager.cpp"
)

add_executable(${target_name}  ${src})
target_link_libraries(${target_name}
   PUBLIC
      deposit_manager_library
      moex_client_library
      Boost::program_options
)
target_include_directories(${target_name}
   PUBLIC
      "include"
)
