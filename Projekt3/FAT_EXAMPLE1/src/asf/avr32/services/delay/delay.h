/*****************************************************************************
 *
 * \file
 *
 * \brief AT32UC3 delay management header file.
 *
 * This file contains definitions and services to handle "delays".
 *
 * Copyright (c) 2009-2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 ******************************************************************************/


#ifndef _DELAY_H_
#define _DELAY_H_

/**
 * \defgroup group_avr32_services_basic_delay Delay functions
 *
 * Driver for busy-waiting. Supports delaying a number of milliseconds,
 * and works in both standalone and with FreeRTOS.
 *
 * \{
 */

#include "compiler.h"
#ifdef FREERTOS_USED
# include "FreeRTOS.h"
# include "task.h"
#else
# include "cycle_counter.h"
#endif

/*!
 * \brief Initialize the delay driver.
 *
 * \param  fcpu_hz: CPU frequency in Hz.
 */
extern void delay_init(unsigned long fcpu_hz);


/*!
 * \brief Waits during at least the specified delay (in millisecond) before returning.
 *
 * Note that in the case of FreeRTOS, the function will delay the current task for a given number of ms.
 *
 * \param  delay:   Number of millisecond to wait.
 */
extern void delay_ms(unsigned long delay);

/**
 * \}
 */

#endif  // _DELAY_H_
