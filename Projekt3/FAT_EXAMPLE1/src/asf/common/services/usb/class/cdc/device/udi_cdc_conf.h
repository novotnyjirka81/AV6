/**
 * \file
 *
 * \brief Default CDC configuration for a USB Device with a single interface
 *
 * Copyright (c) 2009-2011 Atmel Corporation. All rights reserved.
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

#ifndef _UDI_CDC_CONF_H_
#define _UDI_CDC_CONF_H_

#include "usb_protocol_cdc.h"
#include "conf_usb.h"

// Check the number of port
#ifndef  UDI_CDC_PORT_NB
# define  UDI_CDC_PORT_NB 1
#endif
#if UDI_CDC_PORT_NB > 3
# error UDI_CDC_PORT_NB must be inferior or equal at 3
#endif


#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup udi_cdc_group
 * \defgroup udi_cdc_group_conf Default CDC configuration for a USB Device
 * with a single interface CDC
 *
 * @{
 */

//! Control endpoint size (Endpoint 0)
#define  USB_DEVICE_EP_CTRL_SIZE       64

#if XMEGA
/**
 * \name Endpoint configuration on XMEGA
 * The XMEGA supports a IN and OUT endpoint with the same number endpoint.
 */
//@{
//! Endpoints' numbers used by single or first CDC port
#define  UDI_CDC_DATA_EP_IN            (1 | USB_EP_DIR_IN)  // TX
#define  UDI_CDC_DATA_EP_OUT           (2 | USB_EP_DIR_OUT) // RX
#define  UDI_CDC_COMM_EP               (2 | USB_EP_DIR_IN)  // Notify endpoint
//! Endpoints' numbers used by second CDC port (Optional)
#define  UDI_CDC_DATA_EP_IN_2          (3 | USB_EP_DIR_IN)  // TX
#define  UDI_CDC_DATA_EP_OUT_2         (4 | USB_EP_DIR_OUT) // RX
#define  UDI_CDC_COMM_EP_2             (4 | USB_EP_DIR_IN)  // Notify endpoint
//! Endpoints' numbers used by third CDC port (Optional)
#define  UDI_CDC_DATA_EP_IN_3          (5 | USB_EP_DIR_IN)  // TX
#define  UDI_CDC_DATA_EP_OUT_3         (6 | USB_EP_DIR_OUT) // RX
#define  UDI_CDC_COMM_EP_3             (6 | USB_EP_DIR_IN)  // Notify endpoint
//@}

#else

/**
 * \name Default endpoint configuration
 */
//@{
//! Endpoints' numbers used by single or first CDC port
#define  UDI_CDC_DATA_EP_IN            (1 | USB_EP_DIR_IN)  // TX
#define  UDI_CDC_DATA_EP_OUT           (2 | USB_EP_DIR_OUT) // RX
#define  UDI_CDC_COMM_EP               (3 | USB_EP_DIR_IN)  // Notify endpoint
//! Endpoints' numbers used by second CDC port (Optional)
#define  UDI_CDC_DATA_EP_IN_2          (4 | USB_EP_DIR_IN)  // TX
#define  UDI_CDC_DATA_EP_OUT_2         (5 | USB_EP_DIR_OUT) // RX
#define  UDI_CDC_COMM_EP_2             (6 | USB_EP_DIR_IN)  // Notify endpoint
//! Endpoints' numbers used by third CDC port (Optional)
#define  UDI_CDC_DATA_EP_IN_3          (7 | USB_EP_DIR_IN)  // TX
#define  UDI_CDC_DATA_EP_OUT_3         (8 | USB_EP_DIR_OUT) // RX
#define  UDI_CDC_COMM_EP_3             (9 | USB_EP_DIR_IN)  // Notify endpoint
//@}

#endif

//! Interface numbers used by single or first CDC port
#define  UDI_CDC_COMM_IFACE_NUMBER     0
#define  UDI_CDC_DATA_IFACE_NUMBER     1
//! Interface numbers used by second CDC port (Optional)
#define  UDI_CDC_COMM_IFACE_NUMBER_2   2
#define  UDI_CDC_DATA_IFACE_NUMBER_2   3
//! Interface numbers used by third CDC port (Optional)
#define  UDI_CDC_COMM_IFACE_NUMBER_3   4
#define  UDI_CDC_DATA_IFACE_NUMBER_3   5

/**
 * \name UDD Configuration
 */
//@{
//! 3 endpoints used by single CDC interface
#define  USB_DEVICE_MAX_EP             (3*UDI_CDC_PORT_NB)
//@}

//@}

#ifdef __cplusplus
}
#endif
#endif // _UDI_CDC_CONF_H_
