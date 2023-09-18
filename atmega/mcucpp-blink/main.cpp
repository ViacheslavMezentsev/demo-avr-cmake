#include <avr/interrupt.h>
#include <iopins.h>
#include <delay.h>

using namespace Mcucpp;
using namespace IO;

typedef Pb5 Led;

/**
 * @brief   Выполняет настройку.
 * 
 */
void setup()
{
    // Настраиваем порт на вывод.
    Led::SetDirWrite();
    Led::Clear();
}


/**
 * @brief   Выполняет тело цикла.
 * 
 */
void loop()
{
    Led::Toggle();

    delay_ms<1000, F_CPU>();
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
