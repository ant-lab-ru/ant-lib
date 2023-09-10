cmake_minimum_required(VERSION 3.14)

project(ant-lib-tests)

set (CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(queue)
add_subdirectory(can-adapter)
add_subdirectory(ehas)
add_subdirectory(mr25h)

add_subdirectory(ant-imitators/imitator-mr25h40)

add_subdirectory(tests/test-mr25h)

enable_testing()
include(GoogleTest)

