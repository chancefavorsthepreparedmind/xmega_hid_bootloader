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

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include "xmega_bootloader.h"

volatile bool run_boot;

void bootloader_init()
{
	static uint8_t checksum;
	
	/* for some reason jumping to the bootloader from the application doesn't work, we just get an
	 * unrecognized device. Atmel's DFU bootloader doesn't use a function pointer but issues
	 * a software reset and checks if the software reset flag has been set
	 */
	if(RST.STATUS & CHIP_RESET_CAUSE_SOFT)
	{
		run_boot = true;
		RST.STATUS |= 1<<RST_SRF_bp;
	}
	else
	{
	// check first 16 bytes, if zero flash must be empty or corrupted
	// checksum = sum of i_1-i_7
	for(flash_addr_t i = 0; i<8; i++)
		if(nvm_flash_read_word((flash_addr_t)i) == 0xffff) checksum += i;
	
	// if there is no firmware version data then flash must be empty, enter bootloader
	if((*(const __flash flash_addr_t*)FIRMWARE_ADDR == 0xffff) || checksum == 28) run_boot = true;
	}
}

void init_usb()
{
	// enable USB clock
	
	uint16_t cal;
	
	MSB(cal) = nvm_read_production_signature_row(
			nvm_get_production_signature_row_offset(USBRCOSC));
	LSB(cal) = nvm_read_production_signature_row(
			nvm_get_production_signature_row_offset(USBRCOSCA));
	/*
	* If a device has an uncalibrated value in the
	* production signature row (early sample part), load a
	* sane default calibration value.
	*/
	if (cal == 0xFFFF) {
		cal = 0x2340;
	}
	osc_user_calibration(OSC_ID_RC32MHZ,cal);
	
	osc_enable(OSC_ID_RC32MHZ);
	osc_wait_ready(OSC_ID_RC32MHZ);
	
	ccp_write_io((uint8_t *)&CLK.USBCTRL, (CLK_USBPSDIV_8_gc)
	| CLK_USBSRC_RC32M_gc
	| CLK_USBSEN_bm);	
	
	// enable module
	*((uint8_t *)&PR.PRGEN + SYSCLK_PORT_GEN) &= ~SYSCLK_USB;
	udd_set_low_speed();
	
	/***CRUCIAL SEGMENT***/
	// we need to enable DFLL calibration or the USB clock will not be precise enough, i.e.
	// we will not be able to receive buffers containing long series of ones (= 0xff)!
	DFLLRC32M.COMP1 = 0x80;
	DFLLRC32M.COMP2 = 0xBB;
	OSC.DFLLCTRL |= OSC_RC32MCREF_USBSOF_gc;
	DFLLRC32M.CTRL = DFLL_ENABLE_bm;
	
	// initialize vectors, enable round-robin scheduling for low priority interrupts
	PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
	sei();

#ifdef DEBUG_MODE	
	uart_init();
	uart_printf("Xmega Bootloader running...\n\r");
#endif
	
	udc_start();
}

void main_loop(void)
{
	bootloader_init();
	
	if(run_boot == true)
	{
#ifndef DEBUG_MODE
		/* Set interrupt vector location to boot section of flash */
		uint8_t temp = PMIC.CTRL | PMIC_IVSEL_bm;
		CCP = CCP_IOREG_gc;
		PMIC.CTRL = temp;
#endif
		
		init_usb();
		
		while(run_boot == true);
		//leave_bootloader();
		wdt_enable(WDTO_1S_XMEGA);
		while(1);
	}
	
	/*Set interrupt vector location to application section of flash */
	uint8_t temp = PMIC.CTRL & ~PMIC_IVSEL_bm;
	CCP = CCP_IOREG_gc;
	PMIC.CTRL = temp;
	
	null_vector();
	while(1);
	
}

int main(void)
{
	main_loop();
	return 0;
}
