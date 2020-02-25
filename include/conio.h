/*! \file   include/conio.h
    \brief  console input / output (Simulator)
    \author Markus L. Noga <noga@inrialpes.fr>, Thomas Roeblitz <roeblitz@informatik.hu-berlin.de>
    
    \warning Display updates are realized exclusively by lcd_refresh()

    \par Display positions
    Digit display positions are denumerated from right to left, starting
    with 0 for the digit right to the running man. Digit 5 is only partially
    present on the RCXs display.

    \par Native segment masks
    In these bitmasks, bit 0 toggles the middle segment. Bit 1 toggles the 
    top right segment, and the remaining segments are denumerated
    counterclockwise. The dot isn't encoded because it is desirable
*/
    
#ifndef __sim_conio_h_
#define __sim_conio_h_

#include "config.h"

#ifndef NO_CONSOLE_IO

#include "rom/lcd.h"

#define SCROLL_DELAY	500		// scroll delay for cputs

///////////////////////////////////////////////////////////////////////////////
//
// Definitions
//
///////////////////////////////////////////////////////////////////////////////

//
// hex display codes
//
extern const char hex_display_codes[];

#ifndef NO_ASCII

//
// ascii display codes
// only lower 128 bit, please!
//
extern const char ascii_display_codes[];
		
#endif	// NO_ASCII

///////////////////////////////////////////////////////////////////////////////
//
// Functions
//
///////////////////////////////////////////////////////////////////////////////

//
// delay by approximately d ms
// 
extern void delay(unsigned d);

//
// delay by approximately d ms
// 
extern void msleep(unsigned d);


//
// display native mode segment mask at fixed display position
//
// encoding: middle=1, topr=2, top=4, ... (counterclockwise)
// dot not included because not reliably present.
//
extern void cputc_native_0(char mask);
extern void cputc_native_1(char mask);
extern void cputc_native_2(char mask);
extern void cputc_native_3(char mask);
extern void cputc_native_4(char mask);
extern void cputc_native_5(char mask);

// a dispatcher for the fixed position versions
extern void cputc_native(char mask,int pos);


//
// display a hexword
//
extern void cputw(unsigned word);


#ifndef NO_ASCII

//
// display ascii character c at display position pos
//
extern inline void cputc(char c,int pos) {
  cputc_native(ascii_display_codes[c & 0x7f],pos);
}


//
// display a zero terminated string s
// max first 5 characters
//
extern void cputs(char *s);

#endif	// NO_ASCII

#endif	// NO_CONSOLE_IO

#endif
