cmake_minimum_required(VERSION 3.14)

project(can-adapter)

target_sources(${PROJECT_NAME} PUBLIC
"${PROJECT_SOURCE_DIR}/include/libs/can-adapter.h"
"${PROJECT_SOURCE_DIR}/src/can-adapter.cpp"
)

add_library(${PROJECT_NAME} STATIC)

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")

target_link_libraries(${PROJECT_NAME} PUBLIC static-queue)
