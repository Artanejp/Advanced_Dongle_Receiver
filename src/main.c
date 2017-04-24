/*
 * OpenI2CRADIO
 * Config & Main routine.
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
#include <stdlib.h>
#include <string.h>

#include "lcd_aqm0802.h"

#include <xc.h>
#include <signal.h>


/*
 * Config words.
 */
// コンフィギュレーション１の設定
#pragma config FOSC = INTOSC    // 内部ｸﾛｯｸを使用する(INTOSC)
#pragma config WDTE = OFF       // ｳｵｯﾁﾄﾞｯｸﾞﾀｲﾏｰ無し(OFF)
#pragma config PWRTE = ON       // 電源ONから64ms後にﾌﾟﾛｸﾞﾗﾑを開始する(ON)
#pragma config MCLRE = ON      // 外部ﾘｾｯﾄ信号は使用せずにﾃﾞｼﾞﾀﾙ入力(RA5)ﾋﾟﾝとする(OFF)
#pragma config CP = OFF         // ﾌﾟﾛｸﾞﾗﾑﾒﾓﾘｰを保護しない(OFF)
#pragma config CPD = OFF        // ﾃﾞｰﾀﾒﾓﾘｰを保護しない(OFF)
#pragma config BOREN = OFF     // 電源電圧降下常時監視機能ON(ON)
#pragma config CLKOUTEN = OFF   // CLKOUTﾋﾟﾝをRA6ﾋﾟﾝで使用する(OFF)
#pragma config IESO = OFF       // 外部・内部ｸﾛｯｸの切替えでの起動はなし(OFF)
#pragma config FCMEN = OFF      // 外部ｸﾛｯｸ監視しない(FCMEN_OFF)

// コンフィギュレーション２の設定
#pragma config WRT = OFF        // Flashﾒﾓﾘｰを保護しない(OFF)
#pragma config PLLEN = OFF      // 動作クロックを32MHzでは動作させない(OFF)
#pragma config STVREN = ON      // スタックがオーバフローやアンダーフローしたらリセットをする(ON)
#pragma config BORV = LO        // 電源電圧降下常時監視電圧(2.5V)設定(HI)
#pragma config LVP = OFF        // 低電圧プログラミング機能使用しない(OFF)

/*
 * Statuses
  */

enum {
    receiver_swmw = 0,
    receiver_fm = 1,
    receiver_uhf = 2,
};
int intstat_tmr1;
int receiver_mode;
int sw1_pressed;
int sw2_pressed;
int sw3_pressed;
unsigned char dac_level;
//#define _LCD_DEBUG 1
char sbuf[64];

void out_dac(void);
// Set Timer 1 : 1Tick = 1ms, but count per 2.048ms
void set_timer1(unsigned int tick)
{
    unsigned long t = (unsigned long)tick;
    t = t * (2000 / (4 * 2)) ;
    //t = t / 2;
    if(t > 65535) t = 65535;
    if(t == 0) return;
    tick = 65535 - (unsigned int)t + 1;
    
    intstat_tmr1 = 0;
    
    T1CONbits.TMR1ON = 1;
    PIE1bits.TMR1IE = 1;
    PIE1bits.TMR1GIE = 0;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
  
    T1CONbits.nT1SYNC = 1;
    T1CONbits.TMR1CS = 0;
    T1CONbits.T1CKPS = 2; // 1/4
    T1CONbits.T1OSCEN = 0;
    
    TMR1H = tick / 256;
    TMR1L = tick % 256;
//    T1CONbits.TMR1ON = 1;
}

void wait_tmr1(unsigned int tick)
{
        set_timer1(tick); // 0.1 Sec
        while(intstat_tmr1 == 0) {};
        intstat_tmr1 = 0;
}
void accept_command(void)
{
        LATBbits.LATB4 = 1;
        wait_tmr1(50);
        LATBbits.LATB4 =  0;
}

void reset_switches(void)
{
    INTCONbits.IOCIE = 1;
    INTCONbits.IOCIF = 0;
    IOCBPbits.IOCBP = 0x00;
    IOCBNbits.IOCBN = 0b00000111;
    sw1_pressed = 0;
    sw2_pressed = 0;
    sw3_pressed = 0;
}

