/*! \file   tm-and-ir.c
    \brief  A demo for legOS task management and IR driver usage
    \author Markus L. Noga <noga@inrialpes.fr>
    
    Displays incoming IR data at 2400 8N1
    Press view to broadcast "Hello, world!" via IR 
    Press run to stop.
    Any kind of terminal program works well with this.
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
#include "direct-ir.h"
#include "tm.h"
#include "kmain.h"
#include "rom/lcd.h"

///////////////////////////////////////////////////////////////////////////////
//
// Global variables
//
///////////////////////////////////////////////////////////////////////////////

pid_t t1,t2,t3;


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

// view button -> transmit message on IR
//
int view_task(void) {
  char *buffer="Hello, world!\r\n";

  while(1) {
    wait_event(&button_press_wakeup,BUTTON_VIEW);
    dir_write(buffer,15);
    msleep(100);
    wait_event(&button_release_wakeup,BUTTON_VIEW);
  }
}

// show incoming IR bytes
//
int show_task(void) {
  unsigned char *buffer="    ";

  dir_fflush();
  
  while(1) {
      cputs(buffer);
      lcd_refresh();  
      
      buffer[0]=buffer[1];
      buffer[1]=buffer[2];
      buffer[2]=buffer[3];
      
      while(dir_read(&(buffer[3]),1)!=1)
        ;
  }
  return 0;
}

// run button -> stop everything.
//
int run_task(void) {
  wait_event(&button_release_wakeup,BUTTON_RUN);	// debounce
  msleep(100);

  wait_event(&button_press_wakeup,BUTTON_RUN);	// wait for RUN button
  sim_kill(t1);					// kill driver
  sim_kill(t2);
  
  return 0;					// kernel stops motors
}
 
#include "direct-lcd.h"      
// run the tasks
//            
int main(void) {
  sim_init();

  dlcd_show(LCD_S1_SELECT);
  lcd_show(s1_select);
  
    t2=execi(&show_task,1,DEFAULT_STACK_SIZE);
  t1=execi(&view_task,2,DEFAULT_STACK_SIZE);
  t3=execi(&run_task ,3,DEFAULT_STACK_SIZE);
 
  tm_start();

  return 0;
}	
