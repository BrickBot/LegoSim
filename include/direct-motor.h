/*! \file   include/direct-motor.h
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
    
#ifndef	__sim_direct_motor_h__
#define __sim_direct_motor_h__

//#include "config.h"

#include <unistd.h>
#include <string.h>
#include <stdio.h>

#ifndef NO_DIRECT_MOTOR

///////////////////////////////////////////////////////////////////////////////
//
// Definitions
//
///////////////////////////////////////////////////////////////////////////////

// the RCX-specific motor driver I/O address
/* #define	 RCX_MOTORS	*((unsigned char*) 0xf000) */

//! the motor directions
typedef enum {
	off=0,          //!< freewheel
	fwd=1,          //!< forward
	rev=2,          //!< reverse
	brake=3         //!< hold current position
			
}
	MotorDirection;


//! the motor status type.
typedef struct {
  union {
    unsigned assembler;           //!< assures word alignment for assembler
    struct {
      unsigned char delta;        //!< the speed setting
      volatile unsigned char sum; //!< running sum
    } c;
  } access;                       //!< provides access from C and assembler
  
  unsigned char dir;              //!< output pattern when sum overflows
}
  MotorState;
        

//! minimum motor speed        
#define  MIN_SPEED	0
//! maximum motor speed
#define  MAX_SPEED	255


///////////////////////////////////////////////////////////////////////////////
//
// Variables
//
///////////////////////////////////////////////////////////////////////////////

//! motor drive patterns
/*! to be indexed with MotorDirections
    \sa MotorDirections
*/
extern const unsigned char dm_a_pattern[4],
		           dm_b_pattern[4],
    		           dm_c_pattern[4];

extern MotorState dm_a,                  //!< motor A state
                  dm_b,                  //!< motor B state
                  dm_c;                  //!< motor C state


///////////////////////////////////////////////////////////////////////////////
//
// Functions
//
///////////////////////////////////////////////////////////////////////////////

//! initialize motors
//
void dm_init(void);


//! shutdown motors
//
void dm_shutdown(void);


//! set motor A direction
/*! \param dir the direction
*/
extern const inline void motor_a_dir(MotorDirection dir) {

        char buffer[ 512 ];
	dm_a.dir=dm_a_pattern[dir];

        sprintf( buffer, "output actuator A direction %s\n",
                 (dir == off) ? "off" : ((dir == fwd) ? "fwd" : ((dir == rev) ? "rev" : "brake")) );
        write( 1, buffer, strlen( buffer ) );
}

//! set motor B direction
/*! \param dir the direction
*/
extern const inline void motor_b_dir(MotorDirection dir) {

        char buffer[ 512 ];
	dm_b.dir=dm_b_pattern[dir];

        sprintf( buffer, "output actuator B direction %s\n",
                 (dir == off) ? "off" : ((dir == fwd) ? "fwd" : ((dir == rev) ? "rev" : "brake")) );
        write( 1, buffer, strlen( buffer ) );
}

//! set motor C direction
/*! \param dir the direction
*/
extern const inline void motor_c_dir(MotorDirection dir) {

        char buffer[ 512 ];
	dm_c.dir=dm_c_pattern[dir];

        sprintf( buffer, "output actuator C direction %s\n",
                 (dir == off) ? "off" : ((dir == fwd) ? "fwd" : ((dir == rev) ? "rev" : "brake")) );
        write( 1, buffer, strlen( buffer ) );
}


//! set motor A speed
/*! \param speed the speed
*/
extern const inline void motor_a_speed(unsigned char speed) {

        char buffer[ 512 ];
	dm_a.access.c.delta=speed;

        sprintf( buffer, "output actuator A speed %d\n", speed );
        write( 1, buffer, strlen( buffer ) );
}

//! set motor B speed
/*! \param speed the speed
*/
extern const inline void motor_b_speed(unsigned char speed) {

        char buffer[ 512 ];
	dm_b.access.c.delta=speed;

        sprintf( buffer, "output actuator B speed %d\n", speed );
        write( 1, buffer, strlen( buffer ) );
}

//! set motor C speed
/*! \param speed the speed
*/
extern const inline void motor_c_speed(unsigned char speed) {

        char buffer[ 512 ];
	dm_c.access.c.delta=speed;

        sprintf( buffer, "output actuator C speed %d\n", speed );
        write( 1, buffer, strlen( buffer ) );
}

#endif  // NO_DIRECT_MOTOR

#endif  // __direct_motor_h__
