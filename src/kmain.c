/*! \file   kmain.c
    \brief  Main kernel loop (Simulator)
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
 *                  Kekoa Proudfoot <kekoa@graphics.stanford.edu>
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <errno.h>

#include "kmain.h"
#include "direct-button.h"
#include "direct-sensor.h"
#include "direct-ir.h"
#include "tm.h"
#include "conio.h"

///////////////////////////////////////////////////////////////////////////////
//
// Variables
//
///////////////////////////////////////////////////////////////////////////////

//! firmware recognition string.
/*! the ROM checks for this string when validating new firmware
*/
unsigned char *firmware_string="Do you byte, when I knock?";


//// pthread variables

pthread_t      select_thread;
pthread_attr_t select_attributes;

///////////////////////////////////////////////////////////////////////////////
//
// declarations
//
///////////////////////////////////////////////////////////////////////////////

void parse_input( const char *buffer, int buflen );
void skip_whitespaces( const char *buffer, int *i, int buflen );
void handle_button( const char *buffer, int i, int buflen );
void handle_sensor( const char *buffer, int i, int buflen );


///////////////////////////////////////////////////////////////////////////////
//
// select task (thread)
//
///////////////////////////////////////////////////////////////////////////////

//! the thread that observes stdin and sets system variables

void observer( void *params ) {

    char c;
    int i;
    int leave_reading, read_state;
    int result;
    int length, read_length, read_i;

    char bounded_buffer[ 1025 ]; // 1024 + 1 for the '\0'
    char *unbounded_buffer;

    process_data *pd;

    // start indefinite select observing stdin only

    while( 1 ) {

    	// read lines from stdin and set system variables

#ifdef DEBUG
        write( 2, ">", 1 );
#endif

        leave_reading = 0;
        i = 0;
        read_i = 0;

        while( !leave_reading && (i < 1024) &&
               ((read_state = read( 0, bounded_buffer+i, 1 )) > 0) ) {

#ifdef DEBUG
            write( 2, bounded_buffer + i, 1 );
#endif

            if ( bounded_buffer[ i ] == 'i' ) {

                // read an i; can be beginning of input, ir or something else
                read_i = 1;
            }
            else if ( (read_i == 1) && (bounded_buffer[ i ] == 'r') ) {

                int j;

                // 1. read buffer length
                // 2. lock m_buffer_filled
                // 3. start writing at dir_rx_write, be aware of dir_rx_end
                // 4. read content
                // 5. if somebody is waiting (check read_pending),
                //    signal condition (m_buffer_filled)
                // 6. unlock m_buffer_filled

                // read length (1 whitespace + x digits until next whitespace)

                i = 0;
                read( 0, bounded_buffer+i, 1 );

                read_length = 0;
                while( (read_length != 1) &&
                       (read( 0, bounded_buffer+i, 1 ) > 0) ) {

                  if ( bounded_buffer[ i ] == ' ' ) {

                    read_length = 1;
                  }
                  i++;
                }

                length = atoi( bounded_buffer );

                // lock m_buffer_filled
                if ( pthread_mutex_lock( &m_buffer_filled ) != 0 ) {

                  perror( "pthread_mutex_lock" );
                  exit( 1 );
                }

                // read_content
                for( j= 0; j < length; j++ ) {

                  read( 0, dir_rx_write++, 1 );
                  if (dir_rx_write == dir_rx_end)
                    dir_rx_write = dir_rx_buf;
                }

                // read line end ('\n')

                read( 0, &c, 1 );

                // if somebody is waiting (check read_pending),
                // signal condition (m_buffer_filled)

                if ( read_pending == 1 ) {

                  if ( pthread_cond_signal( &c_buffer_filled ) != 0 ) {

                    perror( "pthread_cond_signal" );
                    exit( 1 );
                  }
                }

                // unlock m_buffer_filled
                if ( pthread_mutex_unlock( &m_buffer_filled ) != 0 ) {

                  perror( "pthread_mutex_unlock" );
                  exit( 1 );
                }

                // we do not leave the loop in that case, because there
                // is nothing to parse anymore
                read_i  = 0;
                i = -1;
            }
            else if ( bounded_buffer[ i ] == '\n' ) {

                // check if more expected (msg including '\n')

                bounded_buffer[ i + 1 ] = '\0';
                leave_reading = 1;
                read_i = 0;
            }
            else {

                read_i = 0;
            }

            i++;
	}

        if (read_state <= 0) {
          sleep( 2 );
          exit( 1 );
        }

        // parse input

#ifdef DEBUG
        fprintf( stderr, "parse input: i=%d\n", i );
#endif

          parse_input( bounded_buffer, i );

          delay( 250 );

          // check wakeup function(s)

          if ( pthread_mutex_lock( &cpid_mutex ) != 0 ) {
              perror( "observer: cpid_mutex lock" );
              exit( 1 );
          }

          pd = pd_first;

          while( pd != NULL ) {

            if ( pd->wakeup != NULL ) {

                result = pd->wakeup( pd->wakeup_data );

                if ( result != 0 ) {

                    pd->wakeup_data = result;

                    // unblock thread; signal condvar

                    pthread_cond_signal( &pd->condvar );
                }
            }

            pd = pd->next;
        }

        if ( pthread_mutex_unlock( &cpid_mutex ) != 0 ) {
            perror( "observer: cpid_mutex unlock" );
            exit( 1 );
        }
    }
}

void parse_input( const char *buffer, int buflen ) {

    int i, offset;
    char *line;

    line = (char *) calloc( buflen, sizeof( char ) );

    if ( line == NULL ) {

        perror( "calloc" );
        return;
    }

    // read input, if given; if output -> return
    // wrong place here? i think it should be done
    // before this function is called

    for( i = 0; (buffer[ i ] != '\0') && (i < buflen); i++ ) {

        line[ i ] = buffer[ i ];

        if ( isspace( buffer[ i ] ) ) {

            line[ i ] = '\0';

            if ( strcasecmp( line, "input" ) == 0 ) {

                offset = i + 1;
                break;
            } 
            else if ( strcasecmp( line, "output" ) == 0 ) {

                return;
            }
            else {

                offset = 0;
            }
        }
    }

    // get keyword
    // implemeted: button

    for( i = offset; buffer[ i ] != '\0'; i++ ) {

        line[ i ] = buffer[ i ];

        if ( isspace( buffer[ i ] ) ) {

            line[ i ] = '\0';

            if ( strcasecmp( line + offset, "button" ) == 0 ) {

                skip_whitespaces( buffer, &i, buflen );
                handle_button( buffer, i, buflen );
            }
            else if ( strcasecmp( line + offset, "sensor" ) == 0 ) {

                skip_whitespaces( buffer, &i, buflen );
                handle_sensor( buffer, i, buflen );
            }
            else if ( strcasecmp( line + offset, "exit" ) == 0 ) {
                sleep( 2 );
	        exit(0);
            }

            break;
        }
    }
}

void skip_whitespaces( const char *buffer, int *i, int buflen ) {

    int j;

    for( j = *i; j < buflen; j++ ) {

        if ( !isspace( buffer[ j ] ) ) {

            *i = j;
            break;
        }
    }
}

