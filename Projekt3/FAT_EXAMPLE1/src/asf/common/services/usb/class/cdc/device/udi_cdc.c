/**
 * \file
 *
 * \brief USB Device Communication Device Class (CDC) interface.
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
 */

#include "conf_usb.h"
#include "usb_protocol.h"
#include "usb_protocol_cdc.h"
#include "udd.h"
#include "udc.h"
#include "udi_cdc.h"
#include <string.h>

#ifdef UDI_CDC_LOW_RATE
#  ifdef USB_DEVICE_HS_SUPPORT
#    define UDI_CDC_TX_BUFFERS     (UDI_CDC_DATA_EPS_HS_SIZE)
#    define UDI_CDC_RX_BUFFERS     (UDI_CDC_DATA_EPS_HS_SIZE)
#  else
#    define UDI_CDC_TX_BUFFERS     (UDI_CDC_DATA_EPS_FS_SIZE)
#    define UDI_CDC_RX_BUFFERS     (UDI_CDC_DATA_EPS_FS_SIZE)
#  endif
#else
#  ifdef USB_DEVICE_HS_SUPPORT
#    define UDI_CDC_TX_BUFFERS     (UDI_CDC_DATA_EPS_HS_SIZE)
#    define UDI_CDC_RX_BUFFERS     (UDI_CDC_DATA_EPS_HS_SIZE)
#  else
#    define UDI_CDC_TX_BUFFERS     (5*UDI_CDC_DATA_EPS_FS_SIZE)
#    define UDI_CDC_RX_BUFFERS     (5*UDI_CDC_DATA_EPS_FS_SIZE)
#  endif
#endif

#if UDI_CDC_PORT_NB == 1
# define PORT 0
#else
# define PORT port
#endif

/**
 * \addtogroup udi_cdc_group
 *
 * @{
 */

/**
 * \name Interface for UDC
 */
//@{

bool udi_cdc_comm_enable(void);
void udi_cdc_comm_disable(void);
bool udi_cdc_comm_setup(void);
bool udi_cdc_data_enable(void);
void udi_cdc_data_disable(void);
bool udi_cdc_data_setup(void);
uint8_t udi_cdc_getsetting(void);
void udi_cdc_data_sof_notify(void);
UDC_DESC_STORAGE udi_api_t udi_api_cdc_comm = {
	.enable = udi_cdc_comm_enable,
	.disable = udi_cdc_comm_disable,
	.setup = udi_cdc_comm_setup,
	.getsetting = udi_cdc_getsetting,
};
UDC_DESC_STORAGE udi_api_t udi_api_cdc_data = {
	.enable = udi_cdc_data_enable,
	.disable = udi_cdc_data_disable,
	.setup = udi_cdc_data_setup,
	.getsetting = udi_cdc_getsetting,
	.sof_notify = udi_cdc_data_sof_notify,
};

#if UDI_CDC_PORT_NB > 1
bool udi_cdc_comm_enable_2(void);
void udi_cdc_comm_disable_2(void);
bool udi_cdc_comm_setup_2(void);
bool udi_cdc_data_enable_2(void);
void udi_cdc_data_sof_notify_2(void);
UDC_DESC_STORAGE udi_api_t udi_api_cdc_comm_2 = {
	.enable = udi_cdc_comm_enable_2,
	.disable = udi_cdc_comm_disable_2,
	.setup = udi_cdc_comm_setup_2,
	.getsetting = udi_cdc_getsetting,
};
UDC_DESC_STORAGE udi_api_t udi_api_cdc_data_2 = {
	.enable = udi_cdc_data_enable_2,
	.disable = udi_cdc_data_disable,
	.setup = udi_cdc_data_setup,
	.getsetting = udi_cdc_getsetting,
	.sof_notify = udi_cdc_data_sof_notify_2,
};
#endif

#if UDI_CDC_PORT_NB > 2
bool udi_cdc_comm_enable_3(void);
void udi_cdc_comm_disable_3(void);
bool udi_cdc_comm_setup_3(void);
bool udi_cdc_data_enable_3(void);
void udi_cdc_data_sof_notify_3(void);
UDC_DESC_STORAGE udi_api_t udi_api_cdc_comm_3 = {
	.enable = udi_cdc_comm_enable_3,
	.disable = udi_cdc_comm_disable_3,
	.setup = udi_cdc_comm_setup_3,
	.getsetting = udi_cdc_getsetting,
};
UDC_DESC_STORAGE udi_api_t udi_api_cdc_data_3 = {
	.enable = udi_cdc_data_enable_3,
	.disable = udi_cdc_data_disable,
	.setup = udi_cdc_data_setup,
	.getsetting = udi_cdc_getsetting,
	.sof_notify = udi_cdc_data_sof_notify_3,
};
#endif
//@}


/**
 * \name Internal routines
 */
//@{

/**
 * \name Routines to control serial line
 */
//@{

/**
 * \brief Sends line coding port 1 to application
 *
 * Called after SETUP request when line coding data is received.
 */
static void udi_cdc_line_coding_received(void);
#if UDI_CDC_PORT_NB > 1
/**
 * \brief Sends line coding port 2 to application
 *
 * Called after SETUP request when line coding data is received.
 */
static void udi_cdc_line_coding_received_2(void);
#endif
#if UDI_CDC_PORT_NB > 2
/**
 * \brief Sends line coding port 3 to application
 *
 * Called after SETUP request when line coding data is received.
 */
