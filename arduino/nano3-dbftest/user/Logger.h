#pragma once

#include "Defines.h"


class CLogger : public Print
{
    char _buffer[ 256 ];

public:   

    size_t write( uint8_t );    

    size_t println( char * format, ... );
    size_t println( PGM_P format, ... );
};

extern CLogger Logger;
