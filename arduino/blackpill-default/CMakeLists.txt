cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/gcc-arm-none-eabi.cmake")

# В качестве имени проекта используем имя папки (см. tasks.json и launch.json).
get_filename_component(BASE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(PROJECT_NAME ${BASE_FOLDER})

project(${PROJECT_NAME} C CXX ASM)

add_executable(${PROJECT_NAME} ros_lib/duration.cpp ros_lib/time.cpp HMC5883L_Simple.cpp sensor.cpp)

add_subdirectory(Arduino/Core)
add_subdirectory(Arduino/libraries/SrcWrapper)
add_subdirectory(Arduino/libraries/USBDevice)
add_subdirectory(Arduino/libraries/Wire)

target_link_libraries(${PROJECT_NAME} Arduino::Core)

target_compile_definitions(${PROJECT_NAME} PRIVATE 
    STM32F4xx
    STM32F401xC
    USBCON
    USBD_USE_CDC
    USBD_VID=0x0483
    USBD_PID=0x5740
    HAL_PCD_MODULE_ENABLED    
    HAL_UART_MODULE_ENABLED
    ARDUINO=10607
    ARDUINO_BLACKPILL_F401CC
    ARDUINO_ARCH_STM32
    BOARD_NAME="BLACKPILL_F401CC"
    VARIANT_H="variant_BLACKPILL_F401Cx.h"
    VECT_TAB_OFFSET=0x0 
    USE_HAL_DRIVER 
    USE_FULL_LL_DRIVER
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ros_lib
)

# Опции сборки.
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:DEBUG>:DEBUG_> STM32F4XX USE_STDPERIPH_DRIVER STM32F429_439xx)

target_compile_options(${PROJECT_NAME} PRIVATE
    # device type (cortex-m0, cortex-m3, ..)
    -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mlittle-endian -fcommon
    -fdata-sections -ffunction-sections    
    --param max-inline-insns-single=500    
    $<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics -fpermissive -fno-exceptions -fno-rtti -funsigned-bitfields -fno-use-cxa-atexit -fshort-enums -Winline>
    # warning
    -Wno-stringop-overflow
    -Wall
    # optimization
    $<$<CONFIG:DEBUG>:-O2 -g3>
    $<$<CONFIG:RELEASE>:-Os>
    # other
    -c
)

# Файл конфигурации компоновщика.
set(LINKER_FILE "${CMAKE_CURRENT_SOURCE_DIR}/STM32F401CCUX_FLASH.ld")

target_link_options(${PROJECT_NAME} PRIVATE
    -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mlittle-endian
    --specs=nano.specs
    #-nostartfiles
    -T${LINKER_FILE}
    LINKER:-Map=${PROJECT_NAME}.map,--gc-sections,--print-memory-usage
    $<$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,12.0>:-Wl,--no-warn-rwx-segments>
)

# Артефакты сборки.
stm32_print_size_of_target(${PROJECT_NAME})
stm32_generate_binary_file(${PROJECT_NAME})
stm32_generate_hex_file(${PROJECT_NAME})
stm32_generate_lss_file(${PROJECT_NAME})
