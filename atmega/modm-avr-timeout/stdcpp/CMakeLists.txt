cmake_minimum_required(VERSION 3.19)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../modm/ext/gcc/libstdc++/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../modm/ext/gcc/libstdc++/src
)

target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../modm/ext/gcc/libstdc++/src/functexcept.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../modm/ext/gcc/libstdc++/src/hash_bytes.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../modm/ext/gcc/libstdc++/src/hashtable_c++0x.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../modm/ext/gcc/libstdc++/src/limits.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../modm/ext/gcc/libstdc++/src/list.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../modm/ext/gcc/libstdc++/src/math.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../modm/ext/gcc/libstdc++/src/new_handler.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../modm/ext/gcc/libstdc++/src/tree.cc

    ${CMAKE_CURRENT_SOURCE_DIR}/../modm/ext/gcc/assert.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../modm/ext/gcc/cxxabi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../modm/ext/gcc/new_delete.cpp
)
