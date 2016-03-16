/*
 * Inteligent Dongle
 * Config & Main routine.
 * Copyright (C) 2016-03-16 K.Ohta <whatisthis.sowhat ai gmail.com>
 * License: GPLv2+LE, because use Microchip's XC8 compiler to build.
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

#include <xc.h>
#include <signal.h>

/*
 * Config words.
 */
// CONFIG1
#pragma config FOSC = INTOSC    // Use internal Oscilator.
#pragma config WDTE = OFF       // Not using WDT
#pragma config PWRTE = ON       // Use warm start.
#pragma config MCLRE = ON      // Use 1 pin instead of RESET.
#pragma config CP = OFF         // Not protect program-memory.
#pragma config CPD = OFF        // Not protect data-meomory,
#pragma config BOREN = OFF     // BOR enabled.
#pragma config CLKOUTEN = OFF   // Not output clock to RA6.
#pragma config IESO = OFF       // Not changing to external clock.
#pragma config FCMEN = OFF      // Not using fail-safe colck.

// CONFIG2
#pragma config WRT = OFF        // Not protect flash (program) memories.
#pragma config PLLEN = OFF      // PLL disabled
#pragma config STVREN = ON      // Reset when stack corrupted.
#pragma config BORV = LO        // BOR = low voltage.
#pragma config LVP = OFF        // Not using low-voltage-programming.

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

// Set Timer 1 : 1Tick = 1ms, but count per 1/62.5 ms.
// Timer limit : MAYBE 1048ms.
void set_timer1(unsigned int tick)
{
    unsigned long t = (unsigned long)tick;
    t = t * (500 / 4) ;
    t = t / 2;
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
    set_timer1(tick);
    while(intstat_tmr1 == 0) {};
    intstat_tmr1 = 0;
}
void accept_command(void)
{
    LATBbits.LATB5 = 1;
    wait_tmr1(50);
    LATBbits.LATB5 =  0;
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

// Check push switches: For polling.
void check_sw1(void)
{
    int i;

    if(PORTBbits.RB0 == 0) {
        sw1_pressed = 0;
        for(i = 0; i < 10; i++) {
            wait_tmr1(4);
            if(PORTBbits.RB0 != 0) return;
        }
        if(i >= 10) sw1_pressed = 1;
    }
}

void check_sw2(void)
{
    int i;

    if(PORTBbits.RB1 == 0) {
        sw2_pressed = 0;
        for(i = 0; i < 10; i++) {
            wait_tmr1(4);
            if(PORTBbits.RB1 != 0) break;
        }
        if(i >= 10) sw2_pressed = 1;
    }
}

void check_sw3(void)
{
    int i;

    if(PORTBbits.RB2 == 0) {
        sw3_pressed = 0;
        for(i = 0; i < 10; i++) {
            wait_tmr1(4);
            if(PORTBbits.RB2 != 0) break;
        }
        if(i >= 10) sw3_pressed = 1;
    }
}

// Change using circuit of frontend.
// 0 : MW/SW using +100MHz mixer from 1st input.
// 1: Pass through from 1st input.Mostly FM broadcast
//    and VHF AIR band.
// 2: Pass through from 2nd input. Mostly higher
//    frequencies (i.e. 430MHz band).
// Anther values are ignored (not change).
void change_receiver(int type)
{
    switch(type){
        case receiver_swmw:
            LATC = 0b00000001;
            break;
        case receiver_fm:
            LATC = 0b00000010;
            break;
        case receiver_uhf:
            LATC = 0b00000100;
            break;
    }
}

// Interrupt handler.
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

    if(INTCONbits.IOCIF == 1) {
        INTCONbits.IOCIF = 0;
        if(IOCBFbits.IOCBF0 == 1) {
           IOCBFbits.IOCBF0 = 0;
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
    }

    if(PIR1bits.ADIF == 1) {
        PIR1bits.ADIF = 0;
    }
}

int main(void)
{
    int i;
	// Clock is internal RC(MF): 500KHz.
    OSCCON = 0b00111010 ;

	// PORTA: Still not used.
	// But, DAC will be using for attenuator.
    //ANSELA = 0x00;

	// PORTB : UI switches and LEDs.
	// B0 : SW1 (input)
	// B1 : SW2 (input)
	// B2 : SW3 (input)
	// B3 : SW4 (input) : Reserved.
	// B4 : LED1 (output) : Positive logic.
	// B5 : LED2 (output) : Positive Logic.
	// B6 : ICSP (maybe output)
	// B7 : ICSP (maybe output)
    ANSELB = 0b00000000;
	
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB3 = 0;
    
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;
    
    LCDCONbits.LCDEN = 0;
    SSPCONbits.SSPEN = 0;
	
	// PORTC: Switches of frontend.
	// C0: MW/SW(Input1)
	// C1: FM(Input1)
	// C2: Input2
	// C4, C5 : I2C (Reserved now)
	// C6, C7 : Usart (Reserved now).
    TRISC = 0b11111000;
    LATC = 0b11111100;
    

	// Enable interrupts:
    INTCONbits.INTE = 1;
    WDTCONbits.SWDTEN = 0;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    INTCONbits.IOCIE = 1;
    INTCONbits.IOCIF = 0;
	
    OPTION_REGbits.nWPUEN = 1;

	// Clear timer:
    intstat_tmr1 = 0;
    reset_switches();
    receiver_mode = receiver_swmw;
    change_receiver(receiver_mode);
	// LED1 = OFF, LED2 = OFF.
    LATBbits.LATB4 =  1;
    LATBbits.LATB5 =  0;
	// Main Loop.
    do {
        reset_switches();
		// Sleep until button(s) will be pressed. 
        SLEEP();
        if(sw3_pressed != 0) {
            receiver_mode++;
            if(receiver_mode >= 3) receiver_mode = 0;
       }
        if(sw1_pressed != 0) {
            receiver_mode--;
            if(receiver_mode < 0) receiver_mode = 2;
        }
       change_receiver(receiver_mode);
       accept_command();
    } while(1);
}