static void udi_cdc_line_coding_received_3(void);
#endif
/**
 * \brief Sends line coding port to application
 *
 * Called after SETUP request when line coding data is received.
 *
 * \param port       Communication port number to manage
 */
static void udi_cdc_line_coding_received_common(uint8_t port);

/**
 * \brief Records new state
 *
 * \param port       Communication port number to manage
 * \param b_set      State is enabled if true, else disabled
 * \param bit_mask   Field to process (see CDC_SERIAL_STATE_ defines)
 */
static void udi_cdc_ctrl_state_change(uint8_t port, bool b_set, le16_t bit_mask);

/**
 * \brief Check and eventually notify the USB host of new state
 *
 * \param port       Communication port number to manage
 */
static void udi_cdc_ctrl_state_notify(uint8_t port);

/**
 * \brief Ack sent of serial state message on port 1
 * Callback called after serial state message sent
 *
 * \param status     UDD_EP_TRANSFER_OK, if transfer finished
 * \param status     UDD_EP_TRANSFER_ABORT, if transfer aborted
 * \param n          number of data transfered
 */
static void udi_cdc_serial_state_msg_sent(udd_ep_status_t status, iram_size_t n);
#if UDI_CDC_PORT_NB > 1
/**
 * \brief Ack sent of serial state message on port 2
 * Callback called after serial state message sent
 *
 * \param status     UDD_EP_TRANSFER_OK, if transfer finished
 * \param status     UDD_EP_TRANSFER_ABORT, if transfer aborted
 * \param n          number of data transfered
 */
static void udi_cdc_serial_state_msg_sent_2(udd_ep_status_t status, iram_size_t n);
#endif
#if UDI_CDC_PORT_NB > 2
/**
 * \brief Ack sent of serial state message on port 3
 * Callback called after serial state message sent
 *
 * \param status     UDD_EP_TRANSFER_OK, if transfer finished
 * \param status     UDD_EP_TRANSFER_ABORT, if transfer aborted
 * \param n          number of data transfered
 */
static void udi_cdc_serial_state_msg_sent_3(udd_ep_status_t status, iram_size_t n);
#endif
/**
 * \brief Ack sent of serial state message on a port
 * Callback called after serial state message sent
 *
 * \param port       Communication port number to manage
 * \param status     UDD_EP_TRANSFER_OK, if transfer finished
 * \param status     UDD_EP_TRANSFER_ABORT, if transfer aborted
 * \param n          number of data transfered
 */
static void udi_cdc_serial_state_msg_sent_common(uint8_t port, udd_ep_status_t status, iram_size_t n);

//@}

/**
 * \name Routines to process data transfer
 */
//@{

/**
 * \brief Enable the reception of data from the USB host
 *
 * The value udi_cdc_rx_trans_sel indicate the RX buffer to fill.
 *
 * \param port       Communication port number to manage
 *
 * \return \c 1 if function was successfully done, otherwise \c 0.
 */
static bool udi_cdc_rx_start(uint8_t port);

/**
 * \brief Update rx buffer management with a new data for port 1
 * Callback called after data reception on USB line
 *
 * \param status     UDD_EP_TRANSFER_OK, if transfer finish
 * \param status     UDD_EP_TRANSFER_ABORT, if transfer aborted
 * \param n          number of data received
 */
static void udi_cdc_data_received(udd_ep_status_t status, iram_size_t n);
#if UDI_CDC_PORT_NB > 1
/**
 * \brief Update rx buffer management with a new data for port 2
 * Callback called after data reception on USB line
 *
 * \param status     UDD_EP_TRANSFER_OK, if transfer finish
 * \param status     UDD_EP_TRANSFER_ABORT, if transfer aborted
 * \param n          number of data received
 */
static void udi_cdc_data_received_2(udd_ep_status_t status, iram_size_t n);
#endif
#if UDI_CDC_PORT_NB > 2
/**
 * \brief Update rx buffer management with a new data for port 3
 * Callback called after data reception on USB line
 *
 * \param status     UDD_EP_TRANSFER_OK, if transfer finish
 * \param status     UDD_EP_TRANSFER_ABORT, if transfer aborted
 * \param n          number of data received
 */
static void udi_cdc_data_received_3(udd_ep_status_t status, iram_size_t n);
#endif

/**
 * \brief Update rx buffer management with a new data for a port
 * Callback called after data reception on USB line
 *
 * \param port       Communication port number to manage
 * \param status     UDD_EP_TRANSFER_OK, if transfer finish
 * \param status     UDD_EP_TRANSFER_ABORT, if transfer aborted
 * \param n          number of data received
 */
static void udi_cdc_data_received_common(uint8_t port, udd_ep_status_t status, iram_size_t n);

/**
 * \brief Ack sent of tx buffer on port 1
 * Callback called after data transfer on USB line
 *
 * \param status     UDD_EP_TRANSFER_OK, if transfer finished
 * \param status     UDD_EP_TRANSFER_ABORT, if transfer aborted
 * \param n          number of data transfered
 */
static void udi_cdc_data_sent(udd_ep_status_t status, iram_size_t n);
#if UDI_CDC_PORT_NB > 1
/**
 * \brief Ack sent of tx buffer on port 2
 * Callback called after data transfer on USB line
 *
 * \param status     UDD_EP_TRANSFER_OK, if transfer finished
 * \param status     UDD_EP_TRANSFER_ABORT, if transfer aborted
 * \param n          number of data transfered
 */
