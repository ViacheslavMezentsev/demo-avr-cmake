cmake_minimum_required(VERSION 3.19)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/modm/ext/eyalroz/printf/src
)

target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/modm/ext/eyalroz/printf/src/printf/printf.c
)
