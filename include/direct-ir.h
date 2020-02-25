/*! \file   include/direct-ir.h
    \brief  direct IR port access (Simulator)
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
 */

#ifndef __sim_direct_ir_h__
#define __sim_direct_ir_h__

#include "config.h"

#ifndef NO_DIRECT_IR

#include "mem.h"

#include <pthread.h>

extern unsigned char *dir_rx_write;
extern unsigned char *dir_rx_end;
extern unsigned char dir_rx_buf[];

///////////////////////////////////////////////////////////////////////////////
//
// Functions
//
///////////////////////////////////////////////////////////////////////////////

extern void dir_init(void);

//
// write to IR port, blocking.
// return code: nr of bytes written. -1 -> error.
//
extern size_t dir_write(void* const buf,size_t len);


//
// read from IR port, blocking.
// return code: nr of bytes written. -1 -> error.
//
extern size_t dir_read(void* buf,size_t len);

//
// flush input buffer.
//
extern void dir_fflush(void);


extern pthread_cond_t  c_buffer_filled;
extern pthread_mutex_t m_buffer_filled;
extern int read_pending;
extern int buffer_filled;
extern char *read_buffer;
extern char *read_buffer_end;
extern char *read_buffer_begin;

#endif	// NO_DIRECT_IR

#endif