static void udi_cdc_data_sent_2(udd_ep_status_t status, iram_size_t n);
#endif
#if UDI_CDC_PORT_NB > 2
/**
 * \brief Ack sent of tx buffer on port 3
 * Callback called after data transfer on USB line
 *
 * \param status     UDD_EP_TRANSFER_OK, if transfer finished
 * \param status     UDD_EP_TRANSFER_ABORT, if transfer aborted
 * \param n          number of data transfered
 */
static void udi_cdc_data_sent_3(udd_ep_status_t status, iram_size_t n);
#endif

/**
 * \brief Ack sent of tx buffer on a port
 * Callback called after data transfer on USB line
 *
 * \param port       Communication port number to manage
 * \param status     UDD_EP_TRANSFER_OK, if transfer finished
 * \param status     UDD_EP_TRANSFER_ABORT, if transfer aborted
 * \param n          number of data transfered
 */
static void udi_cdc_data_sent_common(uint8_t port, udd_ep_status_t status, iram_size_t n);

/**
 * \brief Send buffer on line or wait a SOF event
 *
 * \param port       Communication port number to manage
 */
static void udi_cdc_tx_send(uint8_t port);

#if UDI_CDC_PORT_NB == 1
bool udi_cdc_multi_is_rx_ready(uint8_t port);
#endif

//@}

//@}

/**
 * \name Information about configuration of communication line
 */
//@{
static usb_cdc_line_coding_t udi_cdc_line_coding[UDI_CDC_PORT_NB];
static bool udi_cdc_serial_state_msg_ongoing[UDI_CDC_PORT_NB];
static volatile le16_t udi_cdc_state[UDI_CDC_PORT_NB];
COMPILER_WORD_ALIGNED static usb_cdc_notify_serial_state_t uid_cdc_state_msg[UDI_CDC_PORT_NB];
static const uint8_t UDI_CDC_COMM_EPS[]={
	UDI_CDC_COMM_EP,
#if UDI_CDC_PORT_NB > 1
	UDI_CDC_COMM_EP_2,
#endif
#if UDI_CDC_PORT_NB > 2
	UDI_CDC_COMM_EP_3,
#endif
};
static const udd_callback_trans_t udi_cdc_serial_state_msg_sents[] = {
	udi_cdc_serial_state_msg_sent,
#if UDI_CDC_PORT_NB > 1
	udi_cdc_serial_state_msg_sent_2,
#endif
#if UDI_CDC_PORT_NB > 2
	udi_cdc_serial_state_msg_sent_3,
#endif
};
typedef void (*udi_cdc_callback_setup_t) (void);
static udi_cdc_callback_setup_t udi_cdc_line_coding_receiveds[] = {
	udi_cdc_line_coding_received,
#if UDI_CDC_PORT_NB > 1
	udi_cdc_line_coding_received_2,
#endif
#if UDI_CDC_PORT_NB > 2
	udi_cdc_line_coding_receivedt_3,
#endif
};

//! Status of CDC interface
static volatile bool udi_cdc_running[UDI_CDC_PORT_NB];
//@}

/**
 * \name Variables to manage RX/TX transfer requests
 * Two buffers for each sense are used to optimize the speed.
 */
//@{

//! Buffer to receive data
COMPILER_WORD_ALIGNED static uint8_t udi_cdc_rx_buf[UDI_CDC_PORT_NB][2][UDI_CDC_RX_BUFFERS];
//! Data available in RX buffers
static uint16_t udi_cdc_rx_buf_nb[UDI_CDC_PORT_NB][2];
//! Give the current RX buffer used (rx0 if 0, rx1 if 1)
static volatile uint8_t udi_cdc_rx_buf_sel[UDI_CDC_PORT_NB];
//! Read position in current RX buffer
static volatile uint16_t udi_cdc_rx_pos[UDI_CDC_PORT_NB];
//! Signal a transfer on-going
static volatile bool udi_cdc_rx_trans_ongoing[UDI_CDC_PORT_NB];
static const uint8_t UDI_CDC_DATA_EP_OUTS[]={
	UDI_CDC_DATA_EP_OUT,
#if UDI_CDC_PORT_NB > 1
	UDI_CDC_DATA_EP_OUT_2,
#endif
#if UDI_CDC_PORT_NB > 2
	UDI_CDC_DATA_EP_OUT_3,
#endif
};
static const udd_callback_trans_t udi_cdc_data_received_callbacks[] = {
	udi_cdc_data_received,
#if UDI_CDC_PORT_NB > 1
	udi_cdc_data_received_2,
#endif
#if UDI_CDC_PORT_NB > 2
	udi_cdc_data_received_3,
#endif
};

//! Define a transfer halted
#define  UDI_CDC_TRANS_HALTED    2

//! Buffer to send data
COMPILER_WORD_ALIGNED static uint8_t udi_cdc_tx_buf[UDI_CDC_PORT_NB][2][UDI_CDC_TX_BUFFERS];
//! Data available in TX buffers
static uint16_t udi_cdc_tx_buf_nb[UDI_CDC_PORT_NB][2];
//! Give current TX buffer used (tx0 if 0, tx1 if 1)
static volatile uint8_t udi_cdc_tx_buf_sel[UDI_CDC_PORT_NB];
//! Value of SOF during last TX transfer
static uint16_t udi_cdc_tx_sof_num[UDI_CDC_PORT_NB];
//! Signal a transfer on-going
static volatile bool udi_cdc_tx_trans_ongoing[UDI_CDC_PORT_NB];
//! Signal that both buffer content data to send
static volatile bool udi_cdc_tx_both_buf_to_send[UDI_CDC_PORT_NB];
static const uint8_t UDI_CDC_DATA_EP_INS[]={
	UDI_CDC_DATA_EP_IN,
#if UDI_CDC_PORT_NB > 1
	UDI_CDC_DATA_EP_IN_2,
#endif
#if UDI_CDC_PORT_NB > 2
	UDI_CDC_DATA_EP_IN_3,
#endif
};
static const udd_callback_trans_t udi_cdc_data_sents[] = {
	udi_cdc_data_sent,
#if UDI_CDC_PORT_NB > 1
	udi_cdc_data_sent_2,
#endif
#if UDI_CDC_PORT_NB > 2
	udi_cdc_data_sent_3,
#endif
};

