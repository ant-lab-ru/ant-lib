cmake_minimum_required(VERSION 3.14)

project(ant-scheduler-WRR)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PUBLIC
    "${PROJECT_SOURCE_DIR}/src/scheduler-WRR.cpp"
)

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")

target_link_libraries(${PROJECT_NAME} PUBLIC 
    ant-interfaces
)
