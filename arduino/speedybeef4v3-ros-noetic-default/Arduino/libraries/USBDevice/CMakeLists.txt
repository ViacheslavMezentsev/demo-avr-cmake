# v3.21 implemented semantic changes regarding $<TARGET_OBJECTS:...>
# See https://cmake.org/cmake/help/v3.21/command/target_link_libraries.html#linking-object-libraries-via-target-objects
cmake_minimum_required(VERSION 3.21)

add_library(USBDeviceLib OBJECT)

add_library(USBDevice ALIAS USBDeviceLib)

target_include_directories(USBDeviceLib PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/cores/arduino
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/cores/arduino/stm32

  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/system/STM32F4xx
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/system/Drivers/CMSIS/Device/ST/STM32F4xx/Include
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/system/Drivers/STM32F4xx_HAL_Driver/Inc
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/system/Drivers/STM32F4xx_HAL_Driver/Src
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/system/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/system/Middlewares/ST/STM32_USB_Device_Library/Core/Src
  
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/libraries/SrcWrapper/src
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/libraries/SrcWrapper/inc
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/libraries/SrcWrapper/inc/LL
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/libraries/USBDevice/src
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/libraries/USBDevice/inc

  ${CMAKE_CURRENT_SOURCE_DIR}/../../variants/STM32F4xx/F405RGT_F415RGT
  ${CMAKE_CURRENT_SOURCE_DIR}/../../tools/CMSIS/5.9.0/CMSIS/Core/Include
)

target_sources(USBDeviceLib PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/libraries/USBDevice/src/cdc/cdc_queue.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/libraries/USBDevice/src/cdc/usbd_cdc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/libraries/USBDevice/src/cdc/usbd_cdc_if.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/libraries/USBDevice/src/hid/usbd_hid_composite.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/libraries/USBDevice/src/hid/usbd_hid_composite_if.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/libraries/USBDevice/src/usb_device_core.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/libraries/USBDevice/src/usb_device_ctlreq.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/libraries/USBDevice/src/usb_device_ioreq.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/libraries/USBDevice/src/usbd_conf.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/libraries/USBDevice/src/usbd_desc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/libraries/USBDevice/src/usbd_ep_conf.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/libraries/USBDevice/src/usbd_if.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/libraries/USBDevice/src/USBSerial.cpp
)

target_compile_definitions(USBDeviceLib PRIVATE 
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

target_compile_options(USBDeviceLib PRIVATE    
    -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mlittle-endian -fcommon
    -fdata-sections -ffunction-sections    
    $<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics -fno-use-cxa-atexit -fpermissive -fno-exceptions -fno-rtti -funsigned-bitfields -fshort-enums -Winline>
    # отладочная информация
    $<$<CONFIG:DEBUG>:-Os -g -g3>
)

target_link_options(USBDeviceLib PRIVATE
    -mcpu=cortex-m4 -mthumb
    -lc -lgcc
    LINKER:--no-warn-rwx-segments,--cref,--gc-sections
)
