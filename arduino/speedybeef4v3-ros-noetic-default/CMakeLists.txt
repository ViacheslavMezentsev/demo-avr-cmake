cmake_minimum_required(VERSION 3.21)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/gcc-arm-none-eabi.cmake")

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH ARDUINO_PROJ_DIR)
cmake_path(GET ARDUINO_PROJ_DIR PARENT_PATH DEMO_AVR_DIR)

# В качестве имени проекта используем имя папки (см. tasks.json и launch.json).
get_filename_component(BASE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(PROJECT_NAME ${BASE_FOLDER})

project(${PROJECT_NAME} C CXX ASM)

add_executable(${PROJECT_NAME} ros_lib/duration.cpp ros_lib/time.cpp sensor.cpp)

add_subdirectory(Arduino/Core)
add_subdirectory(Arduino/libraries/SPI)
add_subdirectory(Arduino/libraries/BMI270-SPI)
add_subdirectory(Arduino/libraries/SrcWrapper)
add_subdirectory(Arduino/libraries/USBDevice)

target_link_libraries(${PROJECT_NAME} Arduino::Core SPI SrcWrapper USBDevice)

target_compile_definitions(${PROJECT_NAME} PRIVATE 
    STM32F4xx
    STM32F405xx
    USBCON
    USBD_USE_CDC
    USBD_VID=0x0483
    USBD_PID=0x5740
    HAL_PCD_MODULE_ENABLED    
    HAL_UART_MODULE_ENABLED
    ARDUINO=10819
    ARDUINO_GENERIC_F405RGTX
    ARDUINO_ARCH_STM32
    BOARD_NAME="GENERIC_F405RGTX"
    VARIANT_H="variant_generic.h"
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
    -Wall
    # optimization
    $<$<CONFIG:DEBUG>:-O2 -g -g3>
    $<$<CONFIG:RELEASE>:-Os>
    # other
    -c
)

# Файл конфигурации компоновщика.
set(LINKER_FILE "${CMAKE_CURRENT_SOURCE_DIR}/STM32F405RGTX_FLASH.ld")

target_link_options(${PROJECT_NAME} PRIVATE
    -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mlittle-endian
    --specs=nano.specs
    #-nostartfiles
    -T${LINKER_FILE} LINKER:-Map=${PROJECT_NAME}.map,--no-warn-rwx-segments,--gc-sections,--print-memory-usage
)

# Артефакты сборки.
stm32_print_size_of_target(${PROJECT_NAME})
stm32_generate_binary_file(${PROJECT_NAME})
stm32_generate_hex_file(${PROJECT_NAME})
stm32_generate_lss_file(${PROJECT_NAME})
