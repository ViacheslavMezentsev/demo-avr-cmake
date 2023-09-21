#include <stdio.h>
#include <compat/ina90.h>
#include <compat/deprecated.h>
#include <util/atomic.h>
#include <avr/pgmspace.h>

extern void Timer0Init();
extern void USART_Init();
extern unsigned long millis();

#define F(s) PSTR(s)

#define SETBIT(x,y) ( (x) |= ( 1U << (y)) )
#define TOGGLE(x,y) ( (x) ^= ( 1U << (y)) )
#define CHECKBIT(x,y) ( (x) & ( 1U << (y) ) )

unsigned long msec { millis() };

/**
 * @brief   Выполняет настройку.
 * 
 */
void setup()
{
    // Настройка таймера 0.
    Timer0Init();

    // Настройка USART.
    USART_Init();

    SETBIT( DDRB, PB5 );
}


/**
 * @brief   Выполняет тело цикла.
 * 
 */
void loop()
{
    if ( millis() - msec > 1000U )
    {
        msec = millis();

        // Мигаем светодиодом.
        SETBIT( PINB, PB5 );

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
    }
}


/**
 * @brief   Точка входа.
 * 
 */
int main( void )
{
    ATOMIC_BLOCK( ATOMIC_FORCEON )
    {
        setup();
    }

    while (1)
    {
        loop();
    }
}
