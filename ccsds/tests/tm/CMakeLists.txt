enable_testing()

add_executable(tests-ccsds-tm
    "${CMAKE_CURRENT_SOURCE_DIR}//test-ccsds-tm.cpp"
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    ant-ccsds-tm
    ant-queue-static
    ant-sheduler-WRR
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests-mr25h)
