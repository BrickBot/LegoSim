/*! \file   include/sys/h8.h
    \brief  H8/3297 processor registers(Simulator)
    \author Markus L. Noga <noga@inrialpes.fr>, Thomas Roeblitz <roeblitz@informatik.hu-berlin.de>
*/
    
/*
 *  The contents of this file are subject to the Mozilla Public License
 *  Version 1.0 (the "License"); you may not use this file except in
 *  compliance with the License. You may obtain a copy of the License at
 *  http://www.mozilla.org/MPL/
 *
 *  Software distributed under the License is distributed on an "AS IS"
 *  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 *  License for the specific language governing rights and limitations
 *  under the License.
 *
 *  The Original Code is legOS code, released March 19, 1999.
 *
 *  The Initial Developer of the Original Code is Markus L. Noga.
 *  Portions created by Markus L. Noga are Copyright (C) 1999
 *  Markus L. Noga. All Rights Reserved.
 *
 *  Contributor(s): Markus L. Noga <noga@inrialpes.fr>
 */
    
#ifndef __sim_sys_h8_h__
#define __sim_sys_h8_h__

//
// 16 bit timer registers 
//
#define T_IER	*((unsigned char*)0xff90)	// interrupt enable
#define T_CSR	*((unsigned char*)0xff91)	// control & status
#define T_COUNT *((unsigned     *)0xff92)       // the counter
#define T_OCRA	*((unsigned     *)0xff94)       // output compare A
#define T_OCRB	T_OCRA				// shared address with B
#define T_CR	*((unsigned char*)0xff96)	// control
#define T_OCR	*((unsigned char*)0xff97)	// output control
#define T_ICRA  *((unsigned     *)0xff98)       // input capture A
#define T_ICRB  *((unsigned     *)0xff9A)       // input capture B
#define T_ICRC  *((unsigned     *)0xff9C)       // input capture C
#define T_ICRD  *((unsigned     *)0xff9E)       // input capture D


//
// TIER bitmasks
//
#define TIER_ENABLE_ICA		0x80		// input capture IRQ enables
#define TIER_ENABLE_ICB		0x40
#define TIER_ENABLE_ICC		0x20
#define TIER_ENABLE_ICD		0x10
#define TIER_ENABLE_OCA		0x08		// output compare IRQ enables
#define TIER_ENABLE_OCB		0x04
#define TIER_ENABLE_OF		0x02		// overflow IRQ enable
#define TIER_RESERVED		0x01		// always set.

//
// TCSR bitmasks
//
#define TCSR_ICA		0x80		// input capture events
#define TCSR_ICB		0x40
#define TCSR_ICC		0x20
#define TCSR_ICD		0x10
#define TCSR_OCA		0x08		// output compare events
#define TCSR_OCB		0x04
#define TCSR_OF			0x02		// overflow event
#define TCSR_RESET_ON_A		0x01		// reset counter on match A

//
// TCR bitmasks
//
#define TCR_A_RISING		0x80		// input capture on rising
#define TCR_B_RISING		0x40		// edge. if not set -> lower
#define TCR_C_RISING		0x20
#define TCR_D_RISING		0x10
#define TCR_BUFFER_A		0x08		// buffer A in C
#define TCR_BUFFER_B		0x04		// buffer B in D
#define TCR_CLOCK_2		0x00		// clock = pclock / 2
#define TCR_CLOCK_8		0x01		// clock = pclock / 8
#define TCR_CLOCK_32		0x02		// clock = pclock / 32
#define TCR_CLOCK_EXT		0x03		// external clock, rising edge

//
// TOCR bitmasks
//
#define TOCR_OCRA               0x00            // select register to write
#define TOCR_OCRB               0x10
#define TOCR_ENABLE_A           0x08            // enable output signals
#define TOCR_ENABLE_B           0x04
#define TOCR_HIGH_LEVEL_A       0x02            // set output to high for match
#define TOCR_HIGH_LEVEL_B       0x01


//
// 8 bit timer registers
//
#define STCR	0xffc3		// serial / timer control register

#define T0_CR	0xffc8		// timer 0 control
#define T0_CSR	0xffc9		// timer 0 control / status
#define T0_CORA	0xffca		// timer 0 constant A
#define T0_CORB	0xffcb		// timer 0 constant B
#define T0_CNT	0xffcc		// timer 0 counter

#define T1_CR	*((unsigned char*)0xffd0)       // timer 1
#define T1_CSR	*((unsigned char*)0xffd1)
#define T1_CORA	*((unsigned char*)0xffd2)
#define T1_CORB	*((unsigned char*)0xffd3)
#define T1_CNT	*((unsigned char*)0xffd4)


//
// CR flag bitmasks
//
#define CR_ENABLE_IRQA		0x40
#define CR_ENABLE_IRQB		0x80
#define CR_ENABLE_IRQO		0x20

#define CR_CLEAR_NEVER		0x00
#define CR_CLEAR_ON_A		0x08	
#define CR_CLEAR_ON_B		0x10
#define CR_CLEAR_ON_EXTERN	0x18

// I don't define speeds here because the STCR bits are involved, too.
// Also, speeds differ for T0 and T1.


//
// CSR bits
//
#define CSR_MATCH_A	0x6
#define CSR_MATCH_B	0x7
#define CSR_OVERFLOW	0x5


