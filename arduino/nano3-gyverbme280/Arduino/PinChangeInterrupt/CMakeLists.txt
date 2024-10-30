cmake_minimum_required(VERSION 3.19)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/PinChangeInterrupt/src
)

target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/PinChangeInterrupt/src/PinChangeInterrupt.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/PinChangeInterrupt/src/PinChangeInterrupt0.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/PinChangeInterrupt/src/PinChangeInterrupt1.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/PinChangeInterrupt/src/PinChangeInterrupt2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/PinChangeInterrupt/src/PinChangeInterrupt3.cpp
)
