#pragma once

// Внимание:
// Для VS20xx нужно сделать явное определение, чтобы работала
// автоматическая подсветка логики препроцессора в IDE.
// Это делается в настройках проекта:
// IntelliSence\Preprocessor Definitions: __GNUC__ или __ICCAVR__

#define _DEBUG_

// AVR GCC.
#ifdef __GNUC__

    #include <inttypes.h>
    #include <stdint.h>

    #include <stdlib.h>
    #include <setjmp.h>
    #include <string.h>
    #include <stdio.h>
    #include <time.h>

    #include <ctype.h>
    #include <math.h>
    #include <util/delay.h>
    #include <util/atomic.h>

    #include <compat/ina90.h>
    #include <compat/deprecated.h>

    #include <avr/pgmspace.h>
    #include <avr/eeprom.h>
    #include <avr/interrupt.h>
    #include <avr/io.h>
//    #include <avr/iom2560.h>

    #define nop() _NOP()
    #define __disable_interrupt() cli()
    #define __enable_interrupt() sei()
    #define __save_interrupt() SREG
    #define __restore_interrupt(x) SREG = x

    #define longjump( addr ) asm volatile( "jmp %0" :: "i" ( addr ) );

    #define FCHAR_PTR FlashPtr< const char >
    #define SPSTR(s) (__extension__({ \
                static const char __c[] PROGMEM = (s); \
                FlashPtr< const char > _c(__c); _c; }))

    #define F(s) PSTR(s)    

    #define FSTR(s) (__extension__({static const char __c[] PROGMEM = (s); ( PGM_P ) &__c[0];}))

#elif defined( __ICCAVR__ )

    #pragma hdrstop

    #include <intrinsics.h>
    #include <stdint.h>
    #include <pgmspace.h>

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #include <iom2560.h>
    #include <ina90.h>

    // TODO: Подумать об аналогах этих задержек (как в AVR GCC)
    // Динамический вызов тоже может пригодиться.
    // Внимание: Указывайте округлённые до целых значения,
    // т.к. дробная часть в отличие от GCC будет отбрасываться.
    // Это надо как-то исправить для кросс-компиляции.
    #define _delay_us(us) __delay_cycles( ( uint32_t ) (us) * ( F_CPU / 1000000UL ) )
    #define _delay_ms(ms) __delay_cycles( ( uint32_t ) (ms) * ( F_CPU / 1000UL ) )

    #define _BV(x) ( 1 << x )

    #define cbi(port,x) port &= ~( 1 << x )
    #define sbi(port,x) port |= ( 1 << x )

    #define outb(port,byte) port = byte
    #define inp(port) (port)

    #define utoa(num,buf,rad) sprintf(buf, "%i", num)
    #define nop() __no_operation()
    #define sleep() __sleep()

#endif

// Программа.
#ifdef __cplusplus

#include "SmartPtr.h"
//#include "Print.h"
//#include "Streaming.h"
//#include "Logger.h"

//#define loginfo( fmt, ... ) Logger.println( F( "[INFO ] " fmt ), ##__VA_ARGS__ )
//#define logdebug( fmt, ... ) Logger.println( F( "[DEBUG] " fmt ), ##__VA_ARGS__ )
//#define logerror( fmt, ... ) Logger.println( F( "[ERROR] " fmt ), ##__VA_ARGS__ )

#endif

/***********************************************************************
** MACROS:      PR_BEGIN_EXTERN_C
**              PR_END_EXTERN_C
** DESCRIPTION:
**      Macro shorthands for conditional C++ extern block delimiters.
***********************************************************************/
#ifdef __cplusplus
#define PR_BEGIN_EXTERN_C       extern "C" {
#define PR_END_EXTERN_C         }
#else
#define PR_BEGIN_EXTERN_C
#define PR_END_EXTERN_C
#endif

#define PACKED __attribute__((__packed__))
#define TOGGLE(x,y) ( (x) ^= ( 1 << (y)) )
#define CHECKBIT(x,y) ( (x) & ( 1 << (y) ) )


// Definitions for MMC/SDC connection
#define SD_DI   4   // MOSI
#define SD_DO   3   // MISO
#define SD_CLK  2   // CLK
#define SD_CS   5   // SS
//#define SD_INS  0   // CD
//#define SD_WP   1   // WP	

#define W5100_CS   4   // W5100 chip select

#define BAUD_RATE 57600
#define BAUD_REG_VALUE ( ( F_CPU / 4 / BAUD_RATE - 1 ) / 2 )

// AVRJazz Mega168/328 SPI I/O
#define SPI_PORT    PORTB
#define SPI_DDR     DDRB
#define SPI_CS      PORTB5

// Wiznet W5100 Register Addresses
#define MR 0x0000 // Mode Register
#define GAR 0x0001 // Gateway Address: 0x0001 to 0x0004
#define SUBR 0x0005 // Subnet mask Address: 0x0005 to 0x0008
#define SAR 0x0009 // Source Hardware Address (MAC): 0x0009 to 0x000E
#define SIPR 0x000F // Source IP Address: 0x000F to 0x0012
#define RMSR 0x001A // RX Memory Size Register
#define TMSR 0x001B // TX Memory Size Register

extern unsigned long millis();
