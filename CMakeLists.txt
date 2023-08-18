cmake_minimum_required(VERSION 3.14)

project(libs-tests)

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
add_subdirectory(can-frag)

target_include_directories(libs-tests "${PROJECT_SOURCE_DIR}/interfaces/include")

enable_testing()
include(GoogleTest)

