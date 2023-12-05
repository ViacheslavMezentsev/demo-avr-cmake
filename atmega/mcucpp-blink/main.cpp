#include <iopins.h>
#include <delay.h>

using namespace Mcucpp::IO;

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

    // Выполняем синхронную задержку.
    Mcucpp::delay_ms<1000, F_CPU>();
}


/**
 * @brief   Точка входа.
 * 
 */
int main()
{
    setup();

    while (1)
    {
        loop();
    }
}
