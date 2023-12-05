/*
 * Copyright (c) 2010-2011, Fabian Greif
 * Copyright (c) 2012-2014, 2017, Niklas Hauser
 * Copyright (c) 2013, Sascha Schade
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#include <modm/platform.hpp>
#include <modm/io/iostream.hpp>

using namespace modm::literals;
using namespace modm::platform;

/**
 * @brief   Точка входа.
 * 
 */
int main()
{
    Uart0::connect<GpioOutputD1::Txd, GpioInputD0::Rxd>();
    Uart0::initialize<SystemClock, 19200_Bd>();

    // Разрешаем глобальные прерывания.
    enableInterrupts();

    // Создаём поток ввода-вывода.
    modm::IODeviceWrapper<Uart0, modm::IOBuffer::BlockIfFull> serial;
    modm::IOStream stream( serial );

    // Now we can print numbers and other objects to the stream
    // The following will result in the string "24 is a nice number!\n" with
    // the number formatted as ASCII text.
    stream << 24 << " is a nice number!" << modm::endl;

    while ( true )
    {
    }
}
