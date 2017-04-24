/*
 * OpenI2CRADIO
 * I2C/Parallel LCD ACM1602 Handler, variant of ST7066U.
 * Copyright (C) 2013-06-10 K.Ohta <whatisthis.sowhat ai gmail.com>
 * License: GPL2+LE
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2,
 *  or (at your option) any later version.
 *  This library / program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this library; see the file COPYING. If not, write to the
 *  Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
 *  MA 02110-1301, USA.
 *
 *  As a special exception, if you link this(includeed from sdcc) library
 *  with other files, some of which are compiled with SDCC,
 *  to produce an executable, this library does not by itself cause
 *  the resulting executable to be covered by the GNU General Public License.
 *  This exception does not however invalidate any other reasons why
 *  the executable file might be covered by the GNU General Public License.
 */
#ifndef LCD_AQM0802_H
#define	LCD_AQM0802_H

#include <stdarg.h>
#include <stdio.h>
#include <xc.h>

#include <string.h>

#include "i2c_io.h"
#ifdef	__cplusplus
extern "C" {
#endif

#ifndef LCD_I2CADDR
#define LCD_I2CADDR 0x7c
#endif
// Notes: Master clock = 8MHz(0.125uS)
// Wait 5us
// Wait 50us
#define _AQM0802_SHORT_WAIT()     __delay_us(50)
// Wait 5ms
#define _AQM0802_LONG_WAIT()     __delay_ms(50)
// Wait 125ms
#define _AQM0802_LONG_LONG_WAIT()     idle_time_ms(125)
// Wait 5.5ms
#define _AQM0802_I2C_WAIT()     __delay_us(5500)


extern void aqm0802_cls(unsigned char addr);
extern void aqm0802_putchar(unsigned char addr, unsigned char c);
extern void aqm0802_locate_8x2(unsigned char addr, char x, char y);
extern void aqm0802_home(unsigned char addr);
extern void aqm0802_cursordir(unsigned char addr, unsigned char right);
extern void aqm0802_dispcursor(unsigned char addr, unsigned char flag);
extern void aqm0802_init(unsigned char addr, unsigned char cls);
extern void aqm0802_suspend(unsigned char addr);
extern void aqm0802_resume(unsigned char addr);
extern void aqm0802_putstr(unsigned char addr, char *p);

//extern unsigned char acm1602_getchar(unsigned char addr);

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_AQM0802_H */

