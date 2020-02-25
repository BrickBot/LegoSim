/**
 * @Name    legosim
 * @Version 0.1
 * @Autor   Oliver Buehn
 * Don't observe the code. It it terrible down there. There is character
 * orientation rather than object orientation. ;-)
 * This will be changed in the near future.
 */

/*! \file   legosim.java
    \brief  Java Applet GUI for LegoSim
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

import java.applet.*;
import java.io.*;
import java.lang.*;
import java.util.*;
import java.awt.*;
import java.awt.event.*;

public class legosim extends Applet implements MouseListener,
MouseMotionListener, ActionListener {

  final String CONST_STR = "AB";

  // declaration of methods of the interface MouseListener (the rest is
  // used further down):

  public void mouseClicked (MouseEvent mouse_event) {
  }
  public void mouseEntered (MouseEvent mouse_event) {
  }
  public void mouseExited  (MouseEvent mouse_event) {
  }

  // declaration of methods of the interface MouseMotionListener (the rest
  // is used further down:

  public void mouseMoved (MouseEvent mouse_event) {
  }


  InServer in_server;  // the thread which handles stdin.

  protected Image  rcx_image;         // image (, that contains rects).
  protected Vector rects;            // list of rectangles to be clicked.
  protected Image  guy_stand_image; // image of the tiny guy on the lcd.
  protected Image  guy_walk_image; // image of the tiny guy on the lcd.

  TextArea  in_out_control_area; // all io activities will be printed here.
  TextArea  ir_area;            // the infra red interface.
  TextField sensor1;
  TextField sensor2;
  TextField sensor3;
  TextField sensor1_stat;
  TextField sensor2_stat;
  TextField sensor3_stat;
  TextField actuatorA;
  TextField actuatorB;
  TextField actuatorC;
  TextField actuatorA_stat;
  TextField actuatorB_stat;
  TextField actuatorC_stat;

  Button ir_clear_button = new Button("ir clear");
  Button ir_send_button  = new Button("ir send");

  String ir_text  = "";
  String lcd_text = "";
  protected Font default_font = new Font("monospaced", Font.PLAIN, 12);
  protected Font lcd_font     = new Font("monospaced", Font.BOLD, 17);

  boolean is_on        = false;   // true, if the rcx-module is switched on.
  boolean prgrm_loaded = false;  // true, if a program is transfered to rcx.
  boolean is_running   = false; // true, if the rcx-program is running.

  Color cmpnnt_clr = new Color (255, 250, 25); //Lego-yellow simulation.
  //still poorly because of some error in color application.

  int i = 0;

    //----------------
    public void init()
    {
      try {

	loadImage();
	setBackground(Color.lightGray);
	setFont(default_font);

	char c;

	rects = new Vector();
	ImagemapRectangle rctngl;
	int i = 1;

	// get parameters from the *.html start-file (rcx-start.html)
       	while ((rctngl = getRectangleParameter ("rect"+i)) != null)
	{
	    rects.addElement(rctngl);
	    i++;
	}

	in_out_control_area = new TextArea("", 3, 40, 1);
	ir_area        = new TextArea("", 3, 15, 1); //rows, cols, 1 scrllbar.
	sensor1        = new TextField(5);
	sensor2        = new TextField(5);
	sensor3        = new TextField(5);
	sensor1_stat   = new TextField(7);
	sensor2_stat   = new TextField(7);
	sensor3_stat   = new TextField(7);
	actuatorA      = new TextField(5);
	actuatorB      = new TextField(5);
	actuatorC      = new TextField(5);
	actuatorA_stat = new TextField(9);
	actuatorB_stat = new TextField(9);
	actuatorC_stat = new TextField(9);

	in_out_control_area.setEditable(false);
	sensor1_stat.setEditable(false);
	sensor2_stat.setEditable(false);
	sensor3_stat.setEditable(false);
	actuatorA.setEditable(false);
	actuatorB.setEditable(false);
	actuatorC.setEditable(false);
	actuatorA_stat.setEditable(false);
	actuatorB_stat.setEditable(false);
	actuatorC_stat.setEditable(false);

	sensor1.setBackground(cmpnnt_clr);
	sensor2.setBackground(cmpnnt_clr);
	sensor3.setBackground(cmpnnt_clr);
	sensor1_stat.setBackground(cmpnnt_clr);
	sensor2_stat.setBackground(cmpnnt_clr);
	sensor3_stat.setBackground(cmpnnt_clr);
	actuatorA.setBackground(cmpnnt_clr);
	actuatorB.setBackground(cmpnnt_clr);
	actuatorC.setBackground(cmpnnt_clr);
	actuatorA_stat.setBackground(cmpnnt_clr);
	actuatorB_stat.setBackground(cmpnnt_clr);
	actuatorC_stat.setBackground(cmpnnt_clr);
	in_out_control_area.setBackground(cmpnnt_clr);
	ir_area.setBackground(cmpnnt_clr);

	setLayout(new BorderLayout());   // for the whole thing.

	Panel ir_panel = new Panel();
        ir_panel.setBackground(Color.gray);

	Panel ir_send_clear_panel = new Panel();
        ir_send_clear_panel.setBackground(Color.gray);
	ir_send_clear_panel.setLayout(new BorderLayout());
	ir_send_clear_panel.add("North", ir_clear_button);
	ir_send_clear_panel.add("South", ir_send_button);
	ir_panel.add("Center", ir_area);
	ir_panel.add("Center", ir_send_clear_panel);

	Panel sensor_panel = new Panel();
	sensor_panel.setBackground(Color.gray);
	sensor_panel.setLayout(new FlowLayout());
	Panel sensor1_panel = new Panel();
        sensor1_panel.setBackground(Color.gray);
	sensor1_panel.setLayout(new GridLayout(3, 1));
	Panel sensor2_panel = new Panel();
        sensor2_panel.setBackground(Color.gray);
	sensor2_panel.setLayout(new GridLayout(3, 1));
	Panel sensor3_panel = new Panel();
        sensor3_panel.setBackground(Color.gray);
	sensor3_panel.setLayout(new GridLayout(3, 1));

	Panel actuator_panel = new Panel();
	actuator_panel.setBackground(Color.gray);
	actuator_panel.setLayout(new FlowLayout());
	Panel actuatorA_panel = new Panel();
        actuatorA_panel.setBackground(Color.gray);
	actuatorA_panel.setLayout(new GridLayout(3, 1));
	Panel actuatorB_panel = new Panel();
        actuatorB_panel.setBackground(Color.gray);
	actuatorB_panel.setLayout(new GridLayout(3, 1));
	Panel actuatorC_panel = new Panel();
        actuatorC_panel.setBackground(Color.gray);
	actuatorC_panel.setLayout(new GridLayout(3, 1));

	Panel north_panel = new Panel();
	north_panel.setLayout(new GridLayout(2, 1));

	Panel south_panel = new Panel();
	south_panel.setLayout(new GridLayout(2, 1));

//	sensor1_panel.add(new Label("1", Label.CENTER));
	sensor1_panel.add(sensor1_stat);
	sensor1_panel.add(sensor1);
//	sensor2_panel.add(new Label("2", Label.CENTER));
	sensor2_panel.add(sensor2_stat);
	sensor2_panel.add(sensor2);
//	sensor3_panel.add(new Label("3", Label.CENTER));
	sensor3_panel.add(sensor3_stat);
	sensor3_panel.add(sensor3);

	actuatorA_panel.add(new Label("A", Label.CENTER));
	actuatorA_panel.add(actuatorA_stat);
	actuatorA_panel.add(actuatorA);
	actuatorB_panel.add(new Label("B", Label.CENTER));
	actuatorB_panel.add(actuatorB_stat);
	actuatorB_panel.add(actuatorB);
	actuatorC_panel.add(new Label("C", Label.CENTER));
	actuatorC_panel.add(actuatorC_stat);
	actuatorC_panel.add(actuatorC);

	sensor_panel.add(sensor1_panel);
	sensor_panel.add(sensor2_panel);
	sensor_panel.add(sensor3_panel);

	actuator_panel.add(actuatorA_panel);
	actuator_panel.add(actuatorB_panel);
	actuator_panel.add(actuatorC_panel);

	north_panel.add(ir_panel);
	north_panel.add(sensor_panel);

	south_panel.add(actuator_panel);
//	south_panel.add(new Label("input/output control area:"));
	south_panel.add(in_out_control_area);

	add("North", north_panel);
	add("South", south_panel);

       	i = 0;

	in_server = new InServer(this);
	in_server.start();

	addMouseListener      (this);
	addMouseMotionListener(this);
	sensor1.addActionListener(this);
	sensor2.addActionListener(this);
	sensor3.addActionListener(this);
	ir_clear_button.addActionListener(this);
	ir_send_button.addActionListener(this);


    }
      catch (NullPointerException npe) {
	System.out.println (npe.getLocalizedMessage());
	npe.printStackTrace();
      }

    } // init

    public void loadImage()
    {
	rcx_image       = getImage(getCodeBase(), "images/rcx.gif");
	guy_stand_image = getImage(getCodeBase(), "images/guy_stand.gif");
	guy_walk_image  = getImage(getCodeBase(), "images/guy_walk.gif");
    }

    /**
     * (overload update to *not* erase the background before painting)
     */
    public void update(Graphics g)
    {
	paint(g);
    }

    /**
     * paint the rcx_image and the other graphical things [, then call
     * super.paint which
     * will paint all contained components]
     */
    public void paint(Graphics g) {
      g.translate(0, 122);  // sets the new origin of the applet.
      g.drawImage(rcx_image, 0, 0, this);  // paint the image

      g.clearRect(109, 207, 95, 25);
      if (! lcd_text.equals("")) {  // if str. empty, none will be put.
	g.setFont(lcd_font);
	g.drawString(lcd_text, 134, 225);

	if (is_running)
	  g.drawImage(guy_walk_image, 175, 214, 10, 16, this);
	else
	  g.drawImage(guy_stand_image, 175, 214, 10, 16, this);

      } // end if ...equals("")
    }

    //***********************************************
    // find the rectangle we´re inside:
    private ImagemapRectangle findrect (int x, int y)
    {
	int i;
	ImagemapRectangle rctngl = null;

	for (i = 0; i < rects.size(); i++) {
	  rctngl = (ImagemapRectangle) rects.elementAt(i);
	  if (rctngl.contains(x, y)) break;
	}
	if (i < rects.size()) return rctngl;
	else return null;
    }

    private ImagemapRectangle lastrect;

    //********************************************************
    // On pressed mouse, highlight the rect., display a message:
  public void mousePressed (MouseEvent mouse_event) {
      int x, y;
      x = mouse_event.getX();
      y = mouse_event.getY();
      ImagemapRectangle rctngl = findrect (x, y);
      if (rctngl == null) return;
      Graphics g = this.getGraphics();
      g.setXORMode (Color.gray);
      g.drawRect (rctngl.x, rctngl.y, rctngl.width, rctngl.height);

      if (rctngl.id == 1) {
	if (! is_on) {
	  is_on = true;
	  if (prgrm_loaded)
	    lcd_text = "LEGO S";
	  else
	    lcd_text = "     1";
	  repaint();
	}
	else {
	  is_running = false;
	  is_on = false;
	}
	this.showStatus ("On-Off");
	putOut ("input button onoff pressed");
      }

      if (rctngl.id == 2) {
	is_running = !is_running;  // switching running/not running.
	repaint();
	this.showStatus ("Run");
	putOut ("input button run pressed");
      }

      if (rctngl.id == 3)
//	  putOut("input sensor 1 0xF000");
	putOut("input sensor 1 0");

      if (rctngl.id == 4)
//	  putOut("input sensor 2 0xF000");
	putOut("input sensor 2 0");

      if (rctngl.id == 5)
//	  putOut("input sensor 3 0xF000");
	putOut("input sensor 3 0");

      lastrect = rctngl;
      return;
    }

    //********************************************
    // unhighlight when mouse released;
  // perform action, if still inside the rect. where the mouse was pressed:
    public void mouseReleased (MouseEvent mouse_event) {

      int x = mouse_event.getX();
      int y = mouse_event.getY();

      if (lastrect != null) {
	Graphics g = this.getGraphics();
	g.setXORMode (Color.gray);
	g.drawRect (lastrect.x, lastrect.y,
		    lastrect.width, lastrect.height);
	this.showStatus ("");
	ImagemapRectangle rctngl = findrect (x, y);
	if ((rctngl != null) && (rctngl == lastrect)) {
	  if (rctngl.id == 1) {
	    if (! is_on) {
	      lcd_text = "";
	      repaint();
	    }
	    this.showStatus ("On-Off");
	    putOut ("input button onoff released");
	  }
	  if (rctngl.id == 2) {
	    this.showStatus ("Run");
	    putOut ("input button run released");
	  }

	  if (rctngl.id == 3)
//	    putOut("input sensor 1 0x0");
	    putOut("input sensor 1 70000");

	  if (rctngl.id == 4)
//	    putOut("input sensor 2 0x0");
	    putOut("input sensor 2 70000");

	  if (rctngl.id == 5)
//	    putOut("input sensor 3 0x0");
	    putOut("input sensor 3 70000");
	}

	lastrect = null;
      }
      return;
    }

    //************************************************
    // prints x/y-coordinates when dragging the mouse:
    public void mouseDragged (MouseEvent mouse_event) {
      Graphics g = this.getGraphics();
      this.showStatus (mouse_event.getX() + "x" + mouse_event.getY() + "y");
      return;
    }

    //*************************************************************
    protected ImagemapRectangle getRectangleParameter (String name)
    {
      int x, y, w, h, id;

      String value = this.getParameter (name);
      if (value == null) return null;

      try
	{
	  StringTokenizer st = new StringTokenizer (value, ",");

	  x  = Integer.parseInt (st.nextToken());
	  y  = Integer.parseInt (st.nextToken());
	  w  = Integer.parseInt (st.nextToken());
	  h  = Integer.parseInt (st.nextToken());
	  id = Integer.parseInt (st.nextToken());
	}
      catch (NoSuchElementException e) {return null;}
      catch (NumberFormatException e) {return null;}

      return new ImagemapRectangle (x, y, w, h, id);
    }

    //*************************************************************
  public void actionPerformed (ActionEvent action_event) {
    if (action_event.getSource() == sensor1) {
      putOut("input sensor 1 " + sensor1.getText());
      showStatus("sensor 1 sensed");
    } 
    if (action_event.getSource() == sensor2) {
      putOut("input sensor 2 " + sensor2.getText());
      showStatus("sensor 2 sensed");
    } 
    if (action_event.getSource() == sensor3) {
      putOut("input sensor 3 " + sensor3.getText());
      showStatus("sensor 3 sensed");
    }
    
    if (action_event.getActionCommand().equals("ir send")) {
      ir_text = ir_area.getText();
      putOut("input ir " + String.valueOf(ir_text.length()) +
	     " " + ir_text);

      showStatus("infra red send");
    }      
    else if (action_event.getActionCommand().equals("ir clear"))
      ir_area.setText ("");

    return;
  }


    //***************************************
    // has 1 more field (id) as java.awt.Rectangle:
    class ImagemapRectangle extends Rectangle {
      int id;
      public ImagemapRectangle (int x, int y, int w, int h, int id) {
	super (x, y, w, h);
	this.id = id;
      }
    }

    /**
     * Prints the string to stdout and to the in_out_control_area within
     * the legosim.
     */ 
    public void putOut (String out_string) {
      System.out.println (out_string);
      in_out_control_area.append (out_string + "\n");
    }

} // end legosim

