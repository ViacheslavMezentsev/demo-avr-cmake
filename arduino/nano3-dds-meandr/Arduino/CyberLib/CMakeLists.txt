cmake_minimum_required(VERSION 3.19)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/CyberLib
)

target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/CyberLib/CyberLib.cpp
)
