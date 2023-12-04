enable_testing()

add_executable(test-scheduler-WRR
    "${PROJECT_SOURCE_DIR}/tests/schedulers/test-scheduler-WRR.cpp"
)

target_link_libraries(test-scheduler-WRR PUBLIC
    ant-scheduler-WRR
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test-scheduler-WRR)
