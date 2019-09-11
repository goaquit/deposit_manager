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
