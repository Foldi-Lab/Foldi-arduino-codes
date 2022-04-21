/*
  Feeding experimentation device 3 (FED3)
  Classic FED3 script
  This script mimicks the classic FED3 menuing system for selecting among the following programs

  // FEDmodes:
  // 0 Free Feeding
  // 1 Timed free feeding
  // 2 FR1
  // 3 Progressive Ratio
  // 4 FR1 (reversed)
  // 5 Progressive Ratio (reversed)
  // 6 Orientation (FR1 both)
  // 7 Reversal Task

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

//variables for Reversal Task
//(Set the trials to switch here for number of pellets between reversals)
int trialsToSwitch = 10;
int counter = 1;

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
  //                                                                     Mode 2: Fixed Ratio 1
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (fed3.FEDmode == 2)  {
    fed3.sessiontype = "FR1";    //The text in "sessiontype" will appear on the screen and in the logfile
    if (fed3.Left) {
      fed3.logLeftPoke();                               //Log left poke
      if (fed3.LeftCount % fed3.FR == 0) {              //if fixed ratio is  met
        fed3.ConditionedStimulus();                     //deliver conditioned stimulus (tone and lights)
        fed3.Feed();                                    //deliver pellet
        fed3.BNC(500, 1);                                //Send one 500ms pulse to the BNC output
      }
    }
    if (fed3.Right) {                                    //If right poke is triggered
      fed3.logRightPoke();
    }
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                                                                     Mode 3: Progressive Ratio
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (fed3.FEDmode == 3) {
    fed3.sessiontype = "ProgRatio";                      //The text in "sessiontype" will appear on the screen and in the logfile
    if (fed3.Left) {                                     //If left poke is triggered and pellet is not in the well
      fed3.logLeftPoke();                                //Log left poke
      fed3.Click();                                      //Click
      poke_num++;                                        //store this new poke number as current poke number.
      if (poke_num == pokes_required) {                  //check to see if the mouse has acheived the correct number of pokes in order to receive the pellet
        fed3.ConditionedStimulus();                      //Deliver conditioned stimulus (tone and lights)
        fed3.Feed();                                     //Deliver pellet
        pokes_required = round((5 * exp((fed3.PelletCount + 1) * 0.2)) - 5);  //increment the number of pokes required according to the progressive ratio:
        fed3.FR = pokes_required;
        poke_num = 0;                                    //reset poke_num back to 0 for the next trial
      }
    }
    if (fed3.Right) {                                    //If right poke is triggered and pellet is not in the well
      fed3.logRightPoke();
    }
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                                                                     Mode 4: Fixed Ratio 1 Reversed
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (fed3.FEDmode == 4) {
    fed3.sessiontype = "FR1_Rev";                         //The text in "sessiontype" will appear on the screen and in the logfile
    fed3.activePoke = 0;                                  //Set activePoke to 0 to make right poke active
    if (fed3.Left) {                                      //If left poke
      fed3.logLeftPoke();                                 //Log left poke
    }
    if (fed3.Right) {                                     //If right poke is triggered
      fed3.logRightPoke();                                //Log Right Poke
      fed3.ConditionedStimulus();                         //Deliver conditioned stimulus (tone and lights)
      fed3.Feed();                                        //deliver pellet
    }
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                                                                    Mode 5: Progressive Ratio (Reversed)
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (fed3.FEDmode == 5) {
    fed3.sessiontype = "PR_Rev";                          //The text in "sessiontype" will appear on the screen and in the logfile
    fed3.activePoke = 0;                                //Right poke is active
    if (fed3.Right) {                                   //If Right poke is triggered
      fed3.logRightPoke();                              //Log Right poke
      poke_num++;                                       //store this new poke number as current poke number.
      if (poke_num == pokes_required) {                 //check to see if the mouse has acheived the correct number of pokes in order to receive the pellet
        fed3.ConditionedStimulus();                     //Deliver conditioned stimulus (tone and lights)
        fed3.Feed();                                    //Deliver pellet
        //increment the number of pokes required according to the progressive ratio:
        pokes_required = round((5 * exp((fed3.PelletCount + 1) * 0.2)) - 5);
        fed3.FR = pokes_required;
        poke_num = 0;                                   //reset the number of pokes back to 0, for the next trial
        fed3.Right = false;
      }
      else {
        fed3.Click();                                   //If not enough pokes, just do a Click
      }
    }
    if (fed3.Left) {                                    //If left poke is triggered and pellet is not in the well
      fed3.logLeftPoke();
    }
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                                                                     Mode 6: Orientation (FR1 both)
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (fed3.FEDmode == 6) {
  fed3.sessiontype = "FR1 both";    //The text in "sessiontype" will appear on the screen and in the logfile
    if (fed3.Left) {
      fed3.logLeftPoke();                               //Log left poke
      if (fed3.LeftCount % fed3.FR == 0) {              //if fixed ratio is  met
        fed3.ConditionedStimulus();                     //deliver conditioned stimulus (tone and lights)
        fed3.Feed();                                    //deliver pellet
      }
    }
    if (fed3.Right) {                                    //If right poke is triggered
      fed3.logRightPoke();                               //Log Right Poke
      fed3.ConditionedStimulus();                        //Deliver conditioned stimulus (tone and lights)
      fed3.Feed();                                       //deliver pellet
    }
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                                                                     Mode 7: Reversal Task
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////
  // Determine which poke is active based on the trial number. 
  // fed3.activePoke=1 means Left poke is active, fed3.activePoke=0 means Right poke is active.
  ////////////////////////////////////////////////
 if (fed3.FEDmode == 7)  { 
  if (counter == ((trialsToSwitch * 2)+1)) counter = 1;
  if (counter <= trialsToSwitch) fed3.activePoke = 1;
  if (counter > trialsToSwitch) fed3.activePoke = 0;

  fed3.run();                                           //Call fed.run at least once per loop

  ////////////////////////////////////////////////
  // There are 4 cases we need to account for:
  ////////////////////////////////////////////////

  // Case 1: The *left* poke is active and animal pokes *left*
  if (fed3.activePoke == 1 and fed3.Left) {
    fed3.logLeftPoke();                                 //Log left poke
    fed3.ConditionedStimulus();                         //Deliver conditioned stimulus (tone and lights)
    fed3.Feed();                                        //Deliver pellet
    counter ++;
  }

  // Case 2: The *left* poke is active and animal pokes *right*
  if (fed3.activePoke == 1 and fed3.Right) {
    fed3.logRightPoke();                                 //Log left poke
    fed3.Click();
  }

  // Case 3: The *right* poke is active and animal pokes *left*
  if (fed3.activePoke == 0 and fed3.Left) {
    fed3.logLeftPoke();                                //Log right poke
    fed3.Click();
  }

  // Case 4: The *right* poke is active and animal pokes *right*
  if (fed3.activePoke == 0 and fed3.Right) {
    fed3.logRightPoke();                                //Log right poke
    fed3.ConditionedStimulus();                         //Deliver conditioned stimulus (tone and lights)
    fed3.Feed();                                        //Deliver pellet
    counter ++;
  }
}

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                                                                    Call fed.run at least once per loop
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  fed3.run();
}
