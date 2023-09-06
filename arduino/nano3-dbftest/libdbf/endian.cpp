/*****************************************************************************
 * endian.cpp
 *****************************************************************************
 * Routines for Little Endian and Big Endian Systems
 * Library version
 *
 * Author: Björn Berg <clergyman@gmx.de>, Uwe Steinmann <uwe@steinmann.cx>
 *
 *****************************************************************************
 * $Id: dbf_endian.c,v 1.3 2004/09/09 10:32:22 steinm Exp $
 ****************************************************************************/

#include "endian.h"

/*******************************************************************
 * Changes byte construction if dbf is used on another platform
 * than little endian. dBASE databases are written in little endian
 * format.
 *******************************************************************/

// swap 2 byte integers.
uint16_t rotate2b( uint16_t value )
{
    uint16_t tmp = value;

    auto ptmp = ( unsigned char * ) & tmp;

    return ( ( uint16_t ) ptmp[1] << 8 ) + ( uint16_t ) ptmp[0];
}


// swap 4 byte integers.
uint32_t rotate4b( uint32_t value )
{
    uint32_t tmp = value;

    auto ptmp = ( unsigned char * ) & tmp;

    return ( ( uint32_t ) ptmp[3] << 24 ) + ( ( uint32_t ) ptmp[2] << 16 ) + ( ( uint32_t ) ptmp[1] << 8 ) + ( uint32_t ) ptmp[0];
}
