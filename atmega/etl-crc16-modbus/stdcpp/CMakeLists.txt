cmake_minimum_required(VERSION 3.19)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/avr-libstdcpp/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/avr-libstdcpp/src
)

target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/avr-libstdcpp/src/functexcept.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/avr-libstdcpp/src/hash_bytes.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/avr-libstdcpp/src/hashtable_c++0x.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/avr-libstdcpp/src/limits.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/avr-libstdcpp/src/list.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/avr-libstdcpp/src/math.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/avr-libstdcpp/src/new_handler.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/avr-libstdcpp/src/tree.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/avr-libstdcpp/examples/common/cxxabi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/avr-libstdcpp/examples/common/new.cpp
)
