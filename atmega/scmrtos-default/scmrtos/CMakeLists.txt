cmake_minimum_required(VERSION 3.19)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/os_kernel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/os_services.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/os_target.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/recursive_mutex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/usrlib.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/os_target_asm.S
)
