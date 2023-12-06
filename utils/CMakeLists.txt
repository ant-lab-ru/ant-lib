cmake_minimum_required(VERSION 3.14)

project(tests-utils)

set (CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory("${PROJECT_SOURCE_DIR}/ehas")
add_subdirectory("${PROJECT_SOURCE_DIR}/tests/ehas")

# add_subdirectory(${PROJECT_SOURCE_DIR}/queue)

add_subdirectory("${PROJECT_SOURCE_DIR}/schedulers")
add_subdirectory("${PROJECT_SOURCE_DIR}/tests/schedulers")

add_subdirectory("${PROJECT_SOURCE_DIR}/../interfaces" "${PROJECT_BINARY_DIR}/interfaces")

enable_testing()
include(GoogleTest)