void check_sw1(void)
{
    int i;
    //IOCBPbits.IOCBP = 0x00;
    if(PORTBbits.RB0 == 0) {
        //IOCBNbits.IOCBN = 0b00000110;
        sw1_pressed = 0;
        for(i = 0; i < 10; i++) {
            wait_tmr1(4);
            if(PORTBbits.RB0 != 0) return;
        }
        if(i >= 10) sw1_pressed = 1;
    }
    //IOCBNbits.IOCBN = 0x00;
}

void check_sw2(void)
{
    int i;
    //IOCBPbits.IOCBP = 0x00;
    if(PORTBbits.RB1 == 0) {
        //IOCBNbits.IOCBN = 0b00000101;
        sw2_pressed = 0;
        for(i = 0; i < 10; i++) {
            wait_tmr1(4);
            if(PORTBbits.RB1 != 0) break;
        }
        if(i >= 10) sw2_pressed = 1;
    }
    //IOCBNbits.IOCBN = 0x00;
}

void check_sw3(void)
{
    int i;
    //IOCBPbits.IOCBP = 0x00;
    if(PORTBbits.RB2 == 0) {
        //IOCBNbits.IOCBN = 0b00000101;
        sw3_pressed = 0;
        for(i = 0; i < 10; i++) {
            wait_tmr1(4);
            if(PORTBbits.RB2 != 0) break;
        }
        if(i >= 10) sw3_pressed = 1;
    }
    //IOCBNbits.IOCBN = 0x00;
}

void change_receiver(int type)
{
    unsigned char ntmp = LATA & 0b11100011;
    switch(type){
        case receiver_swmw:
            aqm0802_locate_8x2(LCD_I2CADDR, 0, 1);
            aqm0802_putstr(LCD_I2CADDR, "        ");
            aqm0802_locate_8x2(LCD_I2CADDR, 0, 1);
            aqm0802_putstr(LCD_I2CADDR, "SW/MW");
            LATA = ntmp | 0b00000100 ;
            LATC = 0b00000110;
            break;
        case receiver_fm:
            aqm0802_locate_8x2(LCD_I2CADDR, 0, 1);
            aqm0802_putstr(LCD_I2CADDR, "        ");
            aqm0802_locate_8x2(LCD_I2CADDR, 0, 1);
            aqm0802_putstr(LCD_I2CADDR, "FM1");
            LATC = 0b00000101;
            LATA = ntmp | 0b00001000;
            break;
        case receiver_uhf:
            aqm0802_locate_8x2(LCD_I2CADDR, 0, 1);
            aqm0802_putstr(LCD_I2CADDR, "        ");
            aqm0802_locate_8x2(LCD_I2CADDR, 0, 1);
            aqm0802_putstr(LCD_I2CADDR, "ANT2");
            LATC = 0b00000011;
            LATA = ntmp | 0b00010000;
            break;
    }
}

void interrupt intr_handler(void)
{
    
    if(INTCONbits.TMR0IF == 1) {
        INTCONbits.TMR0IF = 0;
        TMR0 = 0;
    }
    if(INTCONbits.INTF == 1) INTCONbits.INTF = 0;
    if(PIR1bits.TMR1IF != 0) { // Tick
        INTCONbits.T0IE = 0;
        INTCONbits.TMR0IE = 0;
        PIR1bits.TMR1IF = 0;
        T1CONbits.TMR1ON = 0;
        T1GCONbits.TMR1GE = 0;
        TMR1H = 0;
        TMR1L = 0;
        T1CONbits.T1OSCEN = 0;
        if(intstat_tmr1 == 0) intstat_tmr1 = 1;
    }
#if 1
    if(INTCONbits.IOCIF == 1) {
        INTCONbits.IOCIF = 0;
        if(IOCBFbits.IOCBF0 == 1) {
           IOCBFbits.IOCBF0 = 0;
           //check_sw1();
           sw1_pressed = 1;
       }
        if(IOCBFbits.IOCBF1 == 1) {
            IOCBFbits.IOCBF1 = 0;
            sw2_pressed = 1;
        }
        if(IOCBFbits.IOCBF2 == 1) {
           IOCBFbits.IOCBF2 = 0;
           sw3_pressed = 1;
        }
        INTCONbits.IOCIF = 0;
        LATBbits.LATB4 = (LATBbits.LATB4 == 0) ? 1 : 0;
    }
#endif
    if(PIR1bits.ADIF == 1) {
        PIR1bits.ADIF = 0;
    }
}

