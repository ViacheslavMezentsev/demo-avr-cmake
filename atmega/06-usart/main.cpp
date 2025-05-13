#include <stdio.h>
#include <compat/ina90.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <avr/pgmspace.h>

extern void USART_Init();

#define F(s) PSTR(s)


/**
 * @brief   Выполняет установку бита регистра в памяти.
 * 
 * @param addr  относительный адрес регистра ввода/вывода
 * @param pin   номер бита
 * 
 */
void SetBit( uint16_t addr, uint8_t pin )
{
    _SFR_IO8( addr ) |= _BV( pin );
}


/**
 * @brief   Выполняет сброс бита регистра в памяти.
 * 
 * @param addr  относительный адрес регистра ввода/вывода
 * @param pin   номер бита
 * 
 */
void ClrBit( uint16_t addr, uint8_t pin )
{
    _SFR_IO8( addr ) &= ~_BV( pin );
}


/**
 * @brief   Выполняет инверсию бита регистра в памяти.
 * 
 * @param addr  относительный адрес регистра ввода/вывода
 * @param pin   номер бита
 * 
 */
void ToggleBit( uint16_t addr, uint8_t pin )
{
    _SFR_IO8( addr ) ^= _BV( pin );
}


/**
 * @brief   Выполняет настройку.
 * 
 */
void setup()
{
    ATOMIC_BLOCK( ATOMIC_FORCEON )
    {
        // Настройка USART.
        USART_Init();

        // Настраиваем порт.
        SetBit( _SFR_IO_ADDR( DDRB ), PB5 );
    }
}


/**
 * @brief   Выполняет тело цикла.
 * 
 */
void loop()
{
    // Мигаем светодиодом.
    ToggleBit( _SFR_IO_ADDR( PORTB ), PB5 );

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
