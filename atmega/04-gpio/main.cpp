#include <util/delay.h>
#include <avr/io.h>

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
 * @brief   Точка входа.
 * 
 */
int main()
{
    // Настраиваем порт.
    SetBit( _SFR_IO_ADDR( DDRB ), PB5 );
    ClrBit( _SFR_IO_ADDR( PORTB ), PB5 );

    // Мигаем светодиодом.
    while (1)
    {    
#if 1
        // Writing a '1' to PINxn toggles the value of PORTxn, independent on the value of DDRxn.
        // The SBI instruction can be used to toggle one single bit in a port.
        PINB |= _BV( PB5 );
#else    
        // Или так.
        if ( bit_is_set( PINB, PB5 ) )
        {
            ClrBit( _SFR_IO_ADDR( PORTB ), PB5 );
        }
        else
        {
            SetBit( _SFR_IO_ADDR( PORTB ), PB5 );
        }
#endif
        _delay_ms( 1000 );
    }
}
