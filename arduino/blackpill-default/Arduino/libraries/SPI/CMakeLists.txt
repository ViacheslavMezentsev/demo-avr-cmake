cmake_minimum_required(VERSION 3.16)

target_include_directories(${PROJECT_NAME} PUBLIC
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

  ${CMAKE_CURRENT_SOURCE_DIR}/../../variants/STM32F4xx/F401CCU
  ${CMAKE_CURRENT_SOURCE_DIR}/../../tools/CMSIS/5.9.0/CMSIS/Core/Include

  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/libraries/SPI/inc
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/libraries/SPI/src
)

target_sources(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/libraries/SPI/src/SPI.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../modules/Arduino_Core_STM32/libraries/SPI/src/utility/spi_com.c
)
