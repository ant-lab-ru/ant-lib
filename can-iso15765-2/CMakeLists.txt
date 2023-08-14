cmake_minimum_required(VERSION 3.14)

project(can-iso15765-2)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PUBLIC
"${PROJECT_SOURCE_DIR}/include/ant-lib/can-iso157652.h"
"${PROJECT_SOURCE_DIR}/src/can-iso157652.cpp"
)

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")
