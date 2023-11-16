cmake_minimum_required(VERSION 3.14)

project(interfaces LANGUAGES C CXX)

add_library(${PROJECT_NAME} INTERFACE)

target_sources(${PROJECT_NAME} PUBLIC
        "${PROJECT_SOURCE_DIR}/include/ant-hwi/can-if.h"
        "${PROJECT_SOURCE_DIR}/include/ant-hwi/can-packet-if.h"
        "${PROJECT_SOURCE_DIR}/include/ant-hwi/i2c-if.h"
        "${PROJECT_SOURCE_DIR}/include/ant-hwi/spi-if.h"
        "${PROJECT_SOURCE_DIR}/include/ant-hwi/wdt-if.h"
	
)

target_include_directories(${PROJECT_NAME} INTERFACE "${PROJECT_SOURCE_DIR}/include")