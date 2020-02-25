/*! \file   conio.c
    \brief  console input / output (Simulator)
    \author Markus L. Noga <noga@inrialpes.fr>, Thomas Roeblitz <roeblitz@informatik.hu-berlin.hu-berlin.de>
    
    \warning Display updates are realized exclusively by lcd_refresh()

    \par Display positions
    Digit display positions are denumerated from right to left, starting
    with 0 for the digit right to the running man. Digit 5 is only partially
    present on the RCXs display.

    \par Native segment masks
    In these bitmasks, bit 0 toggles the middle segment. Bit 1 toggles the 
    top right segment, and the remaining segments are denumerated
    counterclockwise. The dot isn't encoded because it is desirable
    to seperate its positioning from the number display code.
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

#include "conio.h"
#include "mem.h"
#include "rom/lcd.h"				// for lcd_refresh()
#include "direct-lcd.h"

#include <pthread.h> // for nanosleep
#include <stdio.h>   // for sprintf

#ifndef NO_CONSOLE_IO

///////////////////////////////////////////////////////////////////////////////
//
// Definitions
//
///////////////////////////////////////////////////////////////////////////////

//! hex display codes
//
const char hex_display_codes[]={
	0x7e,	// 0
   	0x42,	// 1
   	0x37,	// 2
	0x67,	// 3
	0x4b,	// 4
	0x6d,	// 5
	0x7d,	// 6
	0x46,	// 7
	0x7f,	// 8
	0x4f,	// 9
	0x5f,	// A 
	0x79,	// b 
	0x31,	// c
	0x73,	// d 
	0x3d,	// E
	0x1d,	// F
};

#ifndef NO_ASCII

//! ASCII display codes
/*! This is a 7-bit ASCII table only.
*/
const char ascii_display_codes[]={
	0x00,0x20,0x20,0x20,0x20,0x20,0x20,0x20,	// non-printables
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,	// -> underscore
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,	// except 0x00.
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
					
	0x00,	// 32 ' '
	0x42,	// 33 ! 1
	0x0a,	// 34 "
	0x7b,   // 35 #
	0x6d,	// 36 $ 5 S Z
	0x13,	// 37 % /
	0x7d,	// 38 & 6
	0x08,	// 39 '  alt: ` 
	0x3c,	// 40 ( C [ {
	0x66,	// 41 ) ] }
	0x5b,	// 42 * H K X
	0x43,	// 43 +  alt: 0x19 (worse)
	0x10,	// 44 , .  alt: 0x40
	0x01,	// 45 -
	0x10,	// 46 . alt: 0x40
	0x13,	// 47 /

	0x7e,	// 48 0 0
   	0x42,	// 49 1
   	0x37,	// 50 2
	0x67,	// 51 3
	0x4b,	// 52 4
	0x6d,	// 53 5
	0x7d,	// 54 6
	0x46,	// 55 7
	0x7f,	// 56 8
	0x4f,	// 57 9 Q q

	0x21,	// 58 : ; = alt:0x5 (worse)
	0x21,	// 59 ;
	0x31,	// 60 < c alt:0xd (worse)	
	0x21,	// 61 =
	0x61,	// 62 >   alt: 0x7 (worse)
	0x17,	// 63 ?
	0x3f,	// 64 @ alt: e
				
	0x5f,	// 65 A
	0x79,	// 66 b 
	0x3c,	// 67 C
	0x73,	// 68 d
	0x3d,	// 69 E
	0x1d,	// 70 F
	0x7c,	// 71 G
	0x5b,	// 72 H
	0x42,	// 73 I 1
	0x62,	// 74 J
	0x5b,	// 75 K
	0x38,	// 76 L
	0x5e,	// 77 M N
	0x5e,	// 78 N
	0x7e,	// 79 O alt: o
	0x1f,	// 80 P
	0x4f,	// 81 Q	
	0x11,	// 82 r
	0x6d,	// 83 S
	0x46,	// 84 T alt: t
	0x7a,	// 85 U V W
	0x7a,	// 86 V
	0x7a,	// 87 W
	0x5b,	// 88 X
	0x6b,	// 89 Y
	0x37,	// 90 Z

	0x3c,	// 91 [
	0x49,	// 92 '\\'
	0x66,	// 93 ]
	0x0e,	// 94 ^ ~
	0x20,	// 95 _
	0x02,	// 96 ` alt: '
	
	0x5f,	// 97 A R
	0x79,	// 98 b 
	0x31,	// 99 c
	0x73,	// 100 d 
	0x3d,	// 101 E
	0x1d,	// 102 F
	0x7c,	// 103 G
	0x59,	// 104 h
	0x42,	// 105 I 1
	0x62,	// 106 J alt 0x60 (worse)
	0x5b,	// 107 K alt h (worse?)
	0x38,	// 108 L
	0x51,	// 109 m n
	0x51,	// 110 n
	0x71,	// 111 o
	0x1f,	// 112 P
	0x4f,	// 113 q	
	0x11,	// 114 r
	0x6d,	// 115 S
	0x39,	// 116 t
	0x70,	// 117 u v w
	0x70,	// 118 v
	0x70,	// 119 w
	0x5b,	// 120 X
	0x6b,	// 121 Y
	0x37,	// 122 Z
	
	0x3c,	// 123 {
	0x18,	// 124 | (left) alt: 1 (worse)
	0x66,	// 125 }
	0x0e,	// 126 ~
	0x00	// 127 "" 127 empty
};

