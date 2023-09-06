#include <stdio.h>
#include "main.h"

typedef process< pr0, 128 > CProcess1;

CProcess1 Process1;

template<> OS_PROCESS 
void CProcess1::exec()
{
    printf_P( F( "Process1: setup.\n" ) );

    for (;;)
    {
        sleep( 1500 );

        printf_P( F( "%04u Process1: loop.\n" ), seconds );
    }
}