//
// Serial port
//
#define S_RDR		*((unsigned char*volatile)0xffdd)	// receive data
#define S_TDR		*((unsigned char*volatile)0xffdb)	// transmit data
#define S_MR		*((unsigned char*volatile)0xffd8)	// mode
#define S_CR		*((unsigned char*volatile)0xffda)	// control
#define S_SR		*((unsigned char*volatile)0xffdc)	// status
#define S_BRR		*((unsigned char*volatile)0xffd9)	// bit rate
#define S_TCR		*((unsigned char*volatile)0xffc3)	// serial/timer ctrl

//
// register bitmasks
//
#define SMR_SYNC	0x80		// in sync mode, the other settings
#define SMR_ASYNC	0x00		// have no effect.
#define SMR_7BIT	0x40
#define SMR_8BIT	0x00
#define SMR_P_NONE	0x00
#define SMR_P_EVEN	0x20
#define SMR_P_ODD	0x30
#define SMR_1STOP	0x00
#define SMR_2STOP	0x08
#define SMR_MP		0x04		// multiprocessing -> no parity
					// for mp, mpe in STRC must be set
#define SMR_CLOCK	0x00		// clock rate for baud rate generator
#define SMR_CLOCK_4	0x01		// pclock / 4
#define SMR_CLOCK_16	0x02		// pclock / 16
#define SMR_CLOCK_64	0x03		// pclock / 64


#define SCR_TX_IRQ	0x80		// TIE transmit irq enable
#define SCR_RX_IRQ	0x40		// RIE receive / recv err irq enable
#define SCR_TRANSMIT	0x20		// enable transmission
#define SCR_RECEIVE	0x10		// enable receiving
#define SCR_MP_IRQ	0x08		// multiprocessing receive irq
#define SCR_TE_IRQ	0x04		// TEI transmit end irq enable
#define SCR_INT_CLOCK	0x00		// internal clock source
#define SCR_EXT_CLOCK	0x02		// external clock source
#define SCR_CLOCK_OUT	0x01		// output internal clock to SCK pin


#define SSR_TRANS_EMPTY	0x80		// transmit buffer empty
#define SSR_RECV_FULL	0x40		// receive buffer full
#define SSR_OVERRUN_ERR	0x20		// overrun error
#define SSR_FRAMING_ERR	0x10		// framing error
#define SSR_PARITY_ERR	0x08		// parity error
#define SSR_TRANS_END	0x04		// transmission end because buffer empty
#define SSR_MP		0x02		// multiprocessor
#define SSR_MP_TRANSFER	0x01		// multiprocessor bit transfer


//
// values for the bit rate register BRR
// assuming CMR_CLOCK selected on 16 MHz processor
// error <= 0.16%
//

#define B2400		207
#define B4800		103
#define B9600		51
#define B19200		25
#define B38400		12


//
// A/D converter
//

#define AD_A_H	*((unsigned char*)0xffe0)	// data registers (a-d, low/high)
#define AD_A_L	*((unsigned char*)0xffe1)	// read only.
#define AD_B_H	*((unsigned char*)0xffe2)	// bits 15-6: conversion result
#define AD_B_L	*((unsigned char*)0xffe3)	// bits 5-0: reserved, 0
#define AD_C_H	*((unsigned char*)0xffe4)
#define AD_C_L	*((unsigned char*)0xffe5)
#define AD_D_H	*((unsigned char*)0xffe6)
#define AD_D_L	*((unsigned char*)0xffe7)

//#define AD_A	*((unsigned*)0xffe0)		// data registers (a-d)
//#define AD_B	*((unsigned*)0xffe2)		// bits 15-6: conversion result
//#define AD_C	*((unsigned*)0xffe4)
#define AD_D	*((unsigned*)0xffe6)

extern unsigned ad_a;
extern unsigned ad_b;
extern unsigned ad_c;

#define AD_A ad_a
#define AD_B ad_b
#define AD_C ad_c
#define AD_CSR	*((unsigned char*)0xffe8)	// control & status register
#define AD_CR	*((unsigned char*)0xffe9)	// control register


//
// A/D CSR bitmasks
//

#define	ADCSR_END		0x80
#define ADCSR_ENABLE_IRQ	0x40
#define ADCSR_START		0x20
#define ADCSR_SCAN		0x10		// enable scan mode
#define	ADCSR_TIME_266		0x00		// select A/D conversion time
#define ADCSR_TIME_134		0x08

#define ADCSR_GROUP_0		0x00		// select scan group
#define ADCSR_GROUP_1		0x04

#define ADCSR_AN_0		0x00		// first group of inputs to convert
#define ADCSR_AN_1		0x01
#define ADCSR_AN_2		0x02
#define ADCSR_AN_3		0x03		// in scan mode: AN0-AN3

//
// A/D CR bitmasks
//

#define ADCR_EXTERN		0x80		// allow external triggering


//
// system control register
// FIXME: incomplete

#define SYSCR	*((unsigned char*)0xffc4)

#define SYSCR_SOFTWARE_STANDBY	0x80		// software standby flag

//
// I/O ports
// FIXME: incomplete
//

#define PORT6_DDR   *((unsigned char*)0xffb9)   // data direction 0:read
#define PORT6       *((unsigned char*)0xffbb)

#endif
