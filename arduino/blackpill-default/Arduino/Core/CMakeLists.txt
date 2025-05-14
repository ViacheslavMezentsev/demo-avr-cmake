cmake_minimum_required(VERSION 3.16)

add_library(ArduinoCore OBJECT)

add_library(Arduino::Core ALIAS ArduinoCore)

target_include_directories(ArduinoCore PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/avr
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/stm32
    
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/libraries/SrcWrapper/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/libraries/SrcWrapper/inc/LL
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/libraries/USBDevice/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/libraries/Wire/src/utility
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/libraries/Wire/src

    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/system/STM32F4xx
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/system/Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/system/Drivers/STM32F4xx_HAL_Driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/system/Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/system/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/system/Middlewares/ST/STM32_USB_Device_Library/Core/Inc  
    
    ${CMAKE_CURRENT_SOURCE_DIR}/../variants/STM32F4xx/F401CCU
    ${CMAKE_CURRENT_SOURCE_DIR}/../tools/CMSIS/5.9.0/CMSIS/Core/Include
)   

target_sources(ArduinoCore PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/abi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/avr/dtostrf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/board.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/core_debug.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/HardwareSerial.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/hooks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/IPAddress.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/itoa.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/pins_arduino.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/Print.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/RingBuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/stm32/startup_stm32yyxx.S
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/Stream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/Tone.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/WInterrupts.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/wiring_analog.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/wiring_digital.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/wiring_pulse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/wiring_shift.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/wiring_time.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/WMath.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/WSerial.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../modules/Arduino_Core_STM32/cores/arduino/WString.cpp
    
    ${CMAKE_CURRENT_SOURCE_DIR}/../variants/STM32F4xx/F401CCU/generic_clock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../variants/STM32F4xx/F401CCU/PeripheralPins.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../variants/STM32F4xx/F401CCU/PeripheralPins_BLACKPILL_F401Cx.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../variants/STM32F4xx/F401CCU/PeripheralPins_PYBSTICK26_LITE.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../variants/STM32F4xx/F401CCU/variant_BLACKPILL_F401Cx.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../variants/STM32F4xx/F401CCU/variant_generic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../variants/STM32F4xx/F401CCU/variant_PYBSTICK26_LITE.cpp 
)

target_compile_definitions(ArduinoCore PRIVATE 
    STM32F4xx
    STM32F401xC
    USBCON
    USBD_USE_CDC
    USBD_VID=0x0483
    USBD_PID=0x5740
    HAL_PCD_MODULE_ENABLED    
    HAL_UART_MODULE_ENABLED
    ARDUINO=10607
    ARDUINO_GENERIC_F401CCUX
    ARDUINO_ARCH_STM32
    BOARD_NAME="GENERIC_F401CCUX"
    VARIANT_H="variant_generic.h"
    VECT_TAB_OFFSET=0x0 
    USE_HAL_DRIVER 
    USE_FULL_LL_DRIVER
)

target_compile_options(ArduinoCore PRIVATE    
    -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mlittle-endian -fcommon
    -fdata-sections -ffunction-sections    
    --param max-inline-insns-single=500
    $<$<COMPILE_LANGUAGE:CXX>:-fpermissive -fno-threadsafe-statics -fno-exceptions -fno-rtti -funsigned-bitfields -fno-use-cxa-atexit -fshort-enums -Winline>
    # отладочная информация
    $<$<CONFIG:DEBUG>:-Os -g -g3>
)

target_link_options(ArduinoCore PRIVATE
    -T${LINKER_FILE}
    -mcpu=cortex-m4 -mthumb
    -lc -lgcc
    LINKER:-Map=${PROJECT_NAME}.map,--no-warn-rwx-segments,--cref,--gc-sections,--print-memory-usage
)