//@}



static bool udi_cdc_comm_enable_common(uint8_t port)
{
	// Initialize control signal management
	udi_cdc_state[PORT] = CPU_TO_LE16(0);

	uid_cdc_state_msg[PORT].header.bmRequestType =
			USB_REQ_DIR_IN | USB_REQ_TYPE_CLASS |
			USB_REQ_RECIP_INTERFACE,
	uid_cdc_state_msg[PORT].header.bNotification = USB_REQ_CDC_NOTIFY_SERIAL_STATE,
	uid_cdc_state_msg[PORT].header.wValue = LE16(0),
	uid_cdc_state_msg[PORT].header.wIndex = LE16(UDI_CDC_COMM_IFACE_NUMBER),
	uid_cdc_state_msg[PORT].header.wLength = LE16(2),
	uid_cdc_state_msg[PORT].value = CPU_TO_LE16(0);

	udi_cdc_line_coding[PORT].dwDTERate = CPU_TO_LE32(UDI_CDC_DEFAULT_RATE);
	udi_cdc_line_coding[PORT].bCharFormat = UDI_CDC_DEFAULT_STOPBITS;
	udi_cdc_line_coding[PORT].bParityType = UDI_CDC_DEFAULT_PARITY;
	udi_cdc_line_coding[PORT].bDataBits = UDI_CDC_DEFAULT_DATABITS;
	// Call application callback
	// to initialize memories or indicate that interface is enabled
#if UDI_CDC_PORT_NB == 1
	UDI_CDC_SET_CODING_EXT((&udi_cdc_line_coding[0]));
	return UDI_CDC_ENABLE_EXT();
#else
	UDI_CDC_SET_CODING_EXT(port,(&udi_cdc_line_coding[port]));
	return UDI_CDC_ENABLE_EXT(port);
#endif
}

bool udi_cdc_comm_enable(void)
{
	return udi_cdc_comm_enable_common(0);
}
#if UDI_CDC_PORT_NB > 1
bool udi_cdc_comm_enable_2(void)
{
	return udi_cdc_comm_enable_common(1);
}
#endif
#if UDI_CDC_PORT_NB > 2
bool udi_cdc_comm_enable_3(void)
{
	return udi_cdc_comm_enable_common(2);

}
#endif

static bool udi_cdc_data_enable_common(uint8_t port)
{
	// Initialize TX management
	udi_cdc_tx_trans_ongoing[PORT] = false;
	udi_cdc_tx_both_buf_to_send[PORT] = false;
	udi_cdc_tx_buf_sel[PORT] = 0;
	udi_cdc_tx_buf_nb[PORT][0] = 0;
	udi_cdc_tx_buf_nb[PORT][1] = 0;
	udi_cdc_tx_sof_num[PORT] = 0;
	udi_cdc_tx_send(PORT);

	// Initialize RX management
	udi_cdc_rx_trans_ongoing[PORT] = false;
	udi_cdc_rx_buf_sel[PORT] = 0;
	udi_cdc_rx_buf_nb[PORT][0] = 0;
	udi_cdc_rx_pos[PORT] = 0;
	udi_cdc_running[PORT] = udi_cdc_rx_start(PORT);
	return udi_cdc_running[PORT];
}

bool udi_cdc_data_enable(void)
{
	return udi_cdc_data_enable_common(0);
}
#if UDI_CDC_PORT_NB > 1
bool udi_cdc_data_enable_2(void)
{
	return udi_cdc_data_enable_common(1);
}
#endif
#if UDI_CDC_PORT_NB > 2
bool udi_cdc_data_enable_3(void)
{
	return udi_cdc_data_enable_common(2);
}
#endif


static void udi_cdc_comm_disable_common(uint8_t port)
{
#if UDI_CDC_PORT_NB == 1
	udi_cdc_running[0] = false;
	UDI_CDC_DISABLE_EXT();
#else
	udi_cdc_running[port] = false;
	UDI_CDC_DISABLE_EXT(port);
#endif
}

void udi_cdc_comm_disable(void)
{
	udi_cdc_comm_disable_common(0);
}
#if UDI_CDC_PORT_NB > 1
void udi_cdc_comm_disable_2(void)
{
	udi_cdc_comm_disable_common(1);
}
#endif
#if UDI_CDC_PORT_NB > 2
void udi_cdc_comm_disable_3(void)
{
	udi_cdc_comm_disable_common(2);
}
#endif

void udi_cdc_data_disable(void)
{
}



