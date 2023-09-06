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
 * @brief   Точка входа.
 * 
 */
int main()
{
    // Настраиваем порт.
    SetBit( & DDRB, PB5 );
    ClrBit( & PORTB, PB5 );

    while (1)
    {
        // Мигаем светодиодом.
        if ( bit_is_set( PINB, PB5 ) )
        {
            ClrBit( & PORTB, PB5 );
        }
        else
        {
            SetBit( & PORTB, PB5 );
        }

        _delay_ms( 1000 );
    }
}
