/*! \file   direct-ir.c
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
 *                  Chris Dearman <chris@algor.co.uk>
 */

#include "direct-ir.h"
#include "sys/h8.h"
#include "sys/irq.h"

#include "direct-lcd.h"
#include "mem.h"
#include "conio.h"

#include <stdlib.h> // for exit

#ifndef NO_DIRECT_IR

#define RX_BUF_SIZE 128

///////////////////////////////////////////////////////////////////////////////
//
// Variables
//
///////////////////////////////////////////////////////////////////////////////

pthread_cond_t  c_buffer_filled;
pthread_mutex_t m_buffer_filled;

int read_pending  = 0;

unsigned char dir_rx_buf[ RX_BUF_SIZE ];
unsigned char *dir_rx_end;
unsigned char *dir_rx_write;
unsigned char *dir_rx_read;

///////////////////////////////////////////////////////////////////////////////
//
// Functions
//
///////////////////////////////////////////////////////////////////////////////

//! C core for the rx byte received interrupt
//
void dir_rx_c(void) {}


//! assembler wrapper for the rx byte received interrupt
//
void dir_rx_handler(void) {}

//! C core for the rx error interrupt
//
void dir_rxerror_c(void) {}


//! assembler wrapper for the rx error interrupt
//
void dir_rxerror_handler(void) {}

//! assembler tx byte sent interrupt
/*! write next byte if there's one left, otherwise unhook irq.
*/
void dir_tx_handler(void) {}

//! assembler tx end interrupt
/*! shutdown transmission
*/
void dir_txend_handler(void) {}

//! initialize IR port
/*!
*/
void dir_init(void) {
  dir_rx_write = dir_rx_buf;
  dir_rx_read  = dir_rx_buf;
  dir_rx_end   = dir_rx_buf+RX_BUF_SIZE;
}


//! shutdown IR port
/*!
*/
void dir_shutdown(void) {
}


//! write to IR port, blocking.
/*! \param buf data to transmit
    \param len number of bytes to transmit
    \return number of bytes written, -1 indicates error.
*/
size_t dir_write(void* const buf,size_t len) {

    char *buffer;
    int i, offset;

    if(buf==NULL || len==0)  // catch empty cases
        return 0;

    buffer = malloc( len + 32 );

    if ( buffer == NULL ) {

      perror( "malloc" );
      return -1;
    }

    sprintf( buffer, "output ir %d ", len );

    offset = strlen( buffer );

    for( i = 0; i < len; i++ ) {

        buffer[ i + offset ] = ((char *)buf)[ i ];
    }

    buffer[ i + offset ]     = '\n';
    buffer[ i + offset + 1 ] = '\0';

    write( 1, buffer, strlen(buffer) );

    free( buffer );

    return len;
}

//! read from IR port, blocking.
/*! \param buf allocated receive buffer
    \param len number of bytes to read
    \return number of bytes read, -1 indicates error.
*/
size_t dir_read(void* buf,size_t len) {
  unsigned char *ptr;
        
  if(buf==NULL || len==0)			// catch empty cases
    return 0;

  if ( pthread_mutex_lock( &m_buffer_filled ) != 0 ) {

    perror( "pthread_mutex_lock" );
    exit( 1 );
  }

  ptr = buf;
  buf +=len;

  while( ptr < (unsigned char*)buf ) {

    while( dir_rx_read == dir_rx_write ) {

      read_pending++;

      if ( pthread_cond_wait( &c_buffer_filled, &m_buffer_filled ) != 0 ) {

        perror( "pthread_cond_wait" );
        exit( 1 );
      }
    }

    *(ptr++)=*(dir_rx_read++);
    if (dir_rx_read == dir_rx_end)
      dir_rx_read = dir_rx_buf;
  }

  read_pending--;

  if ( pthread_mutex_unlock( &m_buffer_filled ) != 0 ) {

    perror( "pthread_mutex_unlock" );
    exit( 1 );
  }

  return len; // successful read
}

//! flush input buffer.
//
void dir_fflush(void) {
  dir_rx_read = dir_rx_write;
}

#endif
