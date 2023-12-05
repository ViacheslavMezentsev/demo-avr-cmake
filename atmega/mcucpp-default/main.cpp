#include <delay.h>

volatile uint8_t count {0};

/**
 * @brief   Выполняет настройку.
 * 
 */
void setup()
{
    count = 0;
}


/**
 * @brief   Выполняет тело цикла.
 * 
 */
void loop()
{
    count++;

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
