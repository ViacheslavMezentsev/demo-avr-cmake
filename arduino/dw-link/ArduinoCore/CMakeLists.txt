cmake_minimum_required(VERSION 3.19)

add_library(ArduinoCore OBJECT)

add_library(ArduinoCore::avr ALIAS ArduinoCore)

target_include_directories(ArduinoCore PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/variants/eightanaloginputs)

target_sources(ArduinoCore PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino/hooks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino/WInterrupts.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino/wiring.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino/wiring_analog.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino/wiring_digital.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino/wiring_pulse.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino/wiring_shift.c
    
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino/abi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino/CDC.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino/HardwareSerial.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino/HardwareSerial0.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino/HardwareSerial1.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino/HardwareSerial2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino/HardwareSerial3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino/IPAddress.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino/new.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino/PluggableUSB.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino/Print.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino/Stream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino/Tone.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino/USBCore.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino/WMath.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino/WString.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/ArduinoCore-avr/cores/arduino/wiring_pulse.S
)

target_compile_definitions(ArduinoCore PRIVATE 
    F_CPU=16000000UL
    ARDUINO=10607
    ARDUINO_AVR_NANO
    ARDUINO_ARCH_AVR
)

target_compile_options(ArduinoCore PRIVATE    
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
    -flto
    -ffunction-sections -fdata-sections
    # отладочная информация
    $<$<CONFIG:DEBUG>:-gdwarf-2 -gstrict-dwarf>
    # c/cxx флаги    
    $<$<COMPILE_LANGUAGE:CXX>:-fpermissive -fno-exceptions -funsigned-bitfields -fno-threadsafe-statics>
    # GCC 12 AVR array subscript 0 is outside array bounds
    $<$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,12.0>:--param=min-pagesize=0>
)

target_link_options(ArduinoCore PRIVATE
    -mmcu=${MCU}    
    -Wl,--defsym=__TEXT_REGION_LENGTH__=32K
    -Wl,--defsym=__DATA_REGION_LENGTH__=2K
    -Wl,--defsym=__EEPROM_REGION_LENGTH__=1K
    -Wl,-Map=${PROJECT_NAME}.map,--cref,--gc-sections,--print-memory-usage
    #-Wl,--verbose
)
