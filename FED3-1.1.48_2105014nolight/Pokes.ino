void CheckPokes() {
  // If it's a free feeding session set Ratio_Met to true
  if (FEDmode == 0) Ratio_Met = true;

  // If it's a timed feeding session only set Ratio_Met to true between these hours
  if (FEDmode == 11)  {
    DateTime now = rtc.now();
    if (now.hour() >= timedStart && now.hour() < timedEnd) {
      Ratio_Met = true;
    }
  }

  CheckLeft();
  CheckRight();
//  CheckReset();
}


/********************************************************
  Check Left Poke
  This function checks the left poke and decideds what to do, when to log data, etc.

  This function calls log_data () when pokes occur that do not result in a pellet (in PR or extinction sessions).
  When pokes result in a pellet it does not call logdata() here because it will be called
  again once the pellet is taken, in order to log retrieval time.
********************************************************/
bool CheckLeft () {
  // Make sure Left poke is empty before "activating" it so if he holds his nose in the poke it won't re-trigger.
  if (digitalRead(LEFT_POKE) == HIGH  && FEDmode != 0 && FEDmode != 11) {
    LeftReady = true;
  }

  // If left poke is poked, add one to LeftCount, log data, and go through if statements to decide what to do based on FEDmode
  if (LeftReady == true & digitalRead(LEFT_POKE) == LOW) {
    LeftReady = false;
    LeftCount ++;
  //  Blink(BNC_OUT, 50, 2);
    pellet = false;
    logdata();

    //Free feeding
    if (FEDmode > 0) {
      display.fillCircle(25, 59, 5, BLACK);
      display.refresh();
    }

    // Progressive ratio
    if (FEDmode == 4) {
      if (LeftCount >= (ratio)) { // if LeftCount is greater than or equal to the required ratio
        Ratio_Met = true;
        ConditionedStimulus();
      }
    }

    // Fixed ratio 1, 3, or 5
    if (FEDmode < 4 && LeftCount % FR < 1 && LeftCount != 0) { //For fixed ratio sessions, test if the number of left counts is divisible by the FR
      Ratio_Met = true;
      ConditionedStimulus();
    }

    //Extinction session
    if (FEDmode == 5) { //If it's an extintion session do conditioned stimulus and log data after each poke
      ConditionedStimulus();
    }

    //Light tracking FR1
    if (FEDmode == 6) { //If it's a light tracking task
      if (activePoke == 1) {
        Ratio_Met = true;
        ConditionedStimulus();
      }
      else {
        ErrorStim();
      }
    }

    //Opto stim
    if (FEDmode == 9) { //OptoStim
      ConditionedStimulus();
      optoStim();
    }
    
    if (FEDmode == 14 ) { //For FR1 (both)
      Ratio_Met = true;
      ConditionedStimulus();
    }

    UpdateDisplay();
  }
}

/********************************************************
  Check Right Poke
  Same as CheckLeft above, but for the right poke
********************************************************/
bool CheckRight () {
  if (digitalRead(RIGHT_POKE) == HIGH && FEDmode != 0 && FEDmode != 11) {
    RightReady = true;
  }
  if (RightReady == true & digitalRead(RIGHT_POKE) == LOW ) {
    RightReady = false;
    RightCount ++;
  //  Blink(BNC_OUT, 50, 3);
    pellet = false;
    logdata();

    // free feeding
    if (FEDmode > 0) {
      display.fillCircle(25, 79, 5, BLACK);
      display.refresh();
    }

    // extinction
    if (FEDmode == 5 or FEDmode == 4) { //If it's an extintion session log data after every poke
    }

    if (FEDmode == 6) { //Light tracking task
      if (activePoke == 0) {
        Ratio_Met = true;
        RConditionedStim();
      }
      else {
        ErrorStim();
      }
    }

    if (FEDmode == 7 ) { //For FR1 (reversed)
      Ratio_Met = true;
      ConditionedStimulus();
    }

    if (FEDmode == 8 && RightCount >= (ratio)) { // if it's a progressive ratio schedule and the RightCount is greater than or equal to the ratio
      Ratio_Met = true;
      ConditionedStimulus();
    }

    if (FEDmode == 10) { //OptoStim
      ConditionedStimulus();
      optoStim();
    }
    
    if (FEDmode == 12 && RightCount % FR < 1 && RightCount != 0) { //For FR3 (reversed)//For fixed ratio sessions, test if the number of right counts is divisible by the FR
      Ratio_Met = true;
      ConditionedStimulus();
    }
    
    if (FEDmode == 13 && RightCount % FR < 1 && RightCount != 0) { //For FR5 (reversed)//For fixed ratio sessions, test if the number of right counts is divisible by the FR
      Ratio_Met = true;
      ConditionedStimulus();
    }  
    
    if (FEDmode == 14 ) { //For FR1 (both)
      Ratio_Met = true;
      ConditionedStimulus();
    }

    UpdateDisplay();
  }
}

/********************************************************
  Reset FED by holding left and right pokes for 1 second
  This only works when FED is awake, which means no pellet in the well
********************************************************/
void CheckReset() {  //
  if (digitalRead(LEFT_POKE) == LOW & digitalRead(RIGHT_POKE) == LOW ) {
    delay (1000);
    if (digitalRead(LEFT_POKE) == LOW & digitalRead(RIGHT_POKE) == LOW ) {
      display.clearDisplay();
      display.setRotation(3);
      display.setTextColor(BLACK);
      display.setCursor(15, 40);
      display.setTextSize(1);
      display.println("Resetting FED...");
      display.refresh();
      tone (BUZZER, 5000, 400); delay (200); tone (BUZZER, 2000, 300); delay (200); tone (BUZZER, 4000, 600);
      colorWipe(strip.Color(2, 0, 0), 40);  delay (100); // Color wipe
      colorWipe(strip.Color(2, 0, 2), 40); delay (100); // Color wipe
      colorWipe(strip.Color(0, 2, 2), 40); delay (100); // Color wipe
      colorWipe(strip.Color(0, 0, 0), 20); // OFF
      NVIC_SystemReset();      // processor software reset
    }
  }
}
