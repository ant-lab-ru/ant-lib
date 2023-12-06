cmake_minimum_required(VERSION 3.14)

project(ant-scheduler-WRR)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE "${PROJECT_SOURCE_DIR}/include")

target_link_libraries(${PROJECT_NAME} INTERFACE 
    ant-interfaces
)
