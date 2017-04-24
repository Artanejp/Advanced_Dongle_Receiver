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
#include "generaldef.h"
#include "lcd_aqm0802.h"

static void _AQM0802_TC_WAIT(void)
{
#ifdef __SDCC
   delay10tcy(4);
#else
//   __delay_us(28);
   wait_tmr1(1);
#endif
}

void lcd_waitbusy(unsigned char addr)
{
    _AQM0802_SHORT_WAIT(); // Wait 0.05ms
}

static void sendcmd(unsigned char addr, unsigned char cmd, unsigned char data)
{
    i2c_send_byte(addr, cmd, data);
    // End of Command.
    _AQM0802_TC_WAIT(); // Wait 50us
}

static void sendonce(unsigned char addr, unsigned char cmd)
{
    i2c_send_byte(addr, 0x00, cmd);
    _AQM0802_TC_WAIT(); // Wait 50us
}

void aqm0802_putchar(unsigned char addr, unsigned char c)
{
    sendcmd(addr, 0x40, c);
    _AQM0802_TC_WAIT(); // Wait 0.1ms
}


void aqm0802_setdataramaddress(unsigned char addr, unsigned char pos)
{
   unsigned char cmd = (pos & 0x7f) | 0x80;
   sendonce(addr, cmd);
}


void aqm0802_locate_8x2(unsigned char addr, char x, char y)
{
    unsigned char ramaddr;
    if((x >= 16) || (x < 0)) return; 
    if((y >= 2) || (y < 0)) return; 
    ramaddr = (y == 0)? x & 0x1f : (x & 0x1f) | 0x40;
    aqm0802_setdataramaddress(addr, ramaddr);
}

/*
 * For using
 */
void aqm0802_home(unsigned char addr)
{
    sendonce(addr, 0x02);
    _AQM0802_LONG_WAIT(); // Wait 5ms
}

void aqm0802_cls(unsigned char addr)
{
    sendonce(addr, 0x01);
    _AQM0802_LONG_WAIT(); // Wait 5ms
}


void aqm0802_cursordir(unsigned char addr, unsigned char right)
{
    unsigned char cmd;

    cmd = 0x14;
    if(right == 0)  {
        cmd = 0x10;
    }
    sendonce(addr, cmd);
}

void aqm0802_dispcursor(unsigned char addr, unsigned char flag)
{
    unsigned char cmd;
    cmd = 0x0d; // Cursor ON
    if(flag == 0x00) {
        cmd = 0x0c; // Cursor off
    }
    sendonce(addr, cmd);
}

void aqm0802_setcgramaddress(unsigned char addr, unsigned char pos)
{
    unsigned char cmd = (pos & 0x3f) | 0x40;
    sendonce(addr, cmd);
}

void aqm0802_init(unsigned char addr, unsigned char cls)
{
    int i;
    //aqm0802_ioinit(addr);
    for(i = 0; i < 25; i++) wait_tmr1(5);
    sendonce(addr, 0x38); // 2lines, 8x10dot fonts.
    sendonce(addr, 0x39); // 2lines, 8x10dot fonts.
    sendonce(addr, 0x14); // 2lines, 8x10dot fonts.
    sendonce(addr, 0x73); // 2lines, 8x10dot fonts.
    sendonce(addr, 0x56); // 2lines, 8x10dot fonts.
    sendonce(addr, 0x6c); // 2lines, 8x10dot fonts.
    
    for(i = 0; i < 21; i++) wait_tmr1(10);
    sendonce(addr, 0x38); // 2lines, 8x10dot fonts.
    sendonce(addr, 0x01); // 2lines, 8x10dot fonts.
    
    sendonce(addr, 0x39);
    sendonce(addr, 0x54); // Booster ON
    sendonce(addr, 0x7f);  // Contrast = 0x0f
    sendonce(addr, 0x38);
    //sendcmd(addr, 0x28, 0xff); // 2lines, 8x5dot fonts.
//    sendcmd(addr, 0x08, 0xff); // Display OFF.
 //   sendcmd(addr, 0x0c, 0xff); // Display ON.
//    sendcmd(addr, 0x06, 0xff); // Cursor increment,not shift.
   if(cls == 0){
        aqm0802_home(addr);
    } else {
        aqm0802_cls(addr);
    }
//    _ACM1602_LONG_WAIT();
    //sendcmd(addr, 0x06, 0xff);
}

/*
 * Suspend lcd
 */
void aqm0802_suspend(unsigned char addr)
{
    sendcmd(addr, 0x08, 0xff);
    _AQM0802_LONG_WAIT();
}

void aqm0802_resume(unsigned char addr)
{
    sendcmd(addr, 0x0c, 0xff);
    _AQM0802_LONG_WAIT();
}

void aqm0802_putstr(unsigned char addr, char *p)
{
    int i;
    int l;
    l = strlen(p);
    for(i = 0; i < l; i++) aqm0802_putchar(addr, p[i]);
}