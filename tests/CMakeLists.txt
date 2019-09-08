add_executable(test_deposit_manager
		"deposit_manager_test.cpp"
)
target_link_libraries(test_deposit_manager
			 ${GTEST_BOTH_LIBRARIES}
		
)

add_test(NAME RunTest COMMAND test_deposit_manager)

#gtest_discover_tests(test_deposit_manager "" AUTO)