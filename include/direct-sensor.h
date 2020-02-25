/*! \file   include/direct-sensor.h
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
 */
    
#ifndef	__sim_direct_sensor_h__
#define __sim_direct_sensor_h__

#include "sys/h8.h"
#include "sys/bitops.h"

#include <stdio.h>

#ifndef NO_DIRECT_SENSOR

///////////////////////////////////////////////////////////////////////////////
//
// Definitions
//
///////////////////////////////////////////////////////////////////////////////

unsigned ad_a;
unsigned ad_b;
unsigned ad_c;

//
// the raw sensors
//
#define SENSOR_1	AD_C          //!< Sensor 1
#define SENSOR_2	AD_B          //!< Sensor 2
#define SENSOR_3	AD_A          //!< Sensor 3
#define BATTERY		AD_D          //!< Battery sensor

//
// active light sensor: estimated raw values
//
#define LIGHT_RAW_BLACK 0xeb00        //!< active light sensor raw black value
#define LIGHT_RAW_WHITE 0xba00        //!< active light sensor raw white value

//
// convert raw values to 0 (dark) .. LIGHT_MAX (bright)
// roughly 0-100.
//
#define LIGHT(a)    ((LIGHT_RAW_BLACK - (a))>>7)  //!< map light sensor to 0..LIGHT_MAX
#define LIGHT_MAX   LIGHT(LIGHT_RAW_WHITE)        //!< maximum decoded value

//
// processed active light sensor
//
#define LIGHT_1     LIGHT(SENSOR_1)
#define LIGHT_2     LIGHT(SENSOR_2)
#define LIGHT_3     LIGHT(SENSOR_3)

#ifndef NO_ROTATION_SENSOR
//
// processed rotation sensor
//
#define ROTATION_1  (ds_rotations[2])
#define ROTATION_2  (ds_rotations[1])
#define ROTATION_3  (ds_rotations[0])
#endif

#define ds_scale(x)   (x>>6)
#define ds_unscale(x) (x<<6)

///////////////////////////////////////////////////////////////////////////////
//
// Variables
//
///////////////////////////////////////////////////////////////////////////////

//
// don't manipulate directly unless you know what you're doing!
//

extern unsigned char ds_activation;      //!< activation bitmask

#ifndef NO_ROTATION_SENSOR
extern unsigned char ds_rotation;        //!< rotation   bitmask
extern volatile int  ds_rotations[3];    //!< rotational position
#endif


///////////////////////////////////////////////////////////////////////////////
//
// Functions
//
///////////////////////////////////////////////////////////////////////////////

void sim_direct_sensor_init( void );

//
// set sensor mode to active (light sensor emits light, rotation works)
// sensor: &SENSOR_1,&SENSOR_2,&SENSOR_3
//
extern inline void ds_active(unsigned* const sensor) {
  if(sensor==&SENSOR_3) {
    printf( "output sensor 3 active\n" );
    bit_set(&ds_activation,0);
  }
  else if(sensor==&SENSOR_2) {
    printf( "output sensor 2 active\n" );
    bit_set(&ds_activation,1);
  }
  else if(sensor==&SENSOR_1) {
    printf( "output sensor 1 active\n" );
    bit_set(&ds_activation,2);
  }
}


//
// set sensor mode to passive (light sensor detects ambient light)
// sensor: &SENSOR_1,&SENSOR_2,&SENSOR_3
//
extern inline void ds_passive(unsigned* const sensor) {
  if(sensor==&SENSOR_3) {
    printf( "output sensor 3 passive\n" );
    bit_clear( (&ds_activation), 0 );
  }
  else if(sensor==&SENSOR_2) {
    printf( "output sensor 2 passive\n" );
    bit_clear(&ds_activation,1);
  }
  else if(sensor==&SENSOR_1) {
    printf( "output sensor 1 passive\n" );
    bit_clear(&ds_activation,2);
  }
}


#ifndef NO_ROTATION_SENSOR
//
// set rotation to an absolute value
// axis should be inert during the function call
// sensor: &SENSOR_1,&SENSOR_2,&SENSOR_3
//
extern void ds_rotation_set(unsigned* const sensor,int pos);


//
// start tracking rotation sensor
// sensor: &SENSOR_1,&SENSOR_2,&SENSOR_3
//
extern inline void ds_rotation_on(unsigned* const sensor) {
  if(sensor==&SENSOR_3) {
/*    printf( "output sensor 3 rotation on\n" );*/
    bit_set(&ds_rotation,0);
  }
  else if(sensor==&SENSOR_2) {
/*    printf( "output sensor 2 rotation on\n" );*/
    bit_set(&ds_rotation,1);
  }
  else if(sensor==&SENSOR_1) {
/*    printf( "output sensor 1 rotation on\n" );*/
    bit_set(&ds_rotation,2);
  }
}


//
// stop tracking rotation sensor
// sensor: &SENSOR_1,&SENSOR_2,&SENSOR_3
//
extern inline void ds_rotation_off(unsigned* const sensor) {
  if(sensor==&SENSOR_3)
    bit_clear(&ds_rotation,0);
  else if(sensor==&SENSOR_2)
    bit_clear(&ds_rotation,1);
  else if(sensor==&SENSOR_1)
    bit_clear(&ds_rotation,2);
}
#endif // NO_ROTATION_SENSOR

#endif // NO_DIRECT_SENSOR 

#endif
