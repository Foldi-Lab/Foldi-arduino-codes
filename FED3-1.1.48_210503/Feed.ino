/********************************************************
  Feed function.  This turns the stepper forward and backwards to dispense a pellet,
  stopping once pellet is dispensed.
********************************************************/
void Feed() {
  if (FEDmode != 5 && FEDmode != 9 && FEDmode != 10) { //if not an extinction session
    display.fillTriangle (12, 55, 18, 59, 12, 63, WHITE);
    display.fillTriangle (12, 75, 18, 79, 12, 83, WHITE);
    DisplayDispense();
    digitalWrite (MOTOR_ENABLE, HIGH);  //Enable motor driver

    //Move pellet disk to dispense a pellet
    while (digitalRead (PELLET_WELL) == HIGH && PelletAvailable == false) {
      CountReady = true;
      if (digitalRead (PELLET_WELL) == HIGH) {  //Every stepper movement keep checking if pellet is there
        stepper.step(-45);
        if (digitalRead (PELLET_WELL) == HIGH) {
          stepper.step(-45);
          if (digitalRead (PELLET_WELL) == HIGH) {
            stepper.step(-45);
            if (digitalRead (PELLET_WELL) == HIGH) {
              stepper.step(-45);
              if (digitalRead (PELLET_WELL) == HIGH) {
                stepper.step(-45);
                if (digitalRead (PELLET_WELL) == HIGH) {
                  stepper.step(-45);
                }
              }
            }
          }
        }
      }

      //2 second timer  between dispending movements
      for (int i = 1; i < 200; i++) {
        if (digitalRead (PELLET_WELL) == HIGH && PelletAvailable == false) {
          delay(10);
        }
      }
      numMotorTurns++;

      //Three movements to address jams:

      //minor adjustment to try to dislodget pellet
      if (numMotorTurns == 5 or numMotorTurns == 15) {
        stepper.step(100);
        //1 second timer  between dispending movements
        for (int i = 1; i < 100; i++) {
          if (digitalRead (PELLET_WELL) == HIGH && PelletAvailable == false) {
            delay(10);
          }
        }
      }

      if (numMotorTurns == 10) {
        VibrateJam();
      }

      if (numMotorTurns == 20) {
        ClearJam();
      }
    }
  }
}

/********************************************************
   Try vibrations to clear jam
********************************************************/
void VibrateJam() {
  if (FEDmode != 5) {
    numJamClears++;
    DisplayJamClear();
    if (digitalRead (PELLET_WELL) == HIGH) {
      delay (250); //simple debounce to ensure pellet is out for at least 250ms
      if (digitalRead (PELLET_WELL) == HIGH) {
        digitalWrite (MOTOR_ENABLE, HIGH);  //Enable motor driver
        for (int j = 0; j < 30; j++) {
          if (digitalRead (PELLET_WELL) == HIGH) {
            stepper.step(120);
            if (digitalRead (PELLET_WELL) == HIGH) {
              stepper.step(-60);
            }
          }
        }
        ReleaseMotor ();
      }
    }

  }
  display.fillRoundRect (5, 15, 120, 15, 1, WHITE);  //erase the "Jam clear" text without clearing the entire screen by pasting a white box over it
}

/********************************************************
    ClearJam makes full rotations to try to dislodge a pellet jam.
********************************************************/
void ClearJam() {
  if (FEDmode != 5) {
    numJamClears++;
    DisplayJamClear();
    if (digitalRead (PELLET_WELL) == HIGH) {
      delay (250); //simple debounce to ensure pellet is out for at least 250ms
      if (digitalRead (PELLET_WELL) == HIGH) {
        digitalWrite (MOTOR_ENABLE, HIGH);  //Enable motor driver
        for (int i = 0; i < 21 + random(0, 20); i++) {
          if (digitalRead (PELLET_WELL) == HIGH) {
            stepper.step(-i * 4);
          }
        }
      }
      ReleaseMotor ();
    }
    if (digitalRead (PELLET_WELL) == HIGH) {
      delay (250); //simple debounce
      if (digitalRead (PELLET_WELL) == HIGH) {
        digitalWrite (MOTOR_ENABLE, HIGH);  //Enable motor driver
        for (int i = 0; i < 21 + random(0, 20); i++) {
          if (digitalRead (PELLET_WELL) == HIGH) {
            stepper.step(i * 4);
          }
        }
        ReleaseMotor ();
      }
    }
    display.fillRoundRect (5, 15, 120, 15, 1, WHITE);  //erase the "Jam clear" text without clearing the entire screen by pasting a white box over it
    numMotorTurns = 0;
  }
}
