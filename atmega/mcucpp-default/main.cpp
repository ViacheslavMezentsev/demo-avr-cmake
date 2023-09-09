#include <avr/interrupt.h>
#include <iopins.h>
#include <pinlist.h>
#include <timers.h>

using namespace Mcucpp;
using namespace Timers;
using namespace IO;

#define clockCyclesPerMicrosecond()     ( F_CPU / 1000000UL )
#define clockCyclesToMicroseconds(a)    ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a)    ( (a) * clockCyclesPerMicrosecond() )

// Делитель настроен так, чтобы таймер 0 тикал каждые 64 такта,
// а обработчик переполнения вызывается каждые 256 тактов.
#define MICROSECONDS_PER_TIMER0_OVERFLOW ( clockCyclesToMicroseconds( 64 * 256 ) )

typedef IO::Pb5 Led;

volatile unsigned int count = 0;

/**
 * @brief   Обработчик прерывания TIMER0_OVF.
 * 
 */
ISR( TIMER0_OVF_vect )
{
    count++;

    // Отсчитываем интервал 1 сек.
    if ( count > 1000000UL / MICROSECONDS_PER_TIMER0_OVERFLOW )
    {
        count = 0;

        // Мигаем светодиодом.
        Led::Toggle();  
    }
}


/**
 * @brief   Выполняет настройку.
 * 
 */
void setup()
{
    // Настраиваем порт на вывод.
    Led::SetConfiguration( Led::Port::Out );

    Led::Clear();

    Timer0::Start( Timer0::Div64 );

    Timer0::EnableInterrupt();
}


/**
 * @brief   Выполняет тело цикла.
 * 
 */
void loop()
{
}


/**
 * @brief   Точка входа.
 * 
 */
int main()
{
    setup();

    sei();

    while (1)
    {
        loop();
    }
}