static bool udi_cdc_comm_setup_common(uint8_t port)
{
	if (Udd_setup_is_in()) {
		// GET Interface Requests
		if (Udd_setup_type() == USB_REQ_TYPE_CLASS) {
			// Requests Class Interface Get
			switch (udd_g_ctrlreq.req.bRequest) {
			case USB_REQ_CDC_GET_LINE_CODING:
				// Get configuration of CDC line
				if (sizeof(usb_cdc_line_coding_t) !=
						udd_g_ctrlreq.req.wLength)
					return false; // Error for USB host
				udd_g_ctrlreq.payload =
						(uint8_t *) &
						udi_cdc_line_coding[PORT];
				udd_g_ctrlreq.payload_size =
						sizeof(usb_cdc_line_coding_t);
				return true;
			}
		}
	}
	if (Udd_setup_is_out()) {
		// SET Interface Requests
		if (Udd_setup_type() == USB_REQ_TYPE_CLASS) {
			// Requests Class Interface Set
			switch (udd_g_ctrlreq.req.bRequest) {
			case USB_REQ_CDC_SET_LINE_CODING:
				// Change configuration of CDC line
				if (sizeof(usb_cdc_line_coding_t) !=
						udd_g_ctrlreq.req.wLength)
					return false; // Error for USB host
				udd_g_ctrlreq.callback =
						udi_cdc_line_coding_receiveds[PORT];
				udd_g_ctrlreq.payload =
						(uint8_t *) &
						udi_cdc_line_coding[PORT];
				udd_g_ctrlreq.payload_size =
						sizeof(usb_cdc_line_coding_t);
				return true;
			case USB_REQ_CDC_SET_CONTROL_LINE_STATE:
				// According cdc spec 1.1 chapter 6.2.14
#if UDI_CDC_PORT_NB == 1
				UDI_CDC_SET_DTR_EXT((0 !=
						(udd_g_ctrlreq.req.wValue
						 & CDC_CTRL_SIGNAL_DTE_PRESENT)));
				UDI_CDC_SET_RTS_EXT((0 !=
						(udd_g_ctrlreq.req.wValue
						 & CDC_CTRL_SIGNAL_ACTIVATE_CARRIER)));
#else
				UDI_CDC_SET_DTR_EXT(PORT, (0 !=
						(udd_g_ctrlreq.req.wValue
						 & CDC_CTRL_SIGNAL_DTE_PRESENT)));
				UDI_CDC_SET_RTS_EXT(PORT, (0 !=
						(udd_g_ctrlreq.req.wValue
						 & CDC_CTRL_SIGNAL_ACTIVATE_CARRIER)));
#endif
				return true;
			}
		}
	}
	return false;  // request Not supported
}

bool udi_cdc_comm_setup(void)
{
	return udi_cdc_comm_setup_common(0);
}
#if UDI_CDC_PORT_NB > 1
bool udi_cdc_comm_setup_2(void)
{
	return udi_cdc_comm_setup_common(1);
}
#endif
#if UDI_CDC_PORT_NB > 2
bool udi_cdc_comm_setup_3(void)
{
	return udi_cdc_comm_setup_common(2);
}
#endif

bool udi_cdc_data_setup(void)
{
	return false;  // request Not supported
}

uint8_t udi_cdc_getsetting(void)
{
	return 0;      // CDC don't have multiple alternate setting
}

void udi_cdc_data_sof_notify(void)
{
	udi_cdc_tx_send(0);
}
#if UDI_CDC_PORT_NB > 1
void udi_cdc_data_sof_notify_2(void)
{
	udi_cdc_tx_send(1);
}
#endif
#if UDI_CDC_PORT_NB > 2
void udi_cdc_data_sof_notify_3(void)
{
	udi_cdc_tx_send(2);
}
#endif

//-------------------------------------------------
//------- Internal routines to control serial line


static void udi_cdc_line_coding_received(void)
{
	udi_cdc_line_coding_received_common(0);
}
#if UDI_CDC_PORT_NB > 1
static void udi_cdc_line_coding_received_2(void)
{
	udi_cdc_line_coding_received_common(1);
}
#endif
#if UDI_CDC_PORT_NB > 2
static void udi_cdc_line_coding_received_3(void)
{
	udi_cdc_line_coding_received_common(2);
}
#endif
static void udi_cdc_line_coding_received_common(uint8_t port)
{
	// Send line coding to component associated to CDC
#if UDI_CDC_PORT_NB == 1
	UDI_CDC_SET_CODING_EXT((&udi_cdc_line_coding[0]));
#else
	UDI_CDC_SET_CODING_EXT(port, (&udi_cdc_line_coding[port]));
#endif
}


static void udi_cdc_ctrl_state_change(uint8_t port, bool b_set, le16_t bit_mask)
{
	irqflags_t flags;

	// Update state
	flags = cpu_irq_save(); // Protect udi_cdc_state
	if (b_set) {
		udi_cdc_state[PORT] |= bit_mask;
	} else {
		udi_cdc_state[PORT] &= ~bit_mask;
	}
	cpu_irq_restore(flags);

	// Send it if possible and state changed
	udi_cdc_ctrl_state_notify(PORT);
}


static void udi_cdc_ctrl_state_notify(uint8_t port)
{
	// Send it if possible and state changed
	if ((!udi_cdc_serial_state_msg_ongoing[PORT])
			&& (udi_cdc_state[PORT] != uid_cdc_state_msg[PORT].value)) {
		// Fill notification message
		uid_cdc_state_msg[PORT].value = udi_cdc_state[PORT];
		// Send notification message
		udi_cdc_serial_state_msg_ongoing[PORT] =
				udd_ep_run(UDI_CDC_COMM_EPS[PORT] ,
				false,
				(uint8_t *) & uid_cdc_state_msg[PORT],
				sizeof(uid_cdc_state_msg),
				udi_cdc_serial_state_msg_sents[PORT]);
	}
}


