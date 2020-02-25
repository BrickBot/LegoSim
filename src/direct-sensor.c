/*! \file   direct-sensor.c
    \brief  direct sensor access(Simulator)
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
 *                  Eric Habnerfeller <ehaberfe@atitech.ca>
 *                  Lou Sortman <lou@sunsite.unc.edu>
 */
    
#include "direct-sensor.h"
#include "sys/h8.h"
#include "sys/bitops.h"

#ifndef NO_DIRECT_SENSOR

///////////////////////////////////////////////////////////////////////////////
//
// Definitions
//
///////////////////////////////////////////////////////////////////////////////

#define DS_ALL_ACTIVE         0x07             //!< all sensors active mode
#define DS_ALL_PASSIVE        (~DS_ALL_ACTIVE) //!< all sensors passive mode

//! A/D values for the rotation sensor states
//
#define RANGE_SIZE      (40U<<6)
#define STATE_0_VALUE (1023U<<6)
#define STATE_1_VALUE  (833U<<6)
#define STATE_2_VALUE  (405U<<6)
#define STATE_3_VALUE  (623U<<6)

//! rotation sensor range matching function
//
#define IN_RANGE( val ) (raw > (val - RANGE_SIZE) && \
                         raw < (val + RANGE_SIZE) )


#ifndef NO_ROTATION_SENSOR
//! states for rotation state machine
typedef enum {
  STATE_0=0,
  STATE_1=1,
  STATE_2=2,
  STATE_3=3
} RotationState;
#endif

    
///////////////////////////////////////////////////////////////////////////////
//
// Variables
//
///////////////////////////////////////////////////////////////////////////////

volatile unsigned char ds_channel;        //!< current A/D channel

unsigned char ds_activation;              //!< channel bitmask. 1-> active

#ifndef NO_ROTATION_SENSOR
unsigned char ds_rotation;                //!< channel bitmask. 1-> rotation
 
volatile int ds_rotations[3];             //!< rotation sensor values
static RotationState rotation_state[3];   //!< rotation state machine state
#endif

///////////////////////////////////////////////////////////////////////////////
//
// Functions
//
///////////////////////////////////////////////////////////////////////////////

#ifndef NO_ROTATION_SENSOR
//! set rotation to an absolute value
/*! \param sensor the sensor address, can be &SENSOR_1, &SENSOR_2 or &SENSOR_3
    \param pos    desired absolute position
    
    axis should be inert during the function call
*/
void ds_rotation_set(unsigned* const sensor,int pos) {
  unsigned channel=(unsigned) (sensor-&AD_A);
  unsigned raw = (*sensor);
  RotationState state;

/*
  printf( "output sensor %d rotation %d\n",
          (sensor == SENSOR_1) ? 1 : 
          (sensor == SENSOR_2) ? 2 : 
          (sensor == SENSOR_3) ? 3 : -1, pos );
*/

  if(sensor>=&AD_A && sensor<=&AD_C) {    // catch range violations
    
    if (      raw <= STATE_2_VALUE )      // determine current state
      state=STATE_2;
    else if ( raw <= STATE_3_VALUE )
      state=STATE_3;
    else if ( raw <= STATE_1_VALUE )
      state=STATE_1;
    else
      state=STATE_0;
    
    rotation_state[channel]=state;      
    ds_rotations[channel]=pos;            // reset counter
  }
}


//! process rotation sensor on current A/D channel
/*! \sa ds_channel current channel (global input value)
*/
void ds_rotation_handler() {
  unsigned channel   =ds_channel;
  unsigned raw       =(*((&AD_A)+channel));
  int      cooked    =ds_rotations[channel];
  RotationState state=rotation_state[channel];


  //
  // the rotation sensor has 4 distinct output levels:
  // ~1023 [STATE_0], ~768 [STATE_1], ~357 [STATE_2], ~525 [STATE_3]
  // where ~ signifies +-10
  //
  // finite state machine tracks position
  //

  switch(state) {
    case STATE_0:
     if ( IN_RANGE ( STATE_3_VALUE ) ) {
       state=STATE_3;
       cooked--;
     } else if ( IN_RANGE ( STATE_1_VALUE ) ) {
       state=STATE_1;
       cooked++;
     }
     break;

    case STATE_1:
     if ( IN_RANGE ( STATE_0_VALUE ) ) {
       state=STATE_0;
       cooked--;
     } else if( IN_RANGE ( STATE_2_VALUE ) ) {
       state=STATE_2;
       cooked++;
     }
     break;

    case STATE_2:
     if ( IN_RANGE ( STATE_3_VALUE ) ) {
       state=STATE_3;
       cooked++;
     } else if ( IN_RANGE ( STATE_1_VALUE ) ) {
       state=STATE_1;
       cooked--;
     }
     break;

    case STATE_3:
     if ( IN_RANGE ( STATE_0_VALUE ) ) {
       state=STATE_0;
       cooked++;
     } else if( IN_RANGE ( STATE_2_VALUE ) ) {
       state=STATE_2;
       cooked--;
     }
     break;
   }

   //
   // update data
   //
   rotation_state[channel]=state;
   ds_rotations[channel]=cooked;
}
#endif // NO_ROTATION_SENSOR

//! initialize sensor a/d conversion
/*! all sensors set to passive mode
    rotation tracking disabled
*/
void ds_init(void) {
/*
  int i;
  
  ROM_PORT6_DDR|=DS_ALL_ACTIVE;         // notify ROM we are using
  PORT6_DDR    |=DS_ALL_ACTIVE;         // PORT6 bit 0..2 as outputs
  
  ds_activation=0;                      // all sensors passive
  ds_channel   =0;
  
#ifndef NO_ROTATION_SENSOR
  ds_rotation  =0;                      // rotation tracking disabled
#endif
  
  AD_IRQ =&ds_handler;                  // setup IRQ
  AD_CR &=~ADCR_EXTERN;
  AD_CSR =ADCSR_TIME_266 | ADCSR_GROUP_0 | ADCSR_AN_0  |
          ADCSR_ENABLE_IRQ | ADCSR_START;
  
#ifndef NO_CONSOLE_IO
    delay(10);                         // wait for initial A/D
#else
# warning "Rotation initialization might fail."
#endif
*/
}


//! shutdown sensor a/d conversion
/*! all sensors set to passive mode
*/
void ds_shutdown(void) {
/*
  AD_CSR=0x00;
  PORT6        &=DS_ALL_PASSIVE;
  ROM_PORT6_DDR&=DS_ALL_PASSIVE;
  PORT6_DDR    &=DS_ALL_PASSIVE;
*/
}

//! initializes sensor values
/*!
*/
void sim_direct_sensor_init( void ) {

    ad_a = 0xffff;
    ad_b = 0xffff;
    ad_c = 0xffff;
}

#endif
