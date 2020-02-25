/*! \file   light-sensor.c
    \brief  A demo to decode a light sensor on SENSOR_3.
    \author Markus L. Noga <noga@inrialpes.fr>
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

#include "tm.h"
#include "kmain.h"
#include "direct-button.h"
#include "direct-sensor.h"
#include "direct-ir.h"
#include "conio.h"

pid_t t1,t2;

///////////////////////////////////////////////////////////////////////////////
//
// Functions
//
///////////////////////////////////////////////////////////////////////////////

// event wakeup functions

wakeup_t button_press_wakeup(wakeup_t data) {
	return PRESSED(button_state(),data);
}

wakeup_t button_release_wakeup(wakeup_t data) {
	return RELEASED(button_state(),data);
}

// the light driver
//
int light_driver(void) {
  
  ds_active(&SENSOR_3);                     // set light to active
  
  while(1) {
    lcd_int(LIGHT_3);
    lcd_refresh();
/*    msleep(1000); */
    sleep(1);
  }        
}

int light_stopper(void) {
	wait_event(&button_release_wakeup,BUTTON_RUN);	// debounce
	msleep(200);
	
	wait_event(&button_press_wakeup,BUTTON_RUN);	// wait for RUN button
	sim_kill(t1);					// kill driver

	return 0;					// kernel stops motors
}

int main(void) {
  sim_init();

	t1=execi(&light_driver ,1,DEFAULT_STACK_SIZE);
	t2=execi(&light_stopper,2,DEFAULT_STACK_SIZE);
	
	tm_start();					// brace yourself!
	
        // kernel will disactivate light.
        
	return 0;
}	
