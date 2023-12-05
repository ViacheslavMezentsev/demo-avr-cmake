#include <avr/interrupt.h>
#include <tiny_ostream.h>
#include <iopins.h>
#include <usart.h>
#include <delay.h>

using namespace Mcucpp;
using namespace IO;

typedef Pd1 TxD;
typedef Pb5 Led;
typedef Usart1 MyUsart;

class UsartOut
{
public:
    void put( char c )
    {
        MyUsart::Write(c);
    }
};

typedef basic_ostream<UsartOut> ostream;
ostream cout; 

/**
 * @brief   Выполняет настройку.
 * 
 */
void setup()
{
    TxD::SetDirWrite();
    Usart1::Init( 57600 );

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

    cout << ( Led::IsSet() ? "Led on\r" : "Led off\r" ) << endl;

    // Выполняем синхронную задержку.
    delay_ms<1000, F_CPU>();
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
