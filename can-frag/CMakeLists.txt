cmake_minimum_required(VERSION 3.14)

project(can-frag)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PUBLIC
"${PROJECT_SOURCE_DIR}/include/libs/can-frag.h"
"${PROJECT_SOURCE_DIR}/src/can-frag.cpp"
)

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")