void set_dac(unsigned char level)
{
    if((level == 0xff) && (DACCON0bits.DACEN != 0) ) {
        //DACCON0bits.DACOE = 0;
        //FVRCONbits.FVREN = 0;
        //DACCON0bits.DACEN = 0;
        DACCON1 = 0x00;
    } else {
        if(DACCON0bits.DACEN == 0) {
            DACCON0bits.DACPSS= 0x02; // FVR Buffer 2
            DACCON0bits.DACNSS = 0;
            DACCON0bits.DACEN = 1;
        }
        FVRCONbits.FVREN = 1;
        FVRCONbits.ADFVR = 0x00;
        FVRCONbits.CDAFVR = 0x01;
        while(FVRCONbits.FVRRDY == 0) {}
        DACCON1 = level & 0x1f;
        DACCON0bits.DACOE = 1;
    }
}

void out_dac(void)
{
    unsigned char nn;
    dac_level += 1;
    if(dac_level > 8) dac_level = 0;
    switch(dac_level) {
        case 0:
            nn = 0xff;
            break;
        case 8:
            nn = 0x1f;
            break;
        default:
            nn = dac_level * 3 + 8;
            break;
    }
    sprintf(sbuf, "ATT: %d", dac_level);
    aqm0802_locate_8x2(LCD_I2CADDR, 0, 0);
    aqm0802_putstr(LCD_I2CADDR, "        ");
    aqm0802_locate_8x2(LCD_I2CADDR, 0, 0);
    aqm0802_putstr(LCD_I2CADDR, sbuf);
    set_dac(nn);
}


int main(void)
{
    int i;
    OSCCON = 0b01100010 ; // 2MHz, HF intosc
    
    dac_level = 0x00;
    sw1_pressed = 0;
    sw2_pressed = 0;
    sw3_pressed = 0;
       //ANSELA = 0x00;
    //TRISB = 0b00000111;
//    ANSELBbits.ANSB0 = 1;
    TRISA = 0b11000011;
    LATA  = 0b11011111;

    TRISB = 0b11000111;
    
    LCDCONbits.LCDEN = 0;
    SSPCONbits.SSPEN = 0;
    
    TRISC = 0b11100000;
    LATC = 0b11111100;
    
    FVRCONbits.FVREN = 1;
    FVRCONbits.ADFVR = 0x00;
    FVRCONbits.CDAFVR = 0x01;
    while(FVRCONbits.FVRRDY == 0) {}
    
    DACCON0bits.DACOE = 1;
    DACCON0bits.DACPSS= 0x02; // FVR Buffer 2
    DACCON0bits.DACNSS = 0;
    DACCON0bits.DACEN = 1;
    DACCON1 = 0x00;
    
    INTCONbits.INTE = 1;
    WDTCONbits.SWDTEN = 0;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    INTCONbits.IOCIE = 1;
    INTCONbits.IOCIF = 0;
    OPTION_REGbits.nWPUEN = 1;
    intstat_tmr1 = 0;
    reset_switches();
    
    aqm0802_init(LCD_I2CADDR, 0xff);
    aqm0802_putstr(LCD_I2CADDR, "WELCOME!");
    aqm0802_locate_8x2(LCD_I2CADDR, 0, 0);
    
    receiver_mode = receiver_swmw;
    change_receiver(receiver_mode);
    LATBbits.LATB4 =  1;
    LATBbits.LATB5 =  1;
    
    
   //reset_switches();
    do {
        reset_switches();
        SLEEP();
        if(sw3_pressed != 0) {
            receiver_mode++;
            if(receiver_mode >= 3) receiver_mode = 0;
            sw1_pressed = 0;
       }
        if(sw1_pressed != 0) {
            receiver_mode--;
            if(receiver_mode < 0) receiver_mode = 2;
        }
        if(sw2_pressed != 0) {
            out_dac();
        }
       change_receiver(receiver_mode);
       accept_command();
    } while(1);
}