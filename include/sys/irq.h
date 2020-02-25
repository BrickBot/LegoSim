/*! \file   include/sys/irq.h
    \brief  RCX redirected IRQ vectors
    \author Markus L. Noga <noga@inrialpes.fr>
    
    Lego Mindstorms RCX IRQ redirection vector table
    All redirected handlers can assume r6 to be saved
    All redirected handlers must return with rts, *not* rte.
    
    \warning Incomplete.
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


#ifndef __sys_irq_h__
#define __sys_irq_h__

#define RESET_IRQ	*((void**)0xfd90)	// reset vector

#define OCIA_IRQ	*((void**)0xfda2)	// 16-bit Timer Output Compare A

#define	T0_IRQA		*((void**)0xfda8)	// Timer 0 CMI0A
#define T0_IRQB		*((void**)0xfdaa)	//         CMI0B  
#define T0_IRQO		*((void**)0xfdac)	//         OVI0  

#define	T1_IRQA		*((void**)0xfdae)	// Timer 1 CMI1A  
#define T1_IRQB		*((void**)0xfdb0)	//         CMI1B  
#define T1_IRQO		*((void**)0xfdb2)	//         OVI1  

#define RX_ERROR_IRQ	*((void**)0xfdb4)	// Serial  ERI receive error
#define RX_IRQ		*((void**)0xfdb6)	//         RXI byte received
#define TX_IRQ		*((void**)0xfdb8)	//	   TXI transmit buffer empty
#define TX_END_IRQ	*((void**)0xfdba)	//         TEI transmit end

#define AD_IRQ		*((void**)0xfdbc)	// A/D converter
#define WATCHDOG_IRQ	*((void**)0xfdbe)	// Watchdog timer interrupt vector


#define OCIA_ROM        ((void*)0x04cc)         // OCIA ROM handler
#define OCIA_ROM_RETURN ((void*)0x04d4)         // return from user handler
#define RTS_INSTRUCTION ((void*)0x046a)         // a RTS instruction

//
// convenient macros
//

#define disable_irqs()	__asm__ __volatile__("orc  #0x80,ccr\n":::"cc")
#define enable_irqs()	__asm__ __volatile__("andc #0x7f,ccr\n":::"cc")

#endif
