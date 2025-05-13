#include <vector>
#include <cstdio>
#include <algorithm>

extern void USART_Init();

/**
 * @brief   Точка входа.
 * 
 */
int main()
{
    USART_Init();

    puts( "AVR libstdc++ vector test\n" );

    std::vector<uint8_t> test{ 10, 1, 2, 42, 3 };

    test.push_back( 4 );
    test.erase( test.begin() );

    if ( auto it = std::find( test.begin(), test.end(), 42 ); it != test.end() )
    {
        test.erase( it );
    }

    for ( auto i : test )
    {
        printf( "%i ", i );
    }

    puts( "\n" );

    while (1);
}
