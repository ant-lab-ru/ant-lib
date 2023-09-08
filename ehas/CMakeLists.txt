cmake_minimum_required(VERSION 3.14)

project(ant-ehas)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PUBLIC
    "${PROJECT_SOURCE_DIR}/include/ant-lib/ehas.h"
    "${PROJECT_SOURCE_DIR}/src/ehas.cpp"
)

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")
