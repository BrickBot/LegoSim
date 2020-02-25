/*! \file   InServer.java
    \brief  I/O threads
    \author Oliver Buehn <buehn@informatik.hi-berlin.de>
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
 *  The Original Code is LegoSim code, released October 29, 1999.
 *
 *  The Initial Developer of the Original Code is Markus L. Noga.
 *  Portions created by Frank Mueller are Copyright (C) 1999
 *  Frank Mueller. All Rights Reserved.
 *
 *  Contributor(s):
 */

/* This thread permanently reads the stdin and performs related
   actions as setting the motor speed (only by showing a string in
   the motor field of the rcx-image), putting text into the ir-field
   and so on. (Not to forget: this is a part of the rcx-simulator which
   simulates things done to an imaginary LEGO rcx-module.)
*/

import java.io.*;
import java.lang.*;
import java.util.*;

class InServer extends Thread {
  private legosim applet;

  public InServer(legosim applet) {
    this.applet = applet;
  }

  public void run() {
    String            input_string    = "";
    String            lcd_string      = "";
    String            output_string   = ""; //
    String            actuator_status = "";
    String            actuator_value  = "";
    int               i, lcd_num      = 0;
    int               input           = 0;
    int               ir_msg_lngth    = 0;
    int               dev_int_number;
    char              dev_char_number;
    char              ir_msg[]        = new char[65535];
    char              lcd_char;
    InputStreamReader inpt_strm_rdr   = new InputStreamReader(System.in);
    BufferedReader    bffrd_rdr       = new BufferedReader(inpt_strm_rdr);
    StreamTokenizer   strm_tknzr      = new StreamTokenizer(bffrd_rdr);

    strm_tknzr.eolIsSignificant (true);  // otherwise EOL would be like ' '.


    do {
      input_string = "";
      input = get_next_token(strm_tknzr);
      if (input != strm_tknzr.TT_WORD) {
	input = 0;
//	break; // exit the do...while loop.
      } // if
      if (input != 0)
	input_string = strm_tknzr.sval;

      // OUTPUT >>>
      if (input_string.equals("output")) {
	input = get_next_token(strm_tknzr);
	input_string = strm_tknzr.sval;

	// > ACTUATOR >>
	if (input_string.equals("actuator")) {
	  input = get_next_token(strm_tknzr);
	  dev_char_number = (strm_tknzr.sval).charAt(0);
	  input = get_next_token(strm_tknzr);
	  actuator_status = strm_tknzr.sval;
	  input = get_next_token(strm_tknzr);
	  if (actuator_status.equals("speed"))
	    actuator_value  = Double.toString(strm_tknzr.nval);
	  else if (actuator_status.equals("direction"))
	    actuator_value  = (strm_tknzr.sval);	    
	  else
	    dev_char_number = ' ';

	  switch (dev_char_number) {
	  case 'A':
	    applet.actuatorA_stat.setText(actuator_status);
	    applet.actuatorA.setText(actuator_value);
	    break;
	  case 'B':
	    applet.actuatorB_stat.setText(actuator_status);
	    applet.actuatorB.setText(actuator_value);
	    break;
	  case 'C':
	    applet.actuatorC_stat.setText(actuator_status);
	    applet.actuatorC.setText(actuator_value);
	    break;
	  default:
	    break;
	  } // switch (dev_char_number)
	applet.showStatus("actuator action");
	} // end if ("actuator")

	// LCD >>>>
	if (input_string.equals("lcd")) {
	  input = get_next_token(strm_tknzr);

	  if (input == strm_tknzr.TT_WORD) {
	    if ((strm_tknzr.sval).equals("s")) {
	      try {
		while ((lcd_char = (char) bffrd_rdr.read()) != '\n')
		  output_string = output_string + lcd_char;
	      }
	      catch (IOException ioe) {
		System.out.println(ioe.getMessage());
	      }
	    } // if "s"
	    // hexadecimal input:
	    if ((strm_tknzr.sval).equals("x")) {
	      try {
		while ((lcd_char = (char) bffrd_rdr.read()) != '\n')
		  output_string = output_string + lcd_char;
	      }
	      catch (IOException ioe) {
		System.out.println(ioe.getMessage());
	      }
	      try {
		lcd_num = ((Integer.decode(output_string)).intValue());
	      }
	      catch (NumberFormatException nfe) {
		System.err.println ("NumberFormatException caught: " +
				    "Expression " + nfe.getMessage() +
				    " is invalid.");
	      }
	      applet.in_out_control_area.append (output_string + '\n');
	      output_string = Integer.toString(lcd_num);
	    }

	    // there're only 5 digits on the lcd:
	    for (i = 0; i < 5 & i < output_string.length(); i++) {
	      if (i == 4)  // the tiny guy sits on digit 4.
		lcd_string = lcd_string + ' ';  // skip the tiny guy.
	      lcd_string = lcd_string + output_string.charAt(i);
	    } //for
	    
	    applet.lcd_text = lcd_string;
	    applet.repaint();
	    output_string = "";
	    lcd_string = "";

	  }  // if TT.WORD

	  applet.showStatus("lcd setting");
	} // end if ("lcd")

	// IR >>>>
	if (input_string.equals("ir")) {
	  input = get_next_token(strm_tknzr);
	  ir_msg_lngth = (int) strm_tknzr.nval;
	  try {
	    ir_msg_lngth = bffrd_rdr.read(ir_msg, 0, ir_msg_lngth);
	  }
	  catch (IOException ioe) {
	    System.err.println ("wrong message length?");
	  }
	  applet.in_out_control_area.append (" "); // between msg_lngth & str.

	  for (i = 0; i <= ir_msg_lngth; i++) {
	    applet.ir_area.append (String.valueOf (ir_msg[i]));
	    applet.in_out_control_area.append (String.valueOf (ir_msg[i]));
	  }

/*	  while (input != strm_tknzr.TT_EOL) {
	    if (input == strm_tknzr.TT_WORD)
	      applet.ir_area.append (strm_tknzr.sval + ' ');
	    else if (input == strm_tknzr.TT_NUMBER)
	      applet.ir_area.append (Double.toString(strm_tknzr.nval) + ' ');
	    input = get_next_token(strm_tknzr);

	  } // while
*/	  applet.showStatus("ir received");
	} // end if ("ir")


      } // end if ("output") 

      if (input == strm_tknzr.TT_EOF) {
	System.out.println("Auf Wiedersehen!");
      } // end if (strm_tknzr.TT_EOF)
/*      try {
	bffrd_rdr.readLine();
      } catch (IOException ioe) {System.err.println(ioe);}
*/
    } while (input != strm_tknzr.TT_EOF);
    System.out.println ("do...while left");
  } // public void run

  // -----------------
  /**
    * This method gets the next token from the bffrd_rdr via strm_tknzr,
    * stores it in the input variable, prints it in the in_out_control_area
    * of the legosim and returns it.
    */
  public int get_next_token (StreamTokenizer strm_tknzr) {
    int input = 0;

    try {
      if ((input = strm_tknzr.nextToken()) == strm_tknzr.TT_WORD)
	applet.in_out_control_area.append (strm_tknzr.sval + ' ');
      else if (input == strm_tknzr.TT_NUMBER)
	applet.in_out_control_area.append (Double.toString(strm_tknzr.nval));
      else if (input == strm_tknzr.TT_EOL) {
	applet.in_out_control_area.append ("\n");
      }
    } //try
    catch (IOException ioe) {System.err.println (ioe);}
      return input;
  }

} // InServer extends Thread




