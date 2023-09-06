#pragma once

#include "Defines.h"


class CSPI
{
	void(*_up)();
	void(*_down)();

public:
	void Initialize(void(*up)(), void(*down)());

	void Up();
	void Down();

    static uint8_t Write( uint8_t data );
    static uint8_t Read();

    // Deselect the card and release SPI bus
	static void Release() { Read(); }
};
