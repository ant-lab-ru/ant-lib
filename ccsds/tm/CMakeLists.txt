cmake_minimum_required(VERSION 3.14)

project(ant-ccsds-tm)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PUBLIC
    "${PROJECT_SOURCE_DIR}/src/ccsds-tm-mc-send.cpp"
    "${PROJECT_SOURCE_DIR}/src/ccsds-tm-vc-send.cpp"
    "${PROJECT_SOURCE_DIR}/src/ccsds-tm.cpp"
)

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME} PUBLIC ant-ehas ant-queue-static ant-sheduler-WWR)