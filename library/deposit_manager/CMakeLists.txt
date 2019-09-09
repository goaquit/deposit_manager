set(src
   "src/broker.cpp"
   "src/securities.cpp"
   "src/deposit.cpp"
)

add_library(deposit_manager_library STATIC ${src})
target_include_directories(deposit_manager_library PUBLIC "include")
