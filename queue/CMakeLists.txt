cmake_minimum_required(VERSION 3.14)

project(static-queue)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PUBLIC
    "${PROJECT_SOURCE_DIR}/include/libs/static-queue.h"
    "${PROJECT_SOURCE_DIR}/src/static-queue.cpp"
)

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")