static void udi_cdc_serial_state_msg_sent(udd_ep_status_t status, iram_size_t n)
{
	udi_cdc_serial_state_msg_sent_common(0, status, n);
}
#if UDI_CDC_PORT_NB > 1
static void udi_cdc_serial_state_msg_sent_2(udd_ep_status_t status, iram_size_t n)
{
	udi_cdc_serial_state_msg_sent_common(1, status, n);
}
#endif
#if UDI_CDC_PORT_NB > 2
static void udi_cdc_serial_state_msg_sent_3(udd_ep_status_t status, iram_size_t n)
{
	udi_cdc_serial_state_msg_sent_common(2, status, n);
}
#endif

static void udi_cdc_serial_state_msg_sent_common(uint8_t port, udd_ep_status_t status, iram_size_t n)
{
	udi_cdc_serial_state_msg_ongoing[PORT] = false;

	// For the irregular signals like break, the incoming ring signal,
	// or the overrun error state, this will reset their values to zero
	// and again will not send another notification until their state changes.
	udi_cdc_state[PORT] &= ~(CDC_SERIAL_STATE_BREAK |
			CDC_SERIAL_STATE_RING |
			CDC_SERIAL_STATE_FRAMING |
			CDC_SERIAL_STATE_PARITY | CDC_SERIAL_STATE_OVERRUN);
	uid_cdc_state_msg[PORT].value &= ~(CDC_SERIAL_STATE_BREAK |
			CDC_SERIAL_STATE_RING |
			CDC_SERIAL_STATE_FRAMING |
			CDC_SERIAL_STATE_PARITY | CDC_SERIAL_STATE_OVERRUN);
	// Send it if possible and state changed
	udi_cdc_ctrl_state_notify(PORT);
}


//-------------------------------------------------
//------- Internal routines to process data transfer


static bool udi_cdc_rx_start(uint8_t port)
{
	irqflags_t flags;
	uint8_t buf_sel_trans;

	flags = cpu_irq_save();
	buf_sel_trans = udi_cdc_rx_buf_sel[PORT];
	if (udi_cdc_rx_trans_ongoing[PORT] ||
		(udi_cdc_rx_pos[PORT] < udi_cdc_rx_buf_nb[PORT][buf_sel_trans])) {
		// Transfer already on-going or current buffer no empty
		cpu_irq_restore(flags);
		return false;
	}

	// Change current buffer
	udi_cdc_rx_pos[PORT] = 0;
	udi_cdc_rx_buf_sel[PORT] = (buf_sel_trans==0)?1:0;

	// Start transfer on RX
	udi_cdc_rx_trans_ongoing[PORT] = true;
	cpu_irq_restore(flags);

	if (udi_cdc_multi_is_rx_ready(PORT)) {
#if UDI_CDC_PORT_NB == 1
		UDI_CDC_RX_NOTIFY();
#else
		UDI_CDC_RX_NOTIFY(port);
#endif
	}

	return udd_ep_run( UDI_CDC_DATA_EP_OUTS[PORT],
			true,
			udi_cdc_rx_buf[PORT][buf_sel_trans],
			UDI_CDC_RX_BUFFERS,
			udi_cdc_data_received_callbacks[PORT]);
}


static void udi_cdc_data_received(udd_ep_status_t status, iram_size_t n)
{
	udi_cdc_data_received_common(0, status, n);
}
#if UDI_CDC_PORT_NB > 1
static void udi_cdc_data_received_2(udd_ep_status_t status, iram_size_t n)
{
	udi_cdc_data_received_common(1, status, n);
}
#endif
#if UDI_CDC_PORT_NB > 2
static void udi_cdc_data_received_3(udd_ep_status_t status, iram_size_t n)
{
	udi_cdc_data_received_common(2, status, n);
}
#endif

static void udi_cdc_data_received_common(uint8_t port, udd_ep_status_t status, iram_size_t n)
{
	uint8_t buf_sel_trans;

	if (UDD_EP_TRANSFER_OK != status) {
		// Abort reception
		return;
	}
	buf_sel_trans = (udi_cdc_rx_buf_sel[PORT]==0)?1:0;
	if (!n) {
		udd_ep_run( UDI_CDC_DATA_EP_OUTS[PORT],
				true,
				udi_cdc_rx_buf[PORT][buf_sel_trans],
				UDI_CDC_RX_BUFFERS,
				udi_cdc_data_received_callbacks[PORT]);
		return;
	}
	udi_cdc_rx_buf_nb[PORT][buf_sel_trans] = n;
	udi_cdc_rx_trans_ongoing[PORT] = false;
	udi_cdc_rx_start(PORT);
}


static void udi_cdc_data_sent(udd_ep_status_t status, iram_size_t n)
{
	udi_cdc_data_sent_common(0, status, n);
}
#if UDI_CDC_PORT_NB > 1
static void udi_cdc_data_sent_2(udd_ep_status_t status, iram_size_t n)
{
	udi_cdc_data_sent_common(1, status, n);
}
#endif
#if UDI_CDC_PORT_NB > 2
static void udi_cdc_data_sent_3(udd_ep_status_t status, iram_size_t n)
{
	udi_cdc_data_sent_common(2, status, n);
}
#endif

static void udi_cdc_data_sent_common(uint8_t port, udd_ep_status_t status, iram_size_t n)
{
	if (UDD_EP_TRANSFER_OK != status) {
		// Abort transfer
		return;
	}
	udi_cdc_tx_buf_nb[PORT][(udi_cdc_tx_buf_sel[PORT]==0)?1:0] = 0;
	udi_cdc_tx_both_buf_to_send[PORT] = false;
	udi_cdc_tx_trans_ongoing[PORT] = false;
	udi_cdc_tx_send(PORT);
}