void handle_button( const char *buffer, int i, int buflen ) {

    // get button number (we assume that the number or name is the first item
    // at 'buffer + i')

    char button[ 32 ]; // limited by length of button names
    char action[ 32 ]; // limited by length of button actions

    int j, k;
    int action_i;

    for( j = i; j < buflen; j++ ) {

        button[ j - i ] = buffer[ j ];

        if ( isspace( buffer[ j ] ) ) {

            button[ j - i ] = '\0';
            break;
        }
    }

    skip_whitespaces( buffer, &j, buflen );

    for( k = j; k < buflen; k++ ) {

        action[ k - j ] = buffer[ k ];

        if ( isspace( buffer[ k ] ) ) {

            action[ k - j ] = '\0';
            break;
        }
    }

    // get action

    if ( strcasecmp( action, "pressed" ) == 0 ) {

        action_i = SIM_PRESSED;
    }
    else if ( strcasecmp( action, "released" ) == 0 ) {

        action_i = SIM_RELEASED;
    }
    else {

        // unknown action
        return;
    }

    // get button

    if ( (button[ 0 ] == '1') || (strcasecmp( button, "view" ) == 0) ) {

        if ( action_i == SIM_PRESSED ) {

            sim_button_state = VIEW_PRESSED;
        }
        else {

            sim_button_state = VIEW_RELEASED;
        }
    }
    else if ( (button[ 0 ] == '2') || (strcasecmp( button, "onoff" ) == 0) ) {

        if ( action_i == SIM_PRESSED ) {

            sim_button_state = ONOFF_PRESSED;
        }
        else {

            sim_button_state = ONOFF_RELEASED;
        }
    }
    else if ( (button[ 0 ] == '3') || (strcasecmp( button, "prgm" ) == 0) ) {

        if ( action_i == SIM_PRESSED ) {

            sim_button_state = PRGM_PRESSED;
        }
        else {

            sim_button_state = PRGM_RELEASED;
        }
    }
    else if ( (button[ 0 ] == '4') || (strcasecmp( button, "run" ) == 0) ) {

        if ( action_i == SIM_PRESSED ) {

            sim_button_state = RUN_PRESSED;
        }
        else {

            sim_button_state = RUN_RELEASED;
        }
    }
    else {

        // unknown button
        return;
    }
}

void handle_sensor( const char *buffer, int i, int buflen ) {

    // get sensor number (we assume that the number is the first item
    // at 'buffer + i')

    char sensor[ 32 ]; // limited by length of sensor names
    char  value[ 32 ]; // limited by length of sensor values

    int j, k;
    int value_i;

    for( j = i; j < buflen; j++ ) {

        sensor[ j - i ] = buffer[ j ];

        if ( isspace( buffer[ j ] ) ) {

            sensor[ j - i ] = '\0';
            break;
        }
    }

    skip_whitespaces( buffer, &j, buflen );

    for( k = j; k < buflen; k++ ) {

        value[ k - j ] = buffer[ k ];

        if ( isspace( buffer[ k ] ) ) {

            value[ k - j ] = '\0';
            break;
        }
    }

    // get value

    if ( ( strlen( value ) > 0 ) && ( isdigit( value[ 0 ] ) ) ) {

        value_i = atoi( value );
    }
    else {

        return;
    }

    // get sensor

    if ( sensor[ 0 ] == '1' ) {

        ad_c = value_i;
    }
    else if ( sensor[ 0 ] == '2' ) {

        ad_b = value_i;
    }
    else if ( sensor[ 0 ] == '3' ) {

        ad_a = value_i;
    }
    else {

        // unknown sensor
        return;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// Functions
//
///////////////////////////////////////////////////////////////////////////////

//! the beginning of everything
/*! called by main (must be explizitly called)
*/
void sim_init(void) {

    /*
     * start select task (observing STDIN & setting system variables)
     */

#ifdef MALLOC_NP
    pthread_init();
#endif

    tm_init();

    sim_direct_sensor_init();

    dir_init();

    pthread_attr_init( &select_attributes );

    if ( pthread_create(
           &select_thread,
           &select_attributes,
           (void *(*) (void *)) observer,
           NULL ) != 0 ) {

        perror( "sim_init: pthread_create" );
        switch (errno) {
          case EAGAIN: fprintf( stderr, "errno=EAGAIN\n" );
                       break;
          case EINVAL: fprintf( stderr, "errno=EINVAL\n" );
                       break;
          case ENOMEM: fprintf( stderr, "errno=ENOMEM\n" );
                       break;
          default: fprintf( stderr, "errno=%d\n", errno );
        }

        exit( 1 );
    }

    // init of condition var and mutex in ir component
    if ( pthread_mutex_init( &m_buffer_filled, NULL ) != 0 ) {

        perror( "pthread_mutex_init" );
        exit( 1 );
    }

    if ( pthread_cond_init( &c_buffer_filled, NULL ) != 0 ) {

        perror( "pthread_cond_init" );
        exit( 1 );
    }
}

