/*
  Feeding experimentation device 3 (FED3)
  Test program for checking if rats/mice can break beam of feeding well with their nose
  alexxai@wustl.edu
  March 2022

  This project is released under the terms of the Creative Commons - Attribution - ShareAlike 3.0 license:
  human readable: https://creativecommons.org/licenses/by-sa/3.0/
  legal wording: https://creativecommons.org/licenses/by-sa/3.0/legalcode
  Copyright (c) 2020 Lex Kravitz
*/

#include <FED3.h>                                       //Include the FED3 library 
String sketch = "FoldiTest";                                  //Unique identifier text for each sketch
FED3 fed3 (sketch);                                     //Start the FED3 object

void setup() {
  fed3.begin();                                         //Setup the FED3 hardware

  //Add these two lines to disable sleep and monitor the pellet well from the Arduino script
  fed3.disableSleep();                                  
  pinMode (1, INPUT_PULLUP);
}

void loop() {
  fed3.run();                                           //Call fed.run at least once per loop
  if (fed3.Left) {                                      //If left poke is triggered
    fed3.logLeftPoke();                                 //Log left poke
    fed3.ConditionedStimulus();                         //Deliver conditioned stimulus (tone and lights for 200ms)
    fed3.Feed();                                        //Deliver pellet
  }

  //The below code will directly read the state of the pellet well (Pin 1).  If Pin 1 goes "LOW" it means the mouse broke 
  //the beam in the pellet well, which will increase the "Right Pokes" count on the screen and SD card.  
  //The 500ms delay is to set a max of 2 counts per second, even if the animal keeps interacting with the pellet well
  if (digitalRead(1) == LOW) {                                   //If right poke is triggered
    fed3.logRightPoke();                                //Log right poke
    delay(500);
  }
}
