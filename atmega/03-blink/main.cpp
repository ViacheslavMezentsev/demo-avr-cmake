#include <util/delay.h>
#include <avr/io.h>


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
 * @brief   Выполняет настройку.
 * 
 */
void setup()
{
    // Настраиваем порт.
    SetBit( & DDRB, PB5 );
}


/**
 * @brief   Выполняет тело цикла.
 * 
 */
void loop()
{
    // Мигаем светодиодом.
    // Writing a '1' to PINxn toggles the value of PORTxn, independent on the value of DDRxn.
    // The SBI instruction can be used to toggle one single bit in a port.
#if 1
    PINB = _BV( PB5 );
#else    
    // Или так.
    if ( bit_is_set( PINB, PB5 ) )
    {
        ClrBit( & PORTB, PB5 );
    }
    else
    {
        SetBit( & PORTB, PB5 );
    }
#endif

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
