/********************************************************
   FUNCTION IS CALLED WHEN PELLET_WELL_PIN IS INTERRUPTED
   AND TURNS THE RED LED ON WHENEVER DEVICE IS AWAKE
 *****************************************************/
void wake() {
  if (digitalRead(PELLET_WELL) == HIGH)   PelletAvailable = false;
}

void pelletWellTrigger() {
  if (digitalRead(PELLET_WELL) == LOW)   PelletAvailable = true;
}

/********************************************************
   FUNCTION FOR PUTTING THE ADALOGGER TO SLEEP
 *****************************************************/
void sleep () {
  ReleaseMotor ();
  Serial.println ("*****Sleeping*****"); //There is a problem with the M0 Adalogger board where it won't re-attach the USB connection after sleep.  It's a well documented error, but after this line runs the SerialPrints won't work anymore
  DisplaySleep();
  if (digitalRead(PELLET_WELL) == LOW ) {  //Only sleep if pellet is in well - in future versions we will change this to allow more sleeping
    ReadBatteryLevel();
    UpdateDisplay();
    rtc2.standbyMode();
  }
}

void ReleaseMotor () {
  digitalWrite (A2, LOW);
  digitalWrite (A3, LOW);
  digitalWrite (A4, LOW);
  digitalWrite (A5, LOW);
  digitalWrite (MOTOR_ENABLE, LOW);  //Disable motor driver
}
