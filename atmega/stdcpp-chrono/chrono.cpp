﻿/*
 * Copyright (c) 2022, Christopher Kormanyos
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <chrono>

#define SETBIT(x,b)     ( (x) |= _BV(b) )
#define SETBITS(x,b)    ( (x) |= (b) )
#define CLRBIT(x,b)     ( (x) &= ~_BV(b) )
#define TOGGLE(x,b)     ( (x) ^= _BV(b) )
#define CHECKBIT(x,b)   ( (x) & _BV(b) )

static void app_hw_init()
{
    // Initialize the application including WDT, PORTB.5 and TIMER0

    // We will now disable the watchdog.
    // Service the watchdog just to be sure to avoid pending timeout.
    wdt_reset();

    // Clear WDRF in MCUSR.
    CLRBIT( MCUSR, WDRF );

    // Write logical one to WDCE and WDE.
    // Keep the old prescaler setting to prevent unintentional time-out.
    SETBITS( WDTCSR, _BV( WDCE ) | _BV( WDE ) );

    // Turn off the WDT.
    WDTCSR = 0x00;

    // We will now initialize PORTB.5 to be used as an LED driver port.
    // Set PORTB.5 value to low.
    CLRBIT( PORTB, PORTB5 );

    // Set PORTB.5 direction to output.
    SETBIT( DDRB, DDB5 );

    // We will now initialize the TIMER0 clock and interrupt.
    // Clear the TIMER0 overflow flag.
    TIFR0 = _BV( TOV0 );

    // Enable the TIMER0 overflow interrupt.
    TIMSK0 = _BV( TOIE0 );

    // Set the TIMER0 clock source to f_osc/8 = 2MHz and begin counting.
    TCCR0B = _BV( CS01 );

    // Enable all interrupts.
    sei();
}


/**
 * @brief   Точка входа.
 * 
 */
int main()
{
    // Initialize the application hardware. This includes WDT, PORTB.5 and TIMER0.
    app_hw_init();

    for(;;)
    {
        using namespace std::chrono;

        // Toggle the LED on portb.5.
        SETBIT( PINB, PB5 );

        // Make use of <chrono> to insert a 1s delay (i.e., 1000 milliseconds).
        const auto start = high_resolution_clock::now();

        auto delta = duration_cast<milliseconds>( milliseconds() );

        do
        {
            delta = duration_cast<milliseconds>( high_resolution_clock::now() - start );
        }
        while ( delta < milliseconds( 1000U ) );
    }
}

namespace {

std::uint64_t gpt_system_tick;
std::uint64_t gpt_get_time_elapsed();

std::uint64_t gpt_get_time_elapsed()
{
    // The entire system-tick is composed of the static
    // 64-bit variable mcal_gpt_system_tick and the 8-bit
    // timer register TCNT0. These are concatenated together
    // in software as a cohesive, consistent 64-bit tick.

    // This subroutine returns the concatenated 64-bit
    // mcal_gpt_system_tick/TCNT0 64-bit system tick.
    // A multiple read of the tick is used in order
    // to ensure data consistency.

    // Do the first read of the TIMER0 counter and the system tick.
    const auto tim0_cnt_1 = TCNT0;
    const auto sys_tick_1 = gpt_system_tick;

    // Do the second read of the TIMER0 counter.
    const auto tim0_cnt_2 = TCNT0;

    // Perform the consistency check to obtain the concatenated,
    // consistent, 64-bit system-tick.
    const auto consistent_microsecond_tick =
        ((tim0_cnt_2 >= tim0_cnt_1)
            ? static_cast<std::uint64_t>(sys_tick_1      | static_cast<std::uint8_t>(tim0_cnt_1 >> 1U))
            : static_cast<std::uint64_t>(gpt_system_tick | static_cast<std::uint8_t>(tim0_cnt_2 >> 1U)));

    return consistent_microsecond_tick;
}
}

ISR( TIMER0_OVF_vect )
{
    // Increment the 64-bit system tick with 0x80, representing 128 microseconds.
    gpt_system_tick += UINT8_C( 0x80 );
}

auto std::chrono::high_resolution_clock::now() noexcept -> std::chrono::high_resolution_clock::time_point
{
    // Implement std::chrono::high_resolution_clock::now()
    // for the standard library's high-resolution clock.

    // Your project must supply a monotonic clock counting up.
    // In this example, this is done with gpt_get_time_elapsed()
    // which returns a 64-bit unsigned integral value representing
    // a tick having microsecond resolution.

    // The source of the high-resolution clock is microseconds.
    using microsecond_time_point_type = std::chrono::time_point<high_resolution_clock, microseconds>;

    // Obtain a time-point with microsecond resolution.
    const auto time_point_in_microseconds =
        microsecond_time_point_type( microseconds( gpt_get_time_elapsed() ) );

    // And return the corresponding duration with microsecond resolution.
    return time_point_cast<duration>( time_point_in_microseconds );
}
