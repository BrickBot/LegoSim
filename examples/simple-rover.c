/*! \file   simple-rover.c
    \brief  A simple rover demo 
    \author Markus L. Noga <noga@inrialpes.fr>
    
    a simple rover that evades obstacles by turning left
    push run button to stop.
    assumes motors on A,C, touch sensor on port 1
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
#include "unistd.h"
#include "direct-button.h"
#include "direct-sensor.h"
#include "direct-motor.h"
#include "sys/tm.h"

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

wakeup_t sensor_press_wakeup(wakeup_t data) {
#ifndef TRASH
//	return (SENSOR_1%1000)<50;
//	return SENSOR_1<0x2000;
	return SENSOR_1<0xf000;
#else
	return SENSOR_1;
#endif
}

// the rover driver
//

int rover_driver(void) {
	motor_a_speed(MAX_SPEED/2);		// go!
	motor_c_speed(MAX_SPEED/2);
        	
	while(1) {
#ifndef TRASH
		motor_a_dir(fwd);
		motor_c_dir(fwd);

		cputs("fwwd");
		lcd_refresh();
		
		wait_event(&sensor_press_wakeup,0);

		motor_a_dir(rev);
		motor_c_dir(rev);
		
		cputs("rev ");
		lcd_refresh();
		
		sleep(3);
		
		motor_c_dir(fwd);
		
		cputs("left");
		lcd_refresh();
		sleep(3);
#else
		unsigned ret = 1;

		cputs("ok  ");
		lcd_refresh();
		sleep(1);

		ret = (unsigned) wait_event(&sensor_press_wakeup,0);
		sleep(1);

		do {
		  lcd_unsigned(ret % 1000);
		  lcd_refresh();
		  sleep(1);
		  ret /= 1000;
		} while (ret);

#endif
	}
}

int rover_stopper(void) {
	wait_event(&button_release_wakeup,BUTTON_RUN);	// debounce
	msleep(200);
	
	wait_event(&button_press_wakeup,BUTTON_RUN);	// wait for RUN button
	sim_kill(t1);					// kill driver

	return 0;					// kernel stops motors
}

int main(void) {
  sim_init();
	t1=execi(&rover_driver ,1,DEFAULT_STACK_SIZE);
	t2=execi(&rover_stopper,2,DEFAULT_STACK_SIZE);
	
	tm_start();					// brace yourself!
	
	return 0;
}	
