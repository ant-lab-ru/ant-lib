cmake_minimum_required(VERSION 3.14)

project(ant-ccsds-epp)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PUBLIC
    "${PROJECT_SOURCE_DIR}/include/ant-lib/ccsds-epp.h"
    "${PROJECT_SOURCE_DIR}/src/ccsds-epp.cpp"
)

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME} PUBLIC ant-ehas)