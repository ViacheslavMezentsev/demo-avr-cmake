/*
 * Copyright (c) 2010-2011, Fabian Greif
 * Copyright (c) 2012-2013, 2015-2017, Niklas Hauser
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
#include <modm/processing/timer.hpp>

using namespace modm::platform;
using namespace std::chrono_literals;

// Определяем вывод для светодиода.
typedef GpioB0 Led;

/**
 * @brief   Точка входа.
 * 
 */
int main()
{
    // Настраиваем аппаратный таймер.
    SystemClock::enable();

    // Устанавливаем начальный уровень для светодиода.
    Led::setOutput();
    Led::reset();

    // Разрешаем глобальные прерывания.
    enableInterrupts();

    modm::ShortTimeout timeout( 200ms );

    while ( true )
    {
        if ( timeout.isExpired() )
        {
            timeout.restart( 200ms );

            Led::toggle();
        }
    }
}
