/*
 * Copyright (c) 2013-2015, 2017-2018, Niklas Hauser
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
#include <modm/io/iostream.hpp>
#include <modm/driver/adc/adc_sampler.hpp>

using namespace modm::platform;
using namespace modm::literals;
using namespace std::chrono_literals;

// Create a new UART object
// Create a IOStream for complex formatting tasks
modm::IODeviceWrapper<Uart0, modm::IOBuffer::BlockIfFull> serial;
modm::IOStream output( serial );

// the three sensors are mapped: x = ch1, y = ch2, z = ch0
Adc::Channel sensorMapping[3] = { 1, 2, 0 };

// 3 канала по 32 отсчёта с усреднением.
typedef modm::AdcSampler<AdcInterrupt, 3, 32> sensors;

modm::ShortTimeout timeout( 100ms );

/**
 * @brief   Точка входа.
 * 
 */
int main()
{
    Uart0::connect<GpioOutputD1::Txd, GpioInputD0::Rxd>();
    Uart0::initialize<SystemClock, 19200_Bd>();

    // Initialize the analog to digital converter
    // With the AVR running at 16Mhz and a prescaler of 128 the
    // ADC is running at 125kHz.
    Adc::initialize<SystemClock, 125_kHz>();
    Adc::setReference( Adc::Reference::InternalVcc );
    Adc::enableInterrupt();

    // Enable interrupts, this is needed for every buffered UART
    enableInterrupts();

    output << "Welcome!" << modm::endl;

    // Буфер результатов.
    sensors::DataType sensorData[3];

    sensors::initialize( sensorMapping, sensorData );
    sensors::startReadout();

    while ( true )
    {
        if ( timeout.isExpired() && sensors::isReadoutFinished() )
        {
            auto data = sensors::getData();

            // Передаём по UART.
            output << "x=" << data[0] << modm::endl;
            output << "y=" << data[1] << modm::endl;
            output << "z=" << data[2] << modm::endl;

            // Запускаем следующее чтение.
            sensors::startReadout();
            timeout.restart( 200ms );
        }
    }
}
