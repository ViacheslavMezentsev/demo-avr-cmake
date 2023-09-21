#include <util/atomic.h>
#include <avr/interrupt.h>
#include <iopins.h>
#include <pinlist.h>
#include <timers.h>
#include <Timeout.h>

using namespace Mcucpp;
using namespace Timers;
using namespace IO;

#define clockCyclesPerMicrosecond()     ( F_CPU / 1000000UL )
#define clockCyclesToMicroseconds(a)    ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a)    ( (a) * clockCyclesPerMicrosecond() )

// Делитель настроен так, чтобы таймер 0 тикал каждые 64 такта,
// а обработчик переполнения вызывается каждые 256 тактов.
#define MICROSECONDS_PER_TIMER0_OVERFLOW ( clockCyclesToMicroseconds( 64 * 256 ) )

typedef Pb5 Led;
TimeoutCounter<uint16_t> Counter {0};

/**
 * @brief   Обработчик прерывания TIMER0_OVF.
 * 
 */
ISR( TIMER0_OVF_vect )
{
    Counter.Tick();

    if ( Counter.IsTimeout() )
    {
        Counter.Reset();

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
    Led::SetDirWrite();
    Led::Clear();

    Timer0::Start( Timer0::Div64 );
    Timer0::EnableInterrupt();

    // Настраиваем счётчик на 1 секунду.
    Counter.Set( 1000000UL / MICROSECONDS_PER_TIMER0_OVERFLOW );    
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
    ATOMIC_BLOCK( ATOMIC_FORCEON )
    {
        setup();
    }

    while (1)
    {
        loop();
    }
}
