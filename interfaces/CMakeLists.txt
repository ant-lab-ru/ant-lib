cmake_minimum_required(VERSION 3.14)

project(ant-interfaces LANGUAGES C CXX)

add_library(${PROJECT_NAME} INTERFACE)

target_sources(${PROJECT_NAME} PUBLIC
    "${PROJECT_SOURCE_DIR}/include/ant-lib/if-can.h"
    "${PROJECT_SOURCE_DIR}/include/ant-lib/if-can-packet.h"
    "${PROJECT_SOURCE_DIR}/include/ant-lib/if-i2c.h"
    "${PROJECT_SOURCE_DIR}/include/ant-lib/if-spi.h"
    "${PROJECT_SOURCE_DIR}/include/ant-lib/if-wdt.h"
    "${PROJECT_SOURCE_DIR}/include/ant-lib/if-scheduler.h"
)

target_include_directories(${PROJECT_NAME} INTERFACE "${PROJECT_SOURCE_DIR}/include")