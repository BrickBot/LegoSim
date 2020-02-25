/*! \file   stear.c
    \brief  evades obstacles
    \author Thomas Roeblitz <roeblitz@informatik.hu-berlin.de>
    
    a rover that evades obstacles by turning left or right
    push run button to stop.
    assumes motors on A,C, touch sensor on port 1 and 3
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
 *  Enhanced by the author (see above).
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
        lcd_refresh();
        if ( SENSOR_1 < 0x3000 ) {

	    return 1;
        }
        else if ( SENSOR_3 < 0x3000 ) {

	    return 3;
        }
        else {

	    return 0;
        }
}

// the rover driver sensor
//

int rover_driver(void) {
        int sensor = 0;

	motor_a_speed(MAX_SPEED/2);		// go!
	motor_c_speed(MAX_SPEED/2);
        	
	while(1) {
		motor_a_dir(fwd);
		motor_c_dir(fwd);

		cputs("fwd  ");
		lcd_refresh();
		
		sensor = wait_event(&sensor_press_wakeup,0);

                if ( sensor == 1 ) {

		    motor_a_dir(rev);
    		    motor_c_dir(rev);
		
		    cputs("rev  ");
		    lcd_refresh();
		
		    sleep(2);
		
		    motor_a_dir(fwd);
		
		    cputs("right");
		    lcd_refresh();
		
		    sleep(1);
                }
                else if ( sensor == 3 ) {

		    motor_a_dir(rev);
    		    motor_c_dir(rev);
		
		    cputs("rev ");
		    lcd_refresh();
		
		    sleep(2);
		
		    motor_c_dir(fwd);
		
		    cputs("left");
		    lcd_refresh();
		
		    sleep(1);
                }
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
	t2=execi(&rover_stopper,3,DEFAULT_STACK_SIZE);
	
	tm_start();					// brace yourself!
	
	return 0;
}	
