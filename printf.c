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

#include "xmega_bootloader.h"
#ifdef DEBUG_MODE



void uart_puts(uint8_t *str)
{
	for(uint8_t i = 0; i < strlen((const char*)str); i++)
		uart_putc(str[i]);
}

static const unsigned long dv[] = {
  //  4294967296      // 32 bit unsigned max
  1000000000,// +0
  100000000, // +1
  10000000, // +2
  1000000, // +3
  100000, // +4
  //       65535      // 16 bit unsigned max
  10000, // +5
  1000, // +6
  100, // +7
  10, // +8
  1, // +9
};

static void xtoa(unsigned long x, const unsigned long *dp) {
  char c;
  unsigned long d;
  if (x) {
    while (x < *dp)
      ++dp;
    do {
      d = *dp++;
      c = '0';
      while (x >= d)
        ++c, x -= d;
      uart_putc(c);
    } while (!(d & 1));
  } else
    uart_putc('0');
}

void uart_puth(unsigned n) {
  static const char hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8',
  '9', 'A', 'B', 'C', 'D', 'E', 'F' };
  uart_putc(hex[n & 15]);
}

void uart_printf(char *format, ...)
{
  char c;
  int i;
  long n;
  
  va_list a;
  va_start(a, format);
  while((c = *format++)) {
    if(c == '%') {
      switch(c = *format++) {
      case 's': // String
        uart_puts(va_arg(a, unsigned char*));
        break;
      case 'c':// Char
        uart_putc(va_arg(a, int));
        break;
      case 'i':// 16 bit Integer
      case 'u':// 16 bit Unsigned
        i = va_arg(a, int);
        if(c == 'i' && i < 0) i = -i, uart_putc('-');
        xtoa((unsigned)i, dv + 5);
        break;
      case 'l':// 32 bit Long
      case 'n':// 32 bit uNsigned loNg
        n = va_arg(a, long);
        if(c == 'l' && n < 0) n = -n, uart_putc('-');
        xtoa((unsigned long)n, dv);
        break;
      case 'x':// 16 bit heXadecimal
        i = va_arg(a, int);
       /* uart_puth(i >> 12);
        uart_puth(i >> 8);*/
        uart_puth(i >> 4);
        uart_puth(i);
        break;
      case 0: return;
      default: goto bad_fmt;
      }
    } else
  bad_fmt: uart_putc(c);
  }
  va_end(a);
}

#endif