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


#ifndef PRINTF_H_
#define PRINTF_H_

/************************************************************************/
/*                            MACROS                                    */
/************************************************************************/

#define uart_putc(c)			USART_TXBuffer_PutByte(&USART_data, c)
#define uart_getc()				USART_RXBuffer_GetByte(&USART_data)
#define uart_available()		USART_RXBufferData_Available(&USART_data)

/************************************************************************/
/*                            Functions                                 */
/************************************************************************/


void uart_puts(uint8_t *str);
void uart_puth(unsigned n);
void uart_printf(char *format, ...);

#endif /* PRINTF_H_ */