/*! \file   include/tm.h
    \brief  Header file for task management(Simulator).
    \author Markus L. Noga <noga@inrialpes.fr>, Thomas Roeblitz <roeblitz@informatik.hu-berlin.de>
    
    Defines types and flags used in task management.
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
    
#ifndef __sim_tm_h__
#define __sim_tm_h__

#include "mem.h"
#include "pthread.h"

///////////////////////////////////////////////////////////////////////////////
//
// Definitions
//
///////////////////////////////////////////////////////////////////////////////

typedef unsigned char pstate_t;			//!< process state type
typedef unsigned char priority_t;		//!< process priority type
typedef unsigned long wakeup_t;			//!< wakeup data area type

//
// process states
//

#define P_DEAD		0			//!< dead and gone, stack freed
#define	P_ZOMBIE	1			//!< terminated, cleanup pending
#define P_WAITING	2			//!< waiting for an event
#define P_SLEEPING	3			//!< sleeping. wants to run.
#define P_RUNNING	4			//!< running


#define DEFAULT_STACK_SIZE	1024		//!< 1k stack has to suffice.


//! start function of thread; used by wrapper
/*
*/
typedef int (*execi_func)(void);

//! process data structure
/*
*/
struct _process_data {
//	size_t	*sp_save;			//!< saved stack pointer
	
	pstate_t pstate;			//!< process state
	priority_t priority;                    //!< process priority
		
	struct _process_data *next;		//!< next process in queue
	struct _process_data *prev;		//!< previous process in queue

	struct _process_data *parent;		//!< parent process

//	size_t *stack_base;			//!< lower stack boundary
	
	wakeup_t (*wakeup)(wakeup_t);		//!< event wakeup function
	wakeup_t wakeup_data;			//!< user data for wakeup fn

        pthread_mutex_t condmutex;              //!< mutex for condvar
        pthread_cond_t  condvar;                //!< wait for wakeup

        pthread_t       pthread;                //!< thread indentifier

        execi_func      code_func;		//!< start function of thread
        int	        ret_value;		//!< return value
};

//! process data type
/*! a shorthand
*/
typedef struct _process_data process_data;

//! process id type
/*! In effect, the kernel simply typecasts *process_data to pid_t.
*/
#ifdef TRASH
#ifndef pid_t
typedef size_t  pid_t;
# define pid_t pid_t
#endif
#else
#ifndef __pid_t_defined
typedef size_t  pid_t;
# define __pid_t_defined
#endif
#endif


// to access wait_event function from observer

extern process_data *cpid;                  //!< ptr to current process data

extern process_data *pd_first;              //!< ptr to first process data

extern unsigned nb_tasks;                   //!< number of tasks

extern pthread_mutex_t cpid_mutex;          //!< synchronize access to cpid


void tm_init( void );
void tm_start( void );
pid_t execi(int (*code_start)(void),priority_t priority,size_t stack_size);
wakeup_t wait_event( wakeup_t (*wakeup)( wakeup_t ), wakeup_t data );
void setCPID( void );
void appendPD( process_data *pd );
void *code_start_wrapper( void *arg );
void sim_kill( pid_t pid );

#endif

