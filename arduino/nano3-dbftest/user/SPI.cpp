#include "SPI.h"

// MMC CS -> Low.
void CSPI::Up()
{	
	if ( _up != nullptr ) _up();
}


// MMC CS -> High.
void CSPI::Down()
{
	if ( _down != nullptr ) _down();
}


void CSPI::Initialize( void (* up)(), void (* down)() )
{
    _up = up;
    _down = down;

    // Отключаем SD-карту.
    DDRB |= ( 1 << SD_CS );
    PORTB |= ( 1 << SD_CS );

    // Отключаем W5100.
    DDRB |= _BV( SD_DI ) | _BV( SD_CLK ) | _BV( W5100_CS );
    PORTB |= _BV( SD_DO ) | _BV( SD_DI ) | _BV( W5100_CS );
}


// Послать байт.
uint8_t CSPI::Write( uint8_t data ) 
{
    for ( uint8_t n = 0; n < 8; n++ )
    {
        if ( data & 0x80 )
        {
            PORTB |= _BV( SD_DI );
        }
        else
        {
            PORTB &= ~_BV( SD_DI );
        }

        data <<= 1;

        PORTB |= _BV( SD_CLK );
        nop();
        nop();
        nop();
        nop();
        
        PORTB &= ~_BV( SD_CLK );
        nop();
        nop();
        nop();
    }

    return data;
}


// Принять байт.
uint8_t CSPI::Read() 
{
    uint8_t data = 0;
    
    PORTB |= _BV(SD_DI);

    for (uint8_t n = 0; n < 8; n++)
    {
        PORTB |= _BV(SD_CLK);
        nop();
        nop();

        data <<= 1;

        if (CHECKBIT(PINB, SD_DO)) data |= 0x01;

        PORTB &= ~_BV(SD_CLK);

        nop();
        nop();
        nop();
    }

    return data;
}
