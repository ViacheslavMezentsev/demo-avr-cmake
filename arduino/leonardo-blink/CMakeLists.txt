cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 17)

set(MCU atmega32u4)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../../modules/avr-cmake/cmake/avr_gcc.cmake")

# В качестве имени проекта используем имя папки (см. tasks.json и launch.json).
get_filename_component(BASE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(PROJECT_NAME ${BASE_FOLDER})

project(${PROJECT_NAME} C CXX ASM)

add_executable(${PROJECT_NAME} blink.ino.cpp)

add_subdirectory(ArduinoCore)

target_link_libraries(${PROJECT_NAME} ArduinoCore::avr)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_definitions(${PROJECT_NAME} PRIVATE 
    F_CPU=16000000UL
    ARDUINO=10607
    #ARDUINO_AVR_NANO
    DARDUINO_AVR_LEONARDO
    ARDUINO_ARCH_AVR
)

target_compile_options(${PROJECT_NAME} PRIVATE    
    -mmcu=${MCU}    
    # предупреждения 
    -c
    -g
    -w
    -MMD
    -Wno-error=narrowing
    # оптимизация 
    $<$<CONFIG:DEBUG>:-O1>
    $<$<CONFIG:RELEASE>:-Os>
    -funsigned-char
    -ffunction-sections -fdata-sections
    # отладочная информация
    $<$<CONFIG:DEBUG>:-gdwarf-2 -gstrict-dwarf>
    # c/cxx флаги    
    $<$<COMPILE_LANGUAGE:CXX>:-fpermissive -fno-exceptions -funsigned-bitfields -fno-threadsafe-statics>
    # GCC 12 AVR array subscript 0 is outside array bounds
    #$<$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,12.0>:--param=min-pagesize=0>
)

target_link_options(${PROJECT_NAME} PRIVATE
    -mmcu=${MCU}    
    -Wl,--defsym=__TEXT_REGION_LENGTH__=32K
    -Wl,--defsym=__DATA_REGION_LENGTH__=2K+512
    -Wl,--defsym=__EEPROM_REGION_LENGTH__=1K
    -Wl,-Map=${PROJECT_NAME}.map,--cref,--gc-sections,--print-memory-usage
    #-Wl,--verbose
)

# Подключаем конфигурационный файл компоновщика.
#avr_add_linker_script(${PROJECT_NAME} PRIVATE default.ld)

# Артефакты сборки.
avr_print_size_of_target(${PROJECT_NAME})
avr_generate_binary_file(${PROJECT_NAME})
avr_generate_hex_file(${PROJECT_NAME})
avr_generate_lss_file(${PROJECT_NAME})
