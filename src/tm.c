/*! \file   tm.c
    \brief  Task management(Simulator)
    \author Markus L. Noga <noga@inrialpes.fr>, Thomas Roeblitz <roeblitz@informatik.hu-berlin.de>
    
    Contains the multitasking switcher and scheduler as
    well as library functions relating to task management.
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
 *                  Ben Laurie <ben@algroup.co.uk>
 */
    
#include <pthread.h>

#ifndef NO_TASK_MANAGEMENT

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "tm.h"

///////////////////////////////////////////////////////////////////////////////
//
// Variables
//
///////////////////////////////////////////////////////////////////////////////

process_data pd_single;				//!< single process process data

process_data *cpid=&pd_single;			//!< ptr to current process data

process_data *pd_first=&pd_single;		//!< ptr to first process data

unsigned nb_tasks;				//!< number of tasks

pthread_mutex_t cpid_mutex;                     //!< synchronize access to cpid

///////////////////////////////////////////////////////////////////////////////
//
// Functions
//
///////////////////////////////////////////////////////////////////////////////

void *code_start_wrapper( void *arg );


void tm_init(void) {

        if ( pthread_mutex_init( &cpid_mutex, NULL ) != 0 ) {

            perror( "tm_init: pthread_mutex_init" );
            exit( 1 );
        }

        if ( pthread_mutex_lock( &cpid_mutex ) != 0 ) {
            perror( "tm_init: cpid_mutex lock" );
            exit( 1 );
        }

	// no tasks right now.
	//
	nb_tasks=0;
	
	// the single tasking context
	//
	pd_single.next=pd_single.prev=NULL;
	pd_single.pstate=P_RUNNING;
	pd_single.priority=255;
	cpid=&pd_single;
        cpid->wakeup  = NULL;
        cpid->pthread = pthread_self();

        if ( pthread_mutex_init( &cpid->condmutex, NULL ) != 0 ) {

            perror( "tm_init: pthread_mutex_init" );
            exit( 1 );
        }

        if ( pthread_cond_init( &cpid->condvar, NULL ) != 0 ) {

            perror( "tm_init: pthread_cond_init" );
            exit( 1 );
        }

        if ( pthread_mutex_unlock( &cpid_mutex ) != 0 ) {
            perror( "tm_init: cpid_mutex unlock" );
            exit( 1 );
        }
}	

//! start task management
/*! called in single tasking mode after task setup; here endless loop only
*/
void tm_start(void) {

    pthread_exit(NULL);
}

//! suspend process until wakeup function is non-null
/*! \param wakeup the wakeup function. called in task scheduler context.
    \param data argument passed to wakeup function by scheduler
    \return return value passed on from wakeup
*/
wakeup_t wait_event(wakeup_t (*wakeup)(wakeup_t),wakeup_t data) {

        // check condition, ie. call wakeup and return if result is non-null

        process_data *pd;

        wakeup_t result = wakeup( data );

        if ( result != 0 ) {

            return result;
        }

        if ( pthread_mutex_lock( &cpid_mutex ) != 0 ) {
            perror( "wait_event: cpid_mutex lock" );
            exit( 1 );
        }

        setCPID();

        pd = cpid;

        if ( pthread_mutex_unlock( &cpid_mutex ) != 0 ) {
            perror( "wait_event: cpid_mutex unlock" );
            exit( 1 );
        }

	pd->wakeup     =wakeup;
	pd->wakeup_data=data;
	pd->pstate     =P_WAITING;

        pthread_cleanup_push( ( void (* )() ) &pthread_mutex_unlock, &pd->condmutex );

        if ( pthread_mutex_lock( &pd->condmutex ) != 0 ) {
            perror( "wait_event: condmutex lock" );
            exit( 1 );
        }

//	while (pd->wakeup != NULL) {
	  if ( pthread_cond_wait( &pd->condvar, &pd->condmutex ) != 0 ) {
            perror( "wait_event: pthread_cond_wait" );
            exit( 1 );
	  }
//	}
	
	pd->wakeup     =NULL;

        if ( pthread_mutex_unlock( &pd->condmutex ) != 0 ) {
            perror( "wait_event: condmutex unlock" );
            exit( 1 );
        }

        pthread_cleanup_pop( 0 );

	return pd->wakeup_data;
}

//! set cpid to process data of current thread
void setCPID( void ) {

    // set cpid; traverse list

    process_data *pd;

    if ( pd_first != NULL ) {

        pd = pd_first;

        while( pd != NULL ) {

            if ( pthread_equal( pd->pthread, pthread_self() ) != 0 ) {

                cpid = pd;
                return;
            }

            pd = pd->next;
        }

        // fatal error, process data not found

        perror( "no process data 1: exit" );
        exit( 3 );
    }
    else {

        // fatal error exit

        perror( "no process data 2: exit" );
        exit( 3 );
    }
}

