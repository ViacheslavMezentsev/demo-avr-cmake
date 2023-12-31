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
#ifdef UDR0

    loop_until_bit_is_set( UCSR0A, UDRE0 );

    UDR0 = c;

#elif defined( UDR1 )

    loop_until_bit_is_set( UCSR1A, UDRE1 );

    UDR1 = c;

#endif

    return 0;
}


/**
 * @brief   Выполняет чтение символа из последовательного порта.
 * 
 * @param   stream
 * @return  int 
 * 
 */
static int uart_getc( [[maybe_unused]] FILE * stream )
{
#ifdef UDR0

    loop_until_bit_is_set( UCSR0A, RXC0 );

    return UDR0;

#elif defined( UDR1 )

    loop_until_bit_is_set( UCSR1A, RXC1 );

    return UDR1; 

#endif
}


/**
 * @brief   Выполняет настройку USART.
 * 
 */
void USART_Init()
{
#ifdef UDR0

    // Включение режима двойной скорости.
    UCSR0A |= _BV( U2X0 );

    // Устанавливаем скорость.
    UBRR0H = BAUD_REG_VALUE >> 8;
    UBRR0L = BAUD_REG_VALUE;

    // Включаем передатчик и приёмник USART0.
    UCSR0B = _BV( TXEN0 ) | _BV( RXEN0 );

    // Формат обмена: 8-N-1.
    UCSR0C = _BV( UCSZ01 ) | _BV( UCSZ00 );

#elif defined( UDR1 )

    // Включение режима двойной скорости.
    UCSR1A |= _BV( U2X1 );

    // Устанавливаем скорость.
    UBRR1H = BAUD_REG_VALUE >> 8;
    UBRR1L = BAUD_REG_VALUE;

    // Включаем передатчик и приёмник USART1.
    UCSR1B = _BV( TXEN1 ) | _BV( RXEN1 );

    // Формат обмена: 8-N-1.
    UCSR1C = _BV( UCSZ11 ) | _BV( UCSZ10 );

#endif

    static FILE uart_output = {};
    fdev_setup_stream( & uart_output, uart_putc, nullptr, _FDEV_SETUP_WRITE );
    stdout = & uart_output;

    static FILE uart_input = {};    
    fdev_setup_stream( & uart_input, nullptr, uart_getc, _FDEV_SETUP_READ );    
    stdin = & uart_input;
}
