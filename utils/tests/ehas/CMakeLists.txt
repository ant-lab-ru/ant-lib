enable_testing()

add_executable(test-ehas
    "${CMAKE_CURRENT_SOURCE_DIR}/test-ehas.cpp"
)

target_link_libraries(test-ehas PUBLIC
    ant-ehas
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test-ehas)
