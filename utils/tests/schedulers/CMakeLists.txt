enable_testing()

add_executable(test-scheduler-WRR
    "${CMAKE_CURRENT_SOURCE_DIR}/test-scheduler-WRR.cpp"
)

target_link_libraries(test-scheduler-WRR PUBLIC
    ant-scheduler-WRR
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test-scheduler-WRR)
