/*! \file   hello.c
    \brief  rover that turns left or right, depending on obstacle hit
    \author Thomas Roeblitz <roeblitz@informatik.hu-berlin.de>

    prints "Hello world!" on the display
*/

#include "tm.h"
#include "kmain.h"
#include "direct-button.h"
#include "direct-sensor.h"
#include "direct-motor.h"
#include "direct-ir.h"
#include "conio.h"

#include <unistd.h>

pid_t t1, t2;

wakeup_t button_press_wakeup( wakeup_t data ) {

    return PRESSED( button_state(), data );
}

wakeup_t button_release_wakeup( wakeup_t data ) {
        return RELEASED( button_state(), data );
}

wakeup_t sensor_press_wakeup( wakeup_t data ) {
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

        motor_a_speed(MAX_SPEED/2);             // go!
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
                    cputw( sensor );
                    lcd_refresh();

                    sleep(1);
                }
        }
}

int rover_stopper(void) {
        char buffer[ 12 ];
        int r, sum = 0;

        wait_event(&button_release_wakeup,BUTTON_RUN);  // debounce
//        msleep(200);

        wait_event(&button_press_wakeup,BUTTON_RUN);    // wait for RUN button

        sim_kill( t1 );                                       // kill driver

        r = dir_read( buffer, 1 );

        while( (r > 0) && (buffer[sum] != ')') ) {

//          write( 1, buffer+sum, 1 );
          sum++;
          r = dir_read( buffer+sum, 1 );
          if (sum % 5 == 0)
            cputs( buffer + sum - 5 );
        }

        if (sum % 5 != 0)
          cputs( buffer + sum - ((int)sum / 5) * 5 );
        cputs( "retur" );
        return 0;                                       // kernel stops motors
}

int main(void) {

  sim_init();

    t1 = execi( &rover_driver , 1, DEFAULT_STACK_SIZE );
    t2 = execi( &rover_stopper, 3, DEFAULT_STACK_SIZE );

    tm_start();// brace yourself!
    
    return 0;
}

