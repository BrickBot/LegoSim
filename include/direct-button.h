/*! \file   include/direct-button.h
    \brief  query button states (Simulator)
    \author Thomas Roeblitz <roeblitz@informatik.hu-berlin.de>
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

#ifndef __sim_direct_button_h__
#define __sim_direct_button_h__

#define SET_BUTTON_STATE

extern int sim_button_state;

///////////////////////////////////////////////////////////////////////////////
//
// Definitions
//
///////////////////////////////////////////////////////////////////////////////

//
// the button bitmasks
// buttons at @0xb7:8 and @0xbe:8
//

#define	BUTTON_ONOFF	0x0002		// 0x02 @ 0xffb7
#define BUTTON_RUN	0x0004		// 0x04 @ 0xffb7
#define BUTTON_VIEW	0x4000		// 0x40 @ 0xffbe
#define BUTTON_PROGRAM	0x8000		// 0x80 @ 0xffbe

#define SIM_PRESSED  0
#define SIM_RELEASED 1

#define ONOFF_RELEASED 0x0002
#define ONOFF_PRESSED  0x0001
#define RUN_RELEASED   0x0004
#define RUN_PRESSED    0x0008
#define VIEW_RELEASED  0x4000
#define VIEW_PRESSED   0x1000
#define PRGM_RELEASED  0x8000
#define PRGM_PRESSED   0x2000

//
// buttons are active low
//

//
// true if any of the specified buttons is released
//
#define RELEASED(state,button)	((state) & (button)) 

//
// true if all of the specified buttons are pressed
//
#define PRESSED(state,button)	(!RELEASED(state,button))


///////////////////////////////////////////////////////////////////////////////
//
// Functions
//
///////////////////////////////////////////////////////////////////////////////

//
// get button states
// masks defined above
//
extern inline int button_state(void) {

        return sim_button_state;
}


#endif
