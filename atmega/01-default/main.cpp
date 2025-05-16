#include <util/delay.h>
#include <avr/io.h>

volatile uint8_t count { 0 };

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

    _delay_ms( 1000 );
}


/**
 * @brief   Точка входа.
 * 
 */
int main()
{
    setup();

    while ( 1 )
    {
        loop();
    }
}
