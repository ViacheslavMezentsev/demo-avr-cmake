cmake_minimum_required(VERSION 3.19)

target_include_directories(${PROJECT_NAME} PUBLIC src)

target_sources(${PROJECT_NAME} PRIVATE src/microWire.cpp)
