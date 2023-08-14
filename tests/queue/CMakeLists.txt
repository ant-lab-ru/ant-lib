enable_testing()

add_executable(queue-tests
    "${PROJECT_SOURCE_DIR}/tests/queue/test-queue.cpp"
)

target_link_libraries(queue-tests PUBLIC
    queue
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(queue-tests)
