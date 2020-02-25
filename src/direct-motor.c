/*! \file   direct-motor.c
    \brief  direct motor access (Simulator)
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
    
#include "direct-motor.h"
#include "sys/h8.h"

#ifndef NO_DIRECT_MOTOR
  
///////////////////////////////////////////////////////////////////////////////
//
// Variables
//
///////////////////////////////////////////////////////////////////////////////

//! motor drive patterns
/*! to be indexed with MotorDirections
    \sa MotorDirections
*/
const unsigned char dm_a_pattern[]={0x00,0x80,0x40,0xc0},
		    dm_b_pattern[]={0x00,0x08,0x04,0x0c},
		    dm_c_pattern[]={0x00,0x02,0x01,0x03};

MotorState dm_a,                  //!< motor A state
           dm_b,                  //!< motor B state
           dm_c;                  //!< motor C state

        
///////////////////////////////////////////////////////////////////////////////
//
// Functions
//
///////////////////////////////////////////////////////////////////////////////

//! direct motor output handler
/*! called by system timer in the 16bit timer OCIA irq
*/
extern void dm_handler(void);
		
//! initialize motors
//
void dm_init(void) {
	dm_shutdown();				// shutdown hardware
}


//! shutdown motors
//
void dm_shutdown(void) {
/*	RCX_MOTORS=0x00;			// shutdown hardware
*/        
	motor_a_dir(off);			// initialize driver data
	motor_b_dir(off);
	motor_c_dir(off);
	
	motor_a_speed(MIN_SPEED);
	motor_b_speed(MIN_SPEED);
	motor_c_speed(MIN_SPEED);
}

#endif
