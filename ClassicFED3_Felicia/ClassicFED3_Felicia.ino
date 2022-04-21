/*
  Feeding experimentation device 3 (FED3)
  Classic FED3 script
  This script mimicks the classic FED3 menuing system for selecting among the following programs

  // FEDmodes:
  // 0 Free feeding
  // 1 Timed feeding
  // 2 FR1
  // 3 FR3
  // 4 FR3
  // 5 Extinction
  // 6 FR1 (reversed)
  // 7 FR3 (reversed)
  // 8 FR5 (reversed)

  alexxai@wustl.edu
  December, 2020

  This project is released under the terms of the Creative Commons - Attribution - ShareAlike 3.0 license:
  human readable: https://creativecommons.org/licenses/by-sa/3.0/
  legal wording: https://creativecommons.org/licenses/by-sa/3.0/legalcode
  Copyright (c) 2020 Lex Kravitz

*/

#include <FED3.h>                //Include the FED3 library 
String sketch = "Classic";       //Unique identifier text for each sketch
FED3 fed3 (sketch);              //Start the FED3 object

//variables for PR tasks
int poke_num = 0;                                      // this variable is the number of pokes since last pellet
int pokes_required = 1;                                // increase the number of pokes required each time a pellet is received using an exponential equation

void setup() {
  fed3.ClassicFED3 = true;
  fed3.begin();                                        //Setup the FED3 hardware
}

void loop() {
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                                                                     Mode 0: Free feeding
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (fed3.FEDmode == 0) {
    fed3.sessiontype = "Free_feed";                     //The text in "sessiontype" will appear on the screen and in the logfile
    fed3.DisplayPokes = false;                          //Turn off poke indicators for free feeding mode
    fed3.UpdateDisplay();                               //Update display for free feeding session to remove poke displayt (they are on by default)
    fed3.Feed();
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                                                                     Mode 1: Timed Feeding
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (fed3.FEDmode == 1) {
    fed3.sessiontype = "Timed";                         //The text in "sessiontype" will appear on the screen and in the logfile
    fed3.DisplayPokes = false;                          //Turn off poke indicators for free feeding mode
    fed3.DisplayTimed = true;                           //Display timed feeding info
    fed3.UpdateDisplay();
    if (fed3.currentHour >= fed3.timedStart && fed3.currentHour < fed3.timedEnd) {
      fed3.Feed();
    }
  }
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                                                                     Modes 2-4: Fixed Ratio Programs FR1, FR3, FR5
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  if ((fed3.FEDmode == 2) or (fed3.FEDmode == 3) or (fed3.FEDmode == 4)) {
    if (fed3.FEDmode == 2) fed3.sessiontype = "FR1";    //The text in "sessiontype" will appear on the screen and in the logfile
    if (fed3.FEDmode == 3) fed3.sessiontype = "FR3";    //The text in "sessiontype" will appear on the screen and in the logfile
    if (fed3.FEDmode == 4) fed3.sessiontype = "FR5";    //The text in "sessiontype" will appear on the screen and in the logfile
    if (fed3.Left) {
      fed3.logLeftPoke();                               //Log left poke
      if (fed3.LeftCount % fed3.FR == 0) {              //if fixed ratio is  met
        fed3.ConditionedStimulus();                     //deliver conditioned stimulus (tone and lights)
        fed3.Feed();                                    //deliver pellet
      }
    }
    if (fed3.Right) {                                    //If right poke is triggered
      fed3.logRightPoke();
    }
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                                                                     Mode 5: Extinction
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (fed3.FEDmode == 5) {
    fed3.sessiontype = "Extinct";                        //The text in "sessiontype" will appear on the screen and in the logfile
    if (fed3.Left) {
      fed3.logLeftPoke();                                //Log left poke
      fed3.ConditionedStimulus();                        //deliver conditioned stimulus (tone and lights)
    }

    if (fed3.Right) {                                    //If right poke is triggered
      fed3.logRightPoke();
    }
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                                                                     Mode 6-8: Reversed Fixed Ratio Programs FR1, FR3, FR5
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if ((fed3.FEDmode == 6) or (fed3.FEDmode == 7) or (fed3.FEDmode == 8)) {
    if (fed3.FEDmode == 6) fed3.sessiontype = "FR1 reversed";    //The text in "sessiontype" will appear on the screen and in the logfile
    if (fed3.FEDmode == 7) fed3.sessiontype = "FR3 reversed";    //The text in "sessiontype" will appear on the screen and in the logfile
    if (fed3.FEDmode == 8) fed3.sessiontype = "FR5 reversed";    //The text in "sessiontype" will appear on the screen and in the logfile
    fed3.activePoke = 0;                                  //Set activePoke to 0 to make right poke active
    if (fed3.Left) {                                      //If left poke
      fed3.logLeftPoke();                                 //Log left poke
    }
    if (fed3.Right) {                                     //If right poke is triggered
      fed3.logRightPoke();                                //Log Right Poke
      if (fed3.RightCount % fed3.FR == 0) {               //if fixed ratio is  met
        fed3.ConditionedStimulus();                       //Deliver conditioned stimulus (tone and lights)
        fed3.Feed();                                      //deliver pellet
    }
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                                                                    Call fed.run at least once per loop
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  fed3.run();
  }
}
