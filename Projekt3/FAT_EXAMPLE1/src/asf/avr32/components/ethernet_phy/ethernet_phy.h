/**
 * \file
 *
 * \brief Ethernet Phy management
 *
 * Copyright (c) 2010 Atmel Corporation. All rights reserved.
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
 */
#ifndef _ETHERNET_PHY_H_INCLUDED
#define _ETHERNET_PHY_H_INCLUDED

#include "compiler.h"
#include "board.h"

#if defined( PHY_DP83848 )
#include "dp83848/dp83848.h"
#elif defined( PHY_RTL8201 )
#include "rtl8201/rtl8201.h"
#else
#warning "No ethernet phy currently selected: expect build errors! Please choose one of the supported ethernet phy module."
#include "dummy_phy/dummy_phy.h"
#endif

/**
 *
 * \defgroup ethernet_phy_group Ethernet Phy
 *
 * This is the common API for Ethernet Phy on AVRs. Additional features are available
 * in the documentation of the specific modules.
 *
 * \section ethernet_phy_group_platform Platform Dependencies
 *
 * The ethernet_phy API is partially chip- or platform-specific. While all
 * platforms provide mostly the same functionality, there are some
 * variations around how different bus types and clock tree structures
 * are handled.
 *
 * The following functions are available on all platforms, but there may
 * be variations in the function signature (i.e. parameters) and
 * behaviour. These functions are typically called by platform-specific
 * parts of drivers, and applications that aren't intended to be
 * portable:
 *   - ethernet_phy_hw_reset()
 *   - ethernet_phy_sw_reset()
 *   - ethernet_phy_setup_rmii()
 *   - ethernet_phy_setup_auto_negociation()
 *
 * @{
 */

//! @}

#endif /* _ETHERNET_PHY_H_INCLUDED */
