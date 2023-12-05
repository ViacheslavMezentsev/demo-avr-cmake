cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)

set(MCU atmega328p)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../../modules/avr-cmake/cmake/avr_gcc.cmake")

# В качестве имени проекта используем имя папки (см. tasks.json и launch.json).
get_filename_component(BASE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(PROJECT_NAME ${BASE_FOLDER})

project(${PROJECT_NAME} C CXX ASM)

add_executable(${PROJECT_NAME})

add_subdirectory(stdcpp)

target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/modm/ext/printf/printf.c

    ${CMAKE_CURRENT_SOURCE_DIR}/modm/src/modm/architecture/driver/atomic/flag.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modm/src/modm/io/iostream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modm/src/modm/io/iostream_printf.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modm/src/modm/board/board_fuses.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modm/src/modm/board/board_serial.cpp    
    ${CMAKE_CURRENT_SOURCE_DIR}/modm/src/modm/board/board_fuses.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modm/src/modm/board/board_serial.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modm/src/modm/math/utils/avr/sqrt32.sx
    ${CMAKE_CURRENT_SOURCE_DIR}/modm/src/modm/math/utils/bit_operation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modm/src/modm/platform/clock/clock.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modm/src/modm/platform/core/assert.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modm/src/modm/platform/uart/uart0_rx.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modm/src/modm/platform/uart/uart0_tx.cpp

    main.cpp
)

target_link_libraries(${PROJECT_NAME} m)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${TOOLCHAIN_INC_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/modm/ext
    ${CMAKE_CURRENT_SOURCE_DIR}/modm/src
)

target_compile_definitions(${PROJECT_NAME} PRIVATE F_CPU=16000000UL)

target_compile_options(${PROJECT_NAME} PRIVATE    
    -mmcu=${MCU}    
    # предупреждения 
    -W
    -Wall
    -Wduplicated-cond
    -Werror=format
    -Werror=maybe-uninitialized
    -Werror=overflow
    -Werror=sign-compare
    -Wextra
    -Wlogical-op
    -Wno-redundant-decls
    -Wpointer-arith
    -Wundef
    $<$<COMPILE_LANGUAGE:CXX>:-Wno-volatile -Woverloaded-virtual>
    # оптимизация 
    $<$<CONFIG:DEBUG>:-O1>
    $<$<CONFIG:RELEASE>:-Os>
    -funsigned-char
    -ffunction-sections -fdata-sections
    -finline-limit=10000
    -fno-builtin-printf
    -fshort-wchar
    -funsigned-bitfields
    -fwrapv
    $<$<CONFIG:DEBUG>:-fno-move-loop-invariants -fno-split-wide-types -fno-tree-loop-optimize>
    # отладочная информация
    $<$<CONFIG:DEBUG>:-gdwarf-2 -gstrict-dwarf>
    # c/cxx флаги    
    $<$<COMPILE_LANGUAGE:CXX>:-fpermissive -fno-exceptions -fno-rtti -fno-unwind-tables -fno-threadsafe-statics -fno-use-cxa-atexit -fstrict-enums>
    # GCC 12 AVR array subscript 0 is outside array bounds
    $<$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,12.0>:--param=min-pagesize=0>
)

target_link_options(${PROJECT_NAME} PRIVATE
    -mmcu=${MCU}  
    -Wl,--defsym=__TEXT_REGION_LENGTH__=32K
    -Wl,--defsym=__DATA_REGION_LENGTH__=2K
    -Wl,--defsym=__EEPROM_REGION_LENGTH__=1K
    -Wl,-Map=${PROJECT_NAME}.map,--cref,--gc-sections,--print-memory-usage
    #-Wl,-u,vfprintf -lprintf_min -lprintf_flt
    #-Wl,--verbose
)

# Подключаем конфигурационный файл компоновщика.
avr_add_linker_script(${PROJECT_NAME} PRIVATE default.ld)

# Артефакты сборки.
avr_print_size_of_target(${PROJECT_NAME})
avr_generate_binary_file(${PROJECT_NAME})
avr_generate_hex_file(${PROJECT_NAME})
avr_generate_lss_file(${PROJECT_NAME})
