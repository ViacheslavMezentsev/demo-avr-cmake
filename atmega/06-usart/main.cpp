#include <stdio.h>
#include <compat/ina90.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <avr/pgmspace.h>
#include <avr/io.h>

extern void USART0Init();

#define F(s) PSTR(s)


/**
 * @brief   Выполняет установку бита регистра в памяти.
 * 
 * @param reg   адрес регистра
 * @param pin   номер бита
 * 
 */
void SetBit( volatile uint8_t * reg, uint8_t pin )
{
    reg[0] |= _BV( pin );
}


/**
 * @brief   Выполняет сброс бита регистра в памяти.
 * 
 * @param reg   адрес регистра
 * @param pin   номер бита
 * 
 */
void ClrBit( volatile uint8_t * reg, uint8_t pin )
{
    reg[0] &= ~_BV( pin );
}


/**
 * @brief   Выполняет инверсию бита регистра в памяти.
 * 
 * @param reg   адрес регистра
 * @param pin   номер бита
 * 
 */
void ToggleBit( volatile uint8_t * reg, uint8_t pin )
{
    reg[0] ^= _BV( pin );
}


/**
 * @brief   Выполняет настройку.
 * 
 */
void setup()
{
    ATOMIC_BLOCK( ATOMIC_FORCEON )
    {
        // Настройка USART0.
        USART0Init();

        // Настраиваем порт.
        SetBit( & DDRB, PB5 );
    }
}


/**
 * @brief   Выполняет тело цикла.
 * 
 */
void loop()
{
    // Мигаем светодиодом.
    ToggleBit( & PORTB, PB5 );

    // Ожидаем выполнение команды.
    _NOP();

    if ( bit_is_set( PINB, PB5 ) )
    {
        // Выводим сообщение из ОЗУ.
        printf( "Led on\n" );

        // Выводим сообщение из FLASH.
        //printf_P( F( "Led on\n" ) );
    }
    else
    {
        // Выводим сообщение из ОЗУ.
        //printf( "Led off\n" );

        // Выводим сообщение из FLASH.
        printf_P( F( "Led off\n" ) );
    }

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