//! append process data to list
/*! \param pd process data
*/

void appendPD( process_data *pd ) {

    process_data *curpd;

    if ( pd_first != NULL ) {

        curpd = pd_first;

        while( curpd->next != NULL ) {

            curpd = curpd->next;
        }

        curpd->next = pd;
        pd->prev = curpd;
        pd->next = NULL;
    }
    else {

        // fatal error exit

        perror( "no process data 3: exit" );
        exit( 3 );
    }
}

//! execute a memory image.
/*! \param code_start start address of code to execute
    \param priority   new task's priority
    \param stack_size stack size for new process
    \return -1: fail, else pid.

    will return to caller in any case.
*/
pid_t
execi( int (*code_start) ( void ), priority_t priority, size_t stack_size ) {

        process_data *pd;

        // get memory
        //
        // task & stack area belong to parent process
        // they aren't freed by mm_reaper()
        //

#ifdef DEBUG
        fprintf( stderr, "allocate memory for process data\n" );
#endif

        if( (pd = (process_data *) malloc( sizeof( process_data ) )) == NULL )
                return -1;

        if ( pthread_mutex_lock( &cpid_mutex ) != 0 ) {
            perror( "execi: cpid_mutex lock" );
            exit( 1 );
        }

        appendPD( pd );

        setCPID();

        pd->priority = priority;
        pd->pstate = P_SLEEPING;
        pd->parent = cpid;
        pd->code_func = code_start;
        pd->ret_value = 0;

	if ( pthread_mutex_init( &pd->condmutex, NULL ) != 0 ) {
	  perror( "execi: pthread_mutex_init" );
	  exit( 1 );
	}

	if ( pthread_cond_init( &pd->condvar, NULL ) != 0 ) {
	  perror( "execi: pthread_cond_init" );
	  exit( 1 );
	}

        // start thread

#ifdef DEBUG
        fprintf( stderr, "create pthread\n" );
#endif

        if ( 0 != pthread_create( &pd->pthread,
                                  NULL,
                                  (void *(*) (void *)) code_start_wrapper,
                                  NULL ) ) {

            perror( "pthread_create" );

            if ( pthread_mutex_unlock( &cpid_mutex ) != 0 ) {
              perror( "execi: cpid_mutex unlock" );
              exit( 1 );
            }

            return -1;
        }

        if ( pthread_mutex_unlock( &cpid_mutex ) != 0 ) {
            perror( "execi: cpid_mutex unlock" );
            exit( 1 );
        }

        return (pid_t) pd;
}

void *code_start_wrapper( void *arg ) {

    int result;

    process_data *pd;

    if ( pthread_mutex_lock( &cpid_mutex ) != 0 ) {
        perror( "code_start_wrapper: cpid_mutex lock" );
        exit( 1 );
    }

    setCPID();

    pd = cpid;

    if ( pthread_mutex_unlock( &cpid_mutex ) != 0 ) {
        perror( "code_start_wrapper: cpid_mutex unlock" );
        exit( 1 );
    }

    cpid->ret_value = cpid->code_func();

    return NULL;
}

//! kill a process
/*! \param pid must be valid process ID, or undefined behaviour will result!
*/
void sim_kill( pid_t pid ) {

    process_data *pd = (process_data *) pid;

    if ( pd == NULL ) {

        perror( "sim_kill: pid points to 0x0" );
        return;
    }

    if ( pthread_equal( pd->pthread, pthread_self() ) != 0 ) {

        exit( -1 );
    }
    else {

        // call pthread_cancel

        if ( pthread_cancel( pd->pthread ) != 0 ) {

            perror( "sim_kill: pthread_cancel" );
            exit( 1 );
        }

        // thread should have finshed at this point; set appropriate mode for
        // cancellation needed?

        if ( pthread_join( pd->pthread, NULL ) != 0 ) {

            perror( "sim_kill: pthread_join" );
            exit( 1 );
        }

        if ( pthread_mutex_destroy( &pd->condmutex ) != 0 ) {
            perror( "sim_kill: condmutex destroy" );
            exit( 1 );
        }

        if ( pthread_cond_destroy( &pd->condvar ) != 0 ) {
            perror( "sim_kill: condvar destroy" );
            exit( 1 );
        }

        if ( pd->prev != NULL ) {

            pd->prev->next = pd->next;
        }

        if ( pd->next != NULL ) {

            pd->next->prev = pd->prev;
        }

        free( pd );
    }
}

#endif
