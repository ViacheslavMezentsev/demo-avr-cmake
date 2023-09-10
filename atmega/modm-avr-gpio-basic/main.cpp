/*
 * Copyright (c) 2010-2011, Fabian Greif
 * Copyright (c) 2013-2017, Niklas Hauser
 * Copyright (c) 2014, Sascha Schade
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#include <modm/platform.hpp>

using namespace modm::platform;

typedef GpioOutputB0 Led;
// create a wrapper for a output pin
typedef GpioInverted<GpioOutputB1> LedInverted;

int main()
{
    Led::setOutput();
    Led::set();

    LedInverted::setOutput( modm::Gpio::Low );

    while ( true )
    {
    }
}