static void udi_cdc_tx_send(uint8_t port)
{
	irqflags_t flags;
	uint8_t buf_sel_trans;
	bool b_short_packet;

	if (udi_cdc_tx_trans_ongoing[PORT]) {
		return; // Already on going or wait next SOF to send next data
	}
	if (udd_is_high_speed()) {
		if (udi_cdc_tx_sof_num[PORT] == udd_get_micro_frame_number()) {
			return; // Wait next SOF to send next data
		}
	}else{
		if (udi_cdc_tx_sof_num[PORT] == udd_get_frame_number()) {
			return; // Wait next SOF to send next data
		}
	}

	flags = cpu_irq_save(); // to protect udi_cdc_tx_buf_sel
	buf_sel_trans = udi_cdc_tx_buf_sel[PORT];
	if (!udi_cdc_tx_both_buf_to_send[PORT]) {
		// Send current Buffer
		// and switch the current buffer
		udi_cdc_tx_buf_sel[PORT] = (buf_sel_trans==0)?1:0;
	}else{
		// Send the other Buffer
		// and no switch the current buffer
		buf_sel_trans = (buf_sel_trans==0)?1:0;
	}
	udi_cdc_tx_trans_ongoing[PORT] = true;
	cpu_irq_restore(flags);
	
	b_short_packet = (udi_cdc_tx_buf_nb[PORT][buf_sel_trans] != UDI_CDC_TX_BUFFERS);
	if (b_short_packet) {
		if (udd_is_high_speed()) {
			udi_cdc_tx_sof_num[PORT] = udd_get_micro_frame_number();
		}else{
			udi_cdc_tx_sof_num[PORT] = udd_get_frame_number();
		}
	}else{
		udi_cdc_tx_sof_num[PORT] = 0; // Force next transfer without wait SOF
	}

	// Send the buffer with enable of short packet
	udd_ep_run( UDI_CDC_DATA_EP_INS[PORT],
			b_short_packet,
			udi_cdc_tx_buf[PORT][buf_sel_trans],
			udi_cdc_tx_buf_nb[PORT][buf_sel_trans],
			udi_cdc_data_sents[PORT]);
}


//---------------------------------------------
//------- Application interface


//------- Application interface

void udi_cdc_ctrl_signal_dcd(bool b_set)
{
	udi_cdc_ctrl_state_change(0, b_set, CDC_SERIAL_STATE_DCD);
}

void udi_cdc_ctrl_signal_dsr(bool b_set)
{
	udi_cdc_ctrl_state_change(0, b_set, CDC_SERIAL_STATE_DSR);
}

void udi_cdc_signal_framing_error(void)
{
	udi_cdc_ctrl_state_change(0, true, CDC_SERIAL_STATE_FRAMING);
}

void udi_cdc_signal_parity_error(void)
{
	udi_cdc_ctrl_state_change(0, true, CDC_SERIAL_STATE_PARITY);
}

void udi_cdc_signal_overrun(void)
{
	udi_cdc_ctrl_state_change(0, true, CDC_SERIAL_STATE_OVERRUN);
}

#if UDI_CDC_PORT_NB > 1
void udi_cdc_multi_ctrl_signal_dcd(uint8_t port, bool b_set)
{
	udi_cdc_ctrl_state_change(port, b_set, CDC_SERIAL_STATE_DCD);
}

void udi_cdc_multi_ctrl_signal_dsr(uint8_t port, bool b_set)
{
	udi_cdc_ctrl_state_change(port, b_set, CDC_SERIAL_STATE_DSR);
}

void udi_cdc_multi_signal_framing_error(uint8_t port)
{
	udi_cdc_ctrl_state_change(port, true, CDC_SERIAL_STATE_FRAMING);
}

void udi_cdc_multi_signal_parity_error(uint8_t port)
{
	udi_cdc_ctrl_state_change(port, true, CDC_SERIAL_STATE_PARITY);
}

void udi_cdc_multi_signal_overrun(uint8_t port)
{
	udi_cdc_ctrl_state_change(port, true, CDC_SERIAL_STATE_OVERRUN);
}
#endif

bool udi_cdc_multi_is_rx_ready(uint8_t port)
{
	uint16_t pos = udi_cdc_rx_pos[PORT];
	return (pos < udi_cdc_rx_buf_nb[PORT][udi_cdc_rx_buf_sel[PORT]]);
}

bool udi_cdc_is_rx_ready(void)
{
	return udi_cdc_multi_is_rx_ready(0);
}

int udi_cdc_multi_getc(uint8_t port)
{
	int rx_data = 0;
	bool b_databit_9;
	uint16_t pos;
	uint8_t buf_sel;

	b_databit_9 = (9 == udi_cdc_line_coding[PORT].bDataBits);

udi_cdc_getc_process_one_byte:
	// Check avaliable data
	pos = udi_cdc_rx_pos[PORT];
	buf_sel = udi_cdc_rx_buf_sel[PORT];
	while (pos >= udi_cdc_rx_buf_nb[PORT][buf_sel]) {
		if (!udi_cdc_running[PORT]) {
			return 0;
		}
		goto udi_cdc_getc_process_one_byte;
	}

	// Read data
	rx_data |= udi_cdc_rx_buf[PORT][buf_sel][pos];
	udi_cdc_rx_pos[PORT] = pos+1;

	udi_cdc_rx_start(PORT);

	if (b_databit_9) {
		// Receive MSB
		b_databit_9 = false;
		rx_data = rx_data << 8;
		goto udi_cdc_getc_process_one_byte;
	}
	return rx_data;
}

int udi_cdc_getc(void)
{
	return udi_cdc_multi_getc(0);
}

iram_size_t udi_cdc_multi_read_buf(uint8_t port, int* buf, iram_size_t size)
{
	uint8_t *ptr_buf = (uint8_t *)buf;
	iram_size_t copy_nb;
	uint16_t pos;
	uint8_t buf_sel;

udi_cdc_read_buf_loop_wait:
	// Check avaliable data
	pos = udi_cdc_rx_pos[PORT];
	buf_sel = udi_cdc_rx_buf_sel[PORT];
	while (pos >= udi_cdc_rx_buf_nb[PORT][buf_sel]) {
		if (!udi_cdc_running[PORT]) {
			return size;
		}
		goto udi_cdc_read_buf_loop_wait;
	}

	// Read data
	copy_nb = udi_cdc_rx_buf_nb[PORT][buf_sel] - pos;
	if (copy_nb>size) {
		copy_nb = size;
	}
	memcpy(ptr_buf, &udi_cdc_rx_buf[PORT][buf_sel][pos], copy_nb);
	udi_cdc_rx_pos[PORT] += copy_nb;
	ptr_buf += copy_nb;
	size -= copy_nb;
	udi_cdc_rx_start(PORT);

	if (size) {
		goto udi_cdc_read_buf_loop_wait;
	}
	return 0;
}

iram_size_t udi_cdc_read_buf(int* buf, iram_size_t size)
{
	return udi_cdc_multi_read_buf(0, buf, size);
}

bool udi_cdc_multi_is_tx_ready(uint8_t port)
{
	irqflags_t flags;
	if (udi_cdc_tx_buf_nb[PORT][udi_cdc_tx_buf_sel[PORT]]!=UDI_CDC_TX_BUFFERS) {
		return true;
	}
	if (!udi_cdc_tx_both_buf_to_send[PORT]) {
		flags = cpu_irq_save(); // to protect udi_cdc_tx_buf_sel
		if (!udi_cdc_tx_trans_ongoing[PORT]) {
			// No transfer on-going
			// then use the other buffer to store data
			udi_cdc_tx_both_buf_to_send[PORT] = true;
			udi_cdc_tx_buf_sel[PORT] = (udi_cdc_tx_buf_sel[PORT]==0)?1:0;
		}
	  	cpu_irq_restore(flags);
	}
	return (udi_cdc_tx_buf_nb[PORT][udi_cdc_tx_buf_sel[PORT]]!=UDI_CDC_TX_BUFFERS);
}

bool udi_cdc_is_tx_ready(void)
{
	return udi_cdc_multi_is_tx_ready(0);
}

int udi_cdc_multi_putc(uint8_t port, int value)
{
	irqflags_t flags;
	bool b_databit_9;
	uint8_t buf_sel;

	b_databit_9 = (9 == udi_cdc_line_coding[PORT].bDataBits);

udi_cdc_putc_process_one_byte:
	// Check avaliable space
	if (!udi_cdc_multi_is_tx_ready(PORT)) {
		if (!udi_cdc_running[PORT]) {
			return false;
		}
		goto udi_cdc_putc_process_one_byte;
	}

	// Write value
	flags = cpu_irq_save();
	buf_sel = udi_cdc_tx_buf_sel[PORT];
	udi_cdc_tx_buf[PORT][buf_sel][udi_cdc_tx_buf_nb[PORT][buf_sel]++] = value;
	cpu_irq_restore(flags);

	if (b_databit_9) {
		// Send MSB
		b_databit_9 = false;
		value = value >> 8;
		goto udi_cdc_putc_process_one_byte;
	}
	return true;
}

int udi_cdc_putc(int value)
{
	return udi_cdc_multi_putc(0, value);
}

iram_size_t udi_cdc_multi_write_buf(uint8_t port, const int* buf, iram_size_t size)
{
	irqflags_t flags;
	uint8_t buf_sel;
	uint16_t buf_nb;
	iram_size_t copy_nb;
	uint8_t *ptr_buf = (uint8_t *)buf;

	if (9 == udi_cdc_line_coding[PORT].bDataBits) {
		size *=2;
	}

udi_cdc_write_buf_loop_wait:
	// Check avaliable space
	if (!udi_cdc_multi_is_tx_ready(PORT)) {
		if (!udi_cdc_running[PORT]) {
			return size;
		}
		goto udi_cdc_write_buf_loop_wait;
	}

	// Write values
	flags = cpu_irq_save();
	buf_sel = udi_cdc_tx_buf_sel[PORT];
	buf_nb = udi_cdc_tx_buf_nb[PORT][buf_sel];
	copy_nb = UDI_CDC_TX_BUFFERS - buf_nb;
	if (copy_nb>size) {
		copy_nb = size;
	}
	memcpy(&udi_cdc_tx_buf[PORT][buf_sel][buf_nb], ptr_buf, copy_nb);
	udi_cdc_tx_buf_nb[PORT][buf_sel] = buf_nb + copy_nb;
	cpu_irq_restore(flags);

	// Update buffer pointer
	ptr_buf = ptr_buf + copy_nb;
	size -= copy_nb;

	if (size) {
		goto udi_cdc_write_buf_loop_wait;
	}

	return 0;
}

iram_size_t udi_cdc_write_buf(const int* buf, iram_size_t size)
{
	return udi_cdc_multi_write_buf(0, buf, size);
}

//@}
