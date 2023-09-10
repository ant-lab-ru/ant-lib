enable_testing()

add_executable(tests-mr25h
    "${PROJECT_SOURCE_DIR}/tests/test-mr25h/test-mr25h.cpp"
)

target_link_libraries(tests-mr25h PUBLIC
    ant-mr25h
    imitator-mr25h40
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests-mr25h)
