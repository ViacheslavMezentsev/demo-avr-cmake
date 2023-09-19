cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 17)

set(MCU atmega328p)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../../modules/avr-cmake/cmake/avr_gcc.cmake")

# В качестве имени проекта используем имя папки (см. tasks.json и launch.json).
get_filename_component(BASE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(PROJECT_NAME ${BASE_FOLDER})

project(${PROJECT_NAME} C CXX ASM)

add_executable(${PROJECT_NAME} usart.cpp main.cpp)

add_subdirectory(stdcpp)
add_subdirectory(libode)
#add_subdirectory(printf)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${AVR_TOOLCHAIN_PATH}/lib/gcc/avr/${CMAKE_CXX_COMPILER_VERSION}/include
)

target_link_libraries(${PROJECT_NAME} m)

target_compile_definitions(${PROJECT_NAME} PRIVATE 
    F_CPU=16000000UL
    $<$<CONFIG:RELEASE>:NDEBUG>
    _NO_BAD_XXX
    #PRINTF_SUPPORT_LONG_LONG=0
    #PRINTF_ALIAS_STANDARD_FUNCTION_NAMES=1
)

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
    # оптимизация 
    $<$<CONFIG:DEBUG>:-O1>
    $<$<CONFIG:RELEASE>:-Os>
    -funsigned-char
    -ffunction-sections -fdata-sections
    #-fno-builtin-printf
    -fshort-wchar
    -funsigned-bitfields
    -fwrapv
    # отладочная информация
    $<$<CONFIG:DEBUG>:-gdwarf-2 -gstrict-dwarf>
    # c/cxx флаги    
    $<$<COMPILE_LANGUAGE:CXX>:-fpermissive -fno-exceptions -fno-rtti -fno-unwind-tables -fno-threadsafe-statics -fno-use-cxa-atexit -fstrict-enums>
    # GCC 12 AVR array subscript 0 is outside array bounds
    $<$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,12.0>:--param=min-pagesize=0>
)

target_link_options(${PROJECT_NAME} PRIVATE
    -mmcu=${MCU}  
    -Wl,--relax
    #-Wl,--defsym=__TEXT_REGION_LENGTH__=32K
    #-Wl,--defsym=__DATA_REGION_LENGTH__=2K+512
    #-Wl,--defsym=__EEPROM_REGION_LENGTH__=1K
    -Wl,-Map=${PROJECT_NAME}.map,--cref,--gc-sections,--print-memory-usage
    -Wl,-u,vfprintf -lprintf_flt #-lprintf_min
    #-Wl,--verbose
)

# Подключаем конфигурационный файл компоновщика.
avr_add_linker_script(${PROJECT_NAME} PRIVATE ${MCU}.ld)

# Артефакты сборки.
avr_print_size_of_target(${PROJECT_NAME})
avr_generate_binary_file(${PROJECT_NAME})
avr_generate_hex_file(${PROJECT_NAME})
avr_generate_lss_file(${PROJECT_NAME})
