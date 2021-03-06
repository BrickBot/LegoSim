/*! \file   include/rom/lcd.h
    \brief  ROM LCD control (Simulator)
    \author Markus L. Noga <noga@inrialpes.fr>, Thomas Roeblitz <roeblitz@informatik.hu-berlin.de>
    
    \warning Display updates are realized exclusively by lcd_refresh()
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

#ifndef __sim_rom_lcd_h__
#define __sim_rom_lcd_h__

#include <string.h>
#include <stdio.h>
#include </usr/include/unistd.h>

///////////////////////////////////////////////////////////////////////////////
//
// Definitions
//
///////////////////////////////////////////////////////////////////////////////

//! LCD segment codes
/*! these are not to be confused with the codes defined in direct-lcd.h
    
    circle and dot codes cycle. cycle state is preserved on powerdown.

    each dot code should be invoked six times before using the other.
    mixing them will result in strange behaviour.

*/
typedef enum {
	man_stand	=0x3006,
	man_run		=0x3007,
			
	s1_select	=0x3008,
	s1_active	=0x3009,
			
	s2_select	=0x300A,
	s2_active	=0x300B,
	
	s3_select	=0x300C,
	s3_active	=0x300D,

	a_select	=0x300E,
	a_left		=0x300F,
	a_right		=0x3010,
	
	b_select	=0x3011,
	b_left		=0x3012,
	b_right		=0x3013,
	
	c_select	=0x3014,
	c_left		=0x3015,
	c_right		=0x3016,

	unknown_1	=0x3017, //!< seemingly without effect. cycle reset?

	circle		=0x3018, //!< 0..3 quarters: add one. 4 quarters: reset

	dot		=0x3019, //!< 0..4 dots: add a dot. 5 dots: reset
				 // dots are added on the right.
	dot_inv		=0x301A, //!< 0 dots: show 5. 1..4 dots: subtract one
				 // dots are removed on the left

	battery_x	=0x301B,
	
	//! the IR display values are mutually exclusive.
	
	ir_half		=0x301C,
	ir_full		=0x301D,
	
	everything	=0x3020
	
} lcd_segment;


//! LCD number display styles
/*! note: signed and unsigned are taken by the C programming language
*/
typedef enum {
	digit		=0x3017,	//!< single digit on the right
					// works only with comma_style digit
			
	sign		=0x3001,	//!< signed, no leading zeros
	unsign		=0x301F		//!< unsigned, 0 displayed as 0000
			
} lcd_number_style;


//! LCD comma display styles
/*
*/
typedef enum {
	digit_comma	=0x0000,	//!< single digit on the right
					// works only with number_style digit
			
	e0		=0x3002,	//!< whole
	e_1		=0x3003,	//!< 10ths
	e_2		=0x3004,	//!< 100ths
	e_3		=0x3005,	//!< 1000ths, problematic with negatives
			
} lcd_comma_style;			// lcd display comma style

	
//! display an integer in decimal
#define lcd_int(i)	lcd_number(i,sign,e0)
//! display an unsigned value in decimal
#define lcd_unsigned(u)	lcd_number(u,unsign,e0)
//! display a clock.
/*! passing an argument of 1015 will display 10.15
*/
#define lcd_clock(t)	lcd_number(t,unsign,e_2)
//! display a single digit right of the man symbol
#define lcd_digit(d)	lcd_number(d,digit,digit_comma)


///////////////////////////////////////////////////////////////////////////////
//
// Functions
//
///////////////////////////////////////////////////////////////////////////////

// g++ warns about __asm__ __volatile__. we define the problem away.
// the CXX symbol is predefined in the Makefile
//
#ifdef CXX
# define ASMVOLATILE
#else
# define ASMVOLATILE __volatile__
#endif

//! show LCD segment
/*! \param segment segment to show
*/
extern inline void lcd_show(lcd_segment segment) {

  char buffer[ 512 ];

  // can be augmented by printing the corresponding name
  sprintf( buffer, "output lcd show %d\n", segment );

  write( 1, buffer, strlen( buffer ) );
}

//! hide LCD segment
/*! \param segment segment to hide
*/
extern inline void lcd_hide(lcd_segment segment) {

  char buffer[ 512 ];

  // can be augmented by printing the corresponding name
  sprintf( buffer, "output lcd hide %d\n", segment );

  write( 1, buffer, strlen( buffer ) );
}

//! show number on LCD display
/*! \param i the number
    \param n a number style
    \param c a comma style
*/    
extern void lcd_number(int i,lcd_number_style n,lcd_comma_style c  );

//! clear LCD display
extern inline void lcd_clear(void) {

  char buffer[ 512 ];

  sprintf( buffer, "output lcd clear\n" );

  write( 1, buffer, strlen( buffer ) );
}

//! show LCD display contents to the world
/*! display updates are realized exclusively by calling this function.
*/
extern inline void lcd_refresh(void) {
/***
  char buffer[ 512 ];

  sprintf( buffer, "output lcd refresh\n" );

  write( 1, buffer, strlen( buffer ) );
***/
/*  fflush( stdout ); */
}


#endif
