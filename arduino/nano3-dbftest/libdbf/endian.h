#pragma once

/****************************************************************************
 * endian.h
 ****************************************************************************
 * Routines for Little Endian and Big Endian Systems
 * Library version
 *
 * Version 0.4, 2003-09-08
 * Author: Björn Berg, clergyman@gmx.de
 *
 ****************************************************************************
 * $Id: dbf_endian.h,v 1.2 2004/09/09 10:32:22 steinm Exp $
 ***************************************************************************/

#include <stdint.h>

uint16_t rotate2b(uint16_t var);
uint32_t rotate4b(uint32_t var);
