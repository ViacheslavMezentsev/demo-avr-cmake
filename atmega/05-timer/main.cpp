#include <stdio.h>
#include <util/atomic.h>
#include <compat/ina90.h>
#include <compat/deprecated.h>
#include <avr/pgmspace.h>
#include <avr/io.h>

extern void Timer0Init();
extern void USART0Init();
extern unsigned long millis();

#define F(s) PSTR(s)

#define TOGGLE(x,y) ( (x) ^= ( 1U << (y)) )
#define CHECKBIT(x,y) ( (x) & ( 1U << (y) ) )

unsigned long msec;

/**
 * @brief   Выполняет настройку.
 * 
 */
void setup()
{
    // Настройка таймера 0.
    Timer0Init();

    // Настройка USART0.
    USART0Init();

    sbi( DDRB, PB5 );

    msec = millis();
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
        TOGGLE( PORTB, PB5 );

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
