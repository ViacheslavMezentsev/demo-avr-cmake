cmake_minimum_required(VERSION 3.19)

target_include_directories(${PROJECT_NAME} PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/Mcucpp/3rdparty/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/Mcucpp/mcucpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/Mcucpp/mcucpp/impl"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/Mcucpp/mcucpp/AVR"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/Mcucpp/mcucpp/AVR/atmega168"
)
