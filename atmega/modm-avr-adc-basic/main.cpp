/*
 * Copyright (c) 2010-2011, Fabian Greif
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
#include <modm/io/iostream.hpp>

using namespace modm::platform;
using namespace modm::literals;
using namespace std::chrono_literals;

int
main()
{
	// Create a new UART object and configure it to a baudrate of 115200
	Uart0::connect<GpioOutputD1::Txd, GpioInputD0::Rxd>();
	Uart0::initialize<SystemClock, 19200_Bd>();

	// Enable interrupts, this is needed for every buffered UART
	enableInterrupts();

	// Create a IOStream for complex formatting tasks
	modm::IODeviceWrapper< Uart0, modm::IOBuffer::BlockIfFull > device;
	modm::IOStream output(device);

	output << "Welcome!" << modm::endl;

	// Initialize the analog to digital converter
	// With the AVR running at 16Mhz and a prescaler of 128 the
	// ADC is running at 125kHz.
	Adc::initialize<SystemClock, 125_kHz>();
	Adc::setReference(Adc::Reference::InternalVcc);

	// read the value of channel 0 (=> ADC0 => PA0) and wait until
	// it is finished
	uint16_t value = Adc::readChannel(0);

	// send it via UART
	output << value << modm::endl;

	// now lets use the asynchronous version the be able to do other stuff
	// while waiting for the result
	Adc::setChannel(0);
	Adc::startConversion();

	while (true)
	{
		if (Adc::isConversionFinished())
		{
			// send the result
			output << Adc::getValue() << modm::endl;

			// wait a bit so that the result stays readable when
			// receiving the UART output on a PC. If you want to do further
			// calculations with the results instead of sending it
			// you do not need this.
			modm::delay(500ms);

			// restart the conversion
			Adc::setChannel(0);
			Adc::startConversion();
		}
	}
}
