#include <stdio.h>
#include "main.h"

typedef process< pr1, 128 > CProcess2;

CProcess2 Process2;

template<> OS_PROCESS 
void CProcess2::exec()
{
    printf_P( F( "Process2: setup.\n" ) );

    for (;;)
    {
        sleep( 3000 );
        
        printf_P( F( "%04u Process2: loop.\n" ), seconds );
    }
}
