/*
 * OpenI2CRADIO
 * I2C Handler
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

#include <stdarg.h>
#include <stdio.h>

#include "i2c_io.h"



void i2c1_init(void)
{
 //   unsigned char b;
//    b = _SSPEN | _SSPM3;
 //   SSPCON1bits.SSPEN = 1;
//    SSPCON1bits.SSPM3 = 1;
//    SSPADD = 4; // Fosc:8000[KHz] / (4 * I2C:Clock:400[KHz]) - 1
    SSPSTAT = 0x80;
    PIR1bits.SSPIF = 0;
    PIE1bits.SSPIE = 0;
    SSPCON3bits.PCIE = 0;
    SSPCON3bits.SCIE = 0;
    SSPCON3bits.SDAHT = 1;
 
//    IPR1bits.SSPIP = 1;
}
void i2c2_init(void)
{
    i2c1_init();
}

void OpenI2CMaster(void)
{
    // 2MHZ Clock, I2C = 100KHz,
    SSPADD = 4;
    SSPCON1 = 0x08;  // SSPM = 0x08
    SSPCON2 = 0x00;
    SSPCON1bits.SSPEN = 1;
    
}

void CloseI2C(void)
{
    SSPCON1bits.SSPEN = 0;
}

void WaitI2CMaster(void)
{
    while((SSPSTATbits.R_nW == 1) || ((SSPCON2 & 0x1f) != 0)) {}
}

void StartI2CMaster(void)
{
//    SSPCON2bits.RCEN = 0;
    SSPCON2bits.SEN = 1;
    WaitI2CMaster();
}

void WriteI2CMaster(unsigned char data)
{
    SSPBUF = data;
    WaitI2CMaster();
}

void StopI2CMaster(void)
{
    SSPCON2bits.PEN = 1;
    WaitI2CMaster();
}

// I2C_IO
void i2c_send_byte(unsigned char addr, unsigned char reg, unsigned char data)
{
    OpenI2CMaster();
    StartI2CMaster();
    //while (SSPCON2bits.ACKSTAT);
    WriteI2CMaster(addr);
    //while(SSPCON2bits.ACKSTAT);
    WriteI2CMaster(reg);
    //while(SSPCON2bits.ACKSTAT);
    WriteI2CMaster(data);
    StopI2CMaster();
    //while (SSPCON2bits.PEN);
    CloseI2C();
}

void AckI2C(void)
{
    SSPCON2bits.ACKDT = 0;
}

void NotAckI2C(void)
{
    SSPCON2bits.ACKDT = 1;
}

unsigned char i2c_read_byte(unsigned char addr, unsigned char reg)
{
    unsigned char c;
    OpenI2CMaster();
    StartI2CMaster();
    while(SSPCON2bits.SEN);
    WriteI2CMaster(addr);
  //  delay1ktcy(8);
    //while(SSPCON2bits.SEN);
    WriteI2CMaster(reg);
  //  delay1ktcy(8);
    StopI2CMaster();
    WaitI2CMaster();
    StartI2CMaster();
    WriteI2CMaster(addr | 1);
    if (!SSPCON2bits.ACKSTAT){
      SSPCON2bits.RCEN = 1;
      while(SSPCON2bits.RCEN);
      NotAckI2C();
      while (SSPCON2bits.ACKEN);
      StopI2CMaster();
      while (SSPCON2bits.PEN);
    }
    c = SSPBUF;
    CloseI2C();
    return c;

}
