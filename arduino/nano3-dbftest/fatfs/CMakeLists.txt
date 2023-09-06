cmake_minimum_required(VERSION 3.19)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(${PROJECT_NAME} PRIVATE
#    ccsbcs.c
    diskio.c
    ff.c
    FatFs.cpp
    FatFsCard.cpp
)
