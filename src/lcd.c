/*! \file   lcd.c
    \brief  wrapper for ROM LCD number display functions (Simulator)
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

#include "rom/lcd.h"

///////////////////////////////////////////////////////////////////////////////
//
// Functions
//
///////////////////////////////////////////////////////////////////////////////

//! show number on LCD display
/*! \param i the number
    \param n a number style
    \param c a comma style
*/
void lcd_number(int i,lcd_number_style n,lcd_comma_style c  ) {
  switch(n) {
    case digit :
      if ( c == digit_comma ) {
        printf( "output lcd %d (digit,digit_comma)\n", i % 10 );
      }
      break;
    case sign  :
      switch( c ) {
        case e0:
          printf( "output lcd %d (sign,e0)\n", i );
          break;
        case e_1:
          printf( "output lcd %d.%d (sign,e_1)\n", i / 10, i % 10 );
          break;
        case e_2:
          printf( "output lcd %d.%d (sign,e_2)\n", i / 100, i % 100 );
          break;
        case e_3:
          printf( "output lcd %d.%d (sign,e_3)\n", i / 1000, i % 1000 );
          break;
        default:
      }
      break;
    case unsign:
      switch( c ) {
        case e0:
          printf( "output lcd %d (unsign,e0)\n", i );
          break;
        case e_1:
          printf( "output lcd %d.%d (unsign,e_1)\n", i / 10, i % 10 );
          break;
        case e_2:
          printf( "output lcd %d.%d (unsign,e_2)\n", i / 100, i % 100 );
          break;
        case e_3:
          printf( "output lcd %d.%d (unsign,e_3)\n", i / 1000, i % 1000 );
          break;
        default:
      }
      break;
    default:
  }
}
