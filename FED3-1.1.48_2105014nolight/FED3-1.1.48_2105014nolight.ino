/********************************************************
  Feeding Experimentation Device version 3 (FED3)
  Code by Lex Kravitz
  Washington University in St Louis
  alexxai@wustl.edu
  August 2020
********************************************************/

//  FED3 version number
char ver[] = "1.1.48";

/********************************************************
  Updates in this version of the code
  (see change log on Github site for more details: https://github.com/KravitzLabDevices/FED3)
  - inhibit checkreset once FED has started up
  
  FED is a Feeding Experimentation Device.  It is a smart pellet dispenser
  that logs the timestamp whenever a pellet is removed. These timestamps
  can be used to recreate feeding records over multiple days. FED3 also includes
  operant pokes and stimuli for training mice to obtain pellets.

  FED was originally developed by Nguyen at al and published in 2016:
  https://www.ncbi.nlm.nih.gov/pubmed/27060385

  This code includes code from:
  *** Adafruit, who made the hardware breakout boards and associated code we used in FED ***

  Cavemoa's excellent examples of datalogging with the Adalogger:
  https://github.com/cavemoa/Feather-M0-Adalogger

  Uses Arduino Time library http://playground.arduino.cc/code/time
  Maintained by Paul Stoffregen https://github.com/PaulStoffregen/Time

  Arduino Zero RTC library https://www.arduino.cc/en/Reference/RTC
  Maintained by Arturo Guadalupi https://github.com/arduino-libraries/RTCZero

  This project is released under the terms of the Creative Commons - Attribution - ShareAlike 3.0 license:
  human readable: https://creativecommons.org/licenses/by-sa/3.0/
  legal wording: https://creativecommons.org/licenses/by-sa/3.0/legalcode
  Copyright (c) 2019, 2020 Lex Kravitz

********************************************************/


/********************************************************
  Setup code
********************************************************/
#include "a_header.h" //See "a_Header.h" for #defines and other constants 

void setup() {
  StartUpCommands();
}

/********************************************************
  Main loop
********************************************************/
void loop() {
  CheckForPellet();
  if (PelletAvailable && FEDmode != 5) {  // Don't sleep during extinction testing
    sleep();
  }
  else {
    CheckPokes();
    if (Ratio_Met == true)   Feed();
  }
}
