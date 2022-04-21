/*
  Feeding experimentation device 3 (FED3)

  This script demonstrates how to write a menu system so you can access multiple programs from the FED3 startup screen.  You will scroll through programs by assigning them to "modes".

  In this example, we run four modes:
  // FEDmodes:
  // 0 FR1 reversed
  // 1 FR1
  // 2 Probablistic Reversal Task
  // 3 Orientation (FR1 both)

  alexxai@wustl.edu
  December, 2020

  This project is released under the terms of the Creative Commons - Attribution - ShareAlike 3.0 license:
  human readable: https://creativecommons.org/licenses/by-sa/3.0/
  legal wording: https://creativecommons.org/licenses/by-sa/3.0/legalcode
  Copyright (c) 2020 Lex Kravitz

*/

#include <FED3.h>                //Include the FED3 library 
String sketch = "Menu";          //Unique identifier text for each sketch
FED3 fed3 (sketch);              //Start the FED3 object

//variables for Prob eversal task
//(Set the probabilities and trials to switch here for number of pellets between reversals)

int probability = 80;                                   //Probability of pellet on the high-reward poke
int trialsToSwitch = 10;                                //# of trials before probabilities on the pokes switch
int counter = 1;                                        //trial counter variable - don't change this
int trialTimeout = 5;                                   //timeout duration after each poke, set to 0 to remove the timeout

void setup() {
  fed3.FED3Menu = true;                                //Activate the menu function at startup
  fed3.begin();                                        //Setup the FED3 hardware
}

void loop() {
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                                                                     Mode 1: Fixed Ratio 1 Reversed
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (fed3.FEDmode == 0) {
    fed3.sessiontype = "FR1_R";                           //The text in "sessiontype" will appear on the screen and in the logfile
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
  //                                                                     Mode 2: Fixed Ratio 1
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  if (fed3.FEDmode == 1)  {
    fed3.sessiontype = "FR1";    //The text in "sessiontype" will appear on the screen and in the logfile
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
  //                                                                     Mode 3: Probabilistic Reversal Task
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////
  ////////////////////////////////////////////////
  // Determine which poke is active based on the trial number. 
  // fed3.activePoke=1 means Left poke is active, fed3.activePoke=0 means Right poke is active.
  ////////////////////////////////////////////////
  if (fed3.FEDmode == 2)  { 
  if (counter == ((trialsToSwitch * 2)+1)) counter = 1;
  if (counter <= trialsToSwitch) fed3.activePoke = 1;
  if (counter > trialsToSwitch) fed3.activePoke = 0;

  fed3.run();                                           //Call fed.run at least once per loop

  ////////////////////////////////////////////////
  // There are 4 cases we need to account for:
  ////////////////////////////////////////////////

  // Case 1: The *left* poke is the high probability poke and animal pokes *left*
  if (fed3.activePoke == 1 and fed3.Left) {
    fed3.logLeftPoke();                                 //Log left poke
    if (random(100) < probability) {                    //Select a random number between 0-100 and ask if it is between 0-80 (80% of the time).  If so:
      fed3.ConditionedStimulus();                         //Deliver conditioned stimulus (tone and lights)
      fed3.Feed();                                        //Deliver pellet
    }
    else {                                              //If random number is between 81-100 (20% of the time)
      fed3.Tone(4, 2);                                 //Play the error tone
    }
    counter ++;                                         //Regardless of the probabalistic outcome increment the trial counter
    fed3.Timeout(trialTimeout);                         //Time-out between trials
  }

  // Case 2: The *left* poke is the high probability poke and animal pokes *right*
  if (fed3.activePoke == 1 and fed3.Right) {
    fed3.logRightPoke();                                //Log Right poke
    if (random(100) > probability) {                    //Select a random number between 0-100 and ask if it is between 80-100 (20% of the time).  If so:
      fed3.ConditionedStimulus();                         //Deliver conditioned stimulus (tone and lights)
      fed3.Feed();                                        //Deliver pellet
    }
    else {                                              //If random number is between 0-80 (80% of the time)
      fed3.Tone(4, 2);                               //Play the error tone
    }
    counter ++;                                         //increment the trial counter
    fed3.Timeout(trialTimeout);                         //Time-out between trials
  }

  // Case 3: The *right* poke is the high probability poke and animal pokes *left*
  if (fed3.activePoke == 0 and fed3.Left) {
    fed3.logLeftPoke();                                 //Log left poke
    if (random(100) > probability) {                    //Select a random number between 0-100 and ask if it is between 80-100 (20% of the time).  If so:
      fed3.ConditionedStimulus();                         //Deliver conditioned stimulus (tone and lights)
      fed3.Feed();                                        //Deliver pellet
    }
    else {                                              //If random number is between 0-80 (80% of the time)
      fed3.Tone(40,2);                               //Play the error tone
    }
    counter ++;                                         //increment the trial counter
    fed3.Timeout(trialTimeout);                         //Time-out between trials
  }

  // Case 4: The *right* poke is the high probability poke and animal pokes *right*
  if (fed3.activePoke == 0 and fed3.Right) {
    fed3.logRightPoke();                                //Log Right poke
    if (random(100) < probability) {                    //Select a random number between 0-100 and ask if it is between 0-80 (80% of the time).  If so:
      fed3.ConditionedStimulus();                         //Deliver conditioned stimulus (tone and lights)
      fed3.Feed();                                        //Deliver pellet
    }
    else {                                              //If random number is between 80-100 (20% of the time)
      fed3.Tone(4, 2);                               //Play the error tone
    }
    counter ++;                                         //increment the trial counter
    fed3.Timeout(trialTimeout);                         //Time-out between trials
  }
}
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                                                                     Mode 4: Fixed ratio both
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (fed3.FEDmode == 3) {
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
  //                                                                     If a mode greater than 4 is selected 
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (fed3.FEDmode > 3) {
    fed3.DisplayNoProgram();
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                                                                    Call fed.run at least once per loop
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  fed3.run();
}
