#include <stdio.h>
#include <avr/sfr_defs.h>
#include <avr/io.h>

#define BAUD_RATE           57600U
#define BAUD_REG_VALUE      ( ( F_CPU / 4U / BAUD_RATE - 1U ) / 2U )


/**
 * @brief   Выполняет запись символа в последовательный порт.
 * 
 * @param c         символ 
 * @param stream    указатель на структуру FILE
 * @return int 
 * 
 */
static int uart_putc( char c, [[maybe_unused]] FILE * stream )
{
    loop_until_bit_is_set( UCSR1A, UDRE1 );

    UDR1 = c;

    return 0;
}


/**
 * @brief   Выполняет чтение символа из последовательного порта.
 * 
 * @param stream
 * @return int 
 * 
 */
static int uart_getc( [[maybe_unused]] FILE * stream )
{
    loop_until_bit_is_set( UCSR1A, RXC1 );

    return UDR1;
}


/**
 * @brief   Выполняет настройку USART1.
 * 
 */
void USART1Init()
{
    // Включение режима двойной скорости.
    UCSR1A |= _BV( U2X1 );

    // Устанавливаем скорость.
    UBRR1H = BAUD_REG_VALUE >> 8;
    UBRR1L = BAUD_REG_VALUE;

    // Включаем передатчик и приёмник USART1.
    UCSR1B = _BV( TXEN1 ) | _BV( RXEN1 );

    // Формат обмена: 8-N-1.
    UCSR1C = _BV( UCSZ11 ) | _BV( UCSZ10 );

    static FILE uart_output = {};
    fdev_setup_stream( & uart_output, uart_putc, nullptr, _FDEV_SETUP_WRITE );
    stdout = & uart_output;

    static FILE uart_input = {};    
    fdev_setup_stream( & uart_input, nullptr, uart_getc, _FDEV_SETUP_READ );    
    stdin = & uart_input;
}
