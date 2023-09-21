#include <cstdio>
#include <util/delay.h>
#include <avr/io.h>
#include "etl/string.h"

extern void USART_Init();

etl::string<32> hello( "Hello from AVR!" );


/**
 * @brief   Выполняет настройку.
 * 
 */
void setup()
{
    // Настройка USART.
    USART_Init();
}


/**
 * @brief   Выполняет тело цикла.
 * 
 */
void loop()
{
    printf( "%s\n", hello.c_str() );

    _delay_ms( 1000 );
}


/**
 * @brief   Точка входа.
 * 
 */
int main( void )
{
    setup();

    while (1)
    {
        loop();
    }
}
