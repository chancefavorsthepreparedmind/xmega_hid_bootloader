/*
Copyright (C) 2019  Jörn Giest
This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, version 3.
This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program. If not, see <https://www.gnu.org/licenses/>.
*/


#ifndef XMEGA_BOOTLOADER_H_
#define XMEGA_BOOTLOADER_H_

//#define DEBUG_MODE

#include <asf.h>
#include <avr/io.h>
#include <string.h>
#include <stdarg.h>
#include <usb_device.h>
#include <avr/wdt.h>

#ifdef DEBUG_MODE
#include "usart_driver.h"
#include "printf.h"


#define USART					USARTD0
#define UART_PORT				PORTD_OUT
#define UART_PORT_DIR			PORTD_DIR
#define UART_RX					2
#define UART_TX					3
#endif

#define FIRMWARE_ADDR			0x73FE

#define HID_REPORT_ID_PAGE_DATA					0x01
#define HID_REPORT_ID_EXIT_BOOTLOADER		0x02

#define WDTO_8MS_XMEGA			0<<2
#define WDTO_16MS_XMEGA			1<<2
#define WDTO_1S_XMEGA			7<<2
#define WDTO_2S_XMEGA			8<<2
#define WDTO_8S_XMEGA			10<<2

void main_loop(void) __attribute__((OS_main));
//void (*udi_hid_setup_process_received_report_cb)(void);
void update_firmware(void);
//void leave_bootloader(void);
void bootloader_init(void);
void init_usb(void);
void (*null_vector)(void);


#endif /* XMEGA_BOOTLOADER_H_ */