/*! \file   include/sys/tm.h
    \brief  task management interna
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

#ifndef __sys_tm_h__
#define __sys_tm_h__

#include "config.h"

#ifndef NO_TASK_MANAGEMENT

#include "../tm.h"


///////////////////////////////////////////////////////////////////////////////
//
// Definitions
//
///////////////////////////////////////////////////////////////////////////////

#define SP_RETURN_OFFSET	10	// offset of return address on stack
					// in words.

#define IDLE_STACK_SIZE		128	// should suffice for IRQ service

					
///////////////////////////////////////////////////////////////////////////////
//
// Variables
//
///////////////////////////////////////////////////////////////////////////////

extern process_data pd_single;			// single process process data

extern process_data *cpid;			// ptr to current process data


///////////////////////////////////////////////////////////////////////////////
//
// Functions
//
///////////////////////////////////////////////////////////////////////////////

//
// init task management
// called in single tasking mode before task setup.
//
extern void tm_init(void);


//
// start task management 
// called in single tasking mode after task setup
//
extern void tm_start(void);

	
//
// the task switcher IRQ handler
// located in the assembler process module
//
extern void tm_switcher(void);


//
// the process scheduler
// arg   : current task's current stack pointer
// retval: new task's current stack pointer
//
// actual context switches performed by tm_switcher (assembler wrapper)
//
extern size_t *tm_scheduler(size_t *old_sp);


//
// the idle task
// infinite sleep instruction
//
extern int tm_idle_task(void);


#endif

#endif
