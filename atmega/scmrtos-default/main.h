#pragma once

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <scmRTOS.h>
#include <os_kernel.h>

#include "scmRTOS_CONFIG.h"
#include "scmRTOS_extensions.h"
#include "scmRTOS_TARGET_CFG.h"

#define F(s) PSTR(s)

extern uint32_t seconds;
extern void USART_Init();

using namespace OS;
