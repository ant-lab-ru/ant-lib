cmake_minimum_required(VERSION 3.14)

project(imitator-mr25h40)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PUBLIC
"${PROJECT_SOURCE_DIR}/src/imitator-mr25h40.cpp"
"${PROJECT_SOURCE_DIR}/include/ant-imitators/imitator-mr25h40.h"
)

target_include_directories(${PROJECT_NAME} PUBLIC 
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/../../ant-interfaces/include"
)
