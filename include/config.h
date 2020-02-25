/*! \file   include/config.h
    \brief  overall kernel configuration
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

// #define NO_ASCII			//!< no ascii console output
// #define NO_CONSOLE_IO		//!< no console output
// #define NO_MEMORY_MANAGEMENT 	//!< no memory management
// #define NO_TASK_VISUALIZATION	//!< no running man & nr of tasks
					//!< display

// #define NO_SYSTEM_TIME               //!< no system timer
// #define NO_TASK_MANAGEMENT		//!< no task management
#define NO_EQUAL_PRIORITIES		//!< no two tasks have equal
					//!< priorities
					//!< -> faster scheduler
// #define NO_SEMAPHORES		//!< no POSIX semaphores
// #define NO_DIRECT_IR			//!< no direct IR
#define NO_DIRECT_SOUND			//!< no direct sound 
// #define NO_DIRECT_MOTOR		//!< no direct motor output
// #define NO_DIRECT_SENSOR		//!< no direct sensor input
// #define NO_ROTATION_SENSOR           //!< no rotation processing

//
// dependencies
//

#ifdef NO_CONSOLE_IO
# define NO_ASCII
#endif

#ifdef NO_SYSTEM_TIME
# ifndef NO_TASK_MANAGEMENT
#  error "Task management needs system time."
# endif
#endif

#ifdef NO_MEMORY_MANAGEMENT
# ifndef NO_TASK_MANAGEMENT
#   error "Task management needs memory management."
# endif
#endif

#ifndef NO_TASK_MANAGEMENT
# ifdef NO_SEMAPHORES
#  ifndef NO_MEMORY_MANAGEMENT
#    error "Tasksafe memory management needs semaphores."
#  endif
# endif
#endif

#ifdef NO_DIRECT_SENSOR
# define NO_ROTATION_SENSOR
#endif
