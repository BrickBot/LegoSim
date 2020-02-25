/*! \file   rotation-sensor.c
    \brief  Demo to decode rotation sensors.
    \author Markus L. Noga <noga@inrialpes.fr>
    
    operate rotation sensor on SENSOR_3
    push run button to stop.
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
#include "direct-button.h"
#include "direct-sensor.h"
#include "direct-motor.h"
#include "tm.h"
#include "kmain.h"

pid_t t1,t2;

///////////////////////////////////////////////////////////////////////////////
//
// Functions
//
///////////////////////////////////////////////////////////////////////////////

// button event wakeup functions
//
wakeup_t button_press_wakeup(wakeup_t data) {
  return PRESSED(button_state(),data);
}

wakeup_t button_release_wakeup(wakeup_t data) {
  return RELEASED(button_state(),data);
}

// rotation change event wakeup function
//
wakeup_t rotation_change_wakeup(wakeup_t data) {
  static unsigned rot;
  unsigned *address=(unsigned*) ((unsigned)data); 
  
  if(*address != rot) {
    rot=*address;
    return 1;
  }
  return 0;
}

// the rotation driver
//
int rotation_driver(void) {
  ds_active(&SENSOR_3);
  ds_rotation_set(&SENSOR_3,0);
  ds_rotation_on(&SENSOR_3);
  
  while(1) {
    lcd_int(ROTATION_3);
    lcd_refresh();
    wait_event(&rotation_change_wakeup,(wakeup_t) &ROTATION_3);
  }
}

int rotation_stopper(void) {
  wait_event(&button_release_wakeup,BUTTON_RUN);	// debounce
  msleep(200);

  wait_event(&button_press_wakeup,BUTTON_RUN);	// wait for RUN button
  sim_kill(t1);					// kill driver

  return 0;					// kernel stops motors
}

int main(void) {
  sim_init();

  t1=execi(&rotation_driver ,1,DEFAULT_STACK_SIZE);
  t2=execi(&rotation_stopper,2,DEFAULT_STACK_SIZE);

  cputs("hello");
  lcd_refresh();
  delay(1000);
  tm_start();					// brace yourself!

  return 0;
}	