#endif 	// NO_ASCII

///////////////////////////////////////////////////////////////////////////////
//
// Functions
//
///////////////////////////////////////////////////////////////////////////////

//! uncalibrated delay loop
/*! \param ms approximate time in ms
*/
void delay(unsigned ms) {
  struct timespec ts;

  if ( ms >= 1000 ) {

    ts.tv_sec  = ms/1000;
    ts.tv_nsec = (ms - ts.tv_sec * 1000) * 1000000;
  }
  else {

    ts.tv_sec  = 0;
    ts.tv_nsec = ms * 1000000;
  }

  while( -1 == nanosleep( &ts, &ts ) )
    if ( (ts.tv_sec == 0) && (ts.tv_nsec == 0) ) break;
}

//! uncalibrated delay loop
/*! \param ms approximate time in ms
*/
void msleep(unsigned ms) {

  delay( ms );
}

//! display native mode segment mask
/*! \param mask the segment mask.
    \param pos  the desired display position.
    
    this is a dispatcher for the fixed position routines.
*/
void cputc_native(char mask,int pos) {
}			

//! display native mode segment mask at display position 0
/*! \param mask the mask to display
*/
void cputc_native_0(char mask) {
}

//! display native mode segment mask at display position 1
/*! \param mask the mask to display
*/
void cputc_native_1(char mask) {
}

//! display native mode segment mask at display position 2
/*! \param mask the mask to display
*/
void cputc_native_2(char mask) {
}

//! display native mode segment mask at display position 3
/*! \param mask the mask to display
*/
void cputc_native_3(char mask) {
}

//! display native mode segment mask at display position 4
/*! \param mask the mask to display
*/
void cputc_native_4(char mask) {
}

//! display native mode segment mask at display position 5
/*! \param mask the mask to display. only the middle segment is 
           present on the display.
*/
void cputc_native_5(char mask) {
}	

//! display a hexword in the four leftmost positions.
/*! \param word the hexword

    position 0 is unaffected by this call.
*/
void cputw(unsigned word) {

  char buffer[ 512 ];

  sprintf( buffer, "output lcd x %x\n", word );

  write( 1, buffer, strlen( buffer ) );
}

#ifndef NO_ASCII

//! display an ASCIIZ string
/*! \param s the string

    only the first 5 characters will be displayed.
*/
void cputs(char *s) {

  char buffer[ 512 ];

  sprintf( buffer, "output lcd s %c%c%c%c%c", *s, *(s+1), *(s+2), *(s+3), *(s+4) );

  buffer[strlen(buffer)+1] = '\0';
  buffer[strlen(buffer)] = '\n';

  write( 1, buffer, strlen( buffer ) );
}

#if 0
//! scroll a zero terminated string, blocking
/*! \param s the string
*/
void cputs_scroll(char *s) {
	char b4=0,b3=0,b2=0,b1=0,b0=0;
	int wait=0;

	while(*s || wait!=3) {
		b4=b3;
		b3=b2;
		b2=b1;
		b1=b0;
		b0=ascii_display_codes[(*s)];
		
		// double m,w to increase realism.
		if(*s=='w' || *s=='W' || *s=='m' || *s=='M') {
			if((wait++)) {
				wait=0;
				s++;
			}
		} else if(!*s) {		// 0: scroll out.
			b0=0;
			wait++;
		}
		else				// all else: scroll
			s++;
			
		cputc_native_0(b0);
		cputc_native_1(b1);
		cputc_native_2(b2);
		cputc_native_3(b3);
		cputc_native_4(b4);
		
		lcd_refresh();
		if(wait!=3)
			delay(SCROLL_DELAY); 
	} 
			
}
#endif // 0

#endif // NO_ASCII

#endif // NO_CONSOLE_IO
