cmake_minimum_required(VERSION 3.14)

project(ant-hmc5883l)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PUBLIC
"${PROJECT_SOURCE_DIR}/include/ant-lib/hmc5883l.h"
"${PROJECT_SOURCE_DIR}/src/hmc5883l.cpp"
)

target_include_directories(${PROJECT_NAME} PUBLIC 
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/../../ant-interfaces/include"
)

target_link_libraries(${PROJECT_NAME} PUBLIC ant-ehas)
