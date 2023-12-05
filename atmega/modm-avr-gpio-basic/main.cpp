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

// Определяем обычный вывод для светодиода.
typedef GpioB0 Led;

// Определяем инвертированный вывод для светодиода.
typedef GpioInverted<GpioB1> LedInverted;

/**
 * @brief   Точка входа.
 * 
 */
int main()
{
    // Зажигаем светодиод с прямой логикой вывода.
    Led::setOutput();
    Led::set();

    // Зажигаем светодиод с инвертированной логикой вывода.
    LedInverted::setOutput( modm::Gpio::Low );

    while ( true )
    {
    }
}
