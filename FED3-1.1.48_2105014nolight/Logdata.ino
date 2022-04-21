void logdata() {
  DisplaySDLogging();
  WriteToSD();
}

// Create new file on uSD incrementing file name as required
void CreateFile() {
  //put this next line *Right Before* any file open line:
  SdFile::dateTimeCallback(dateTime);

  // see if the card is present and can be initialized:
  if (!SD.begin(cardSelect, SD_SCK_MHZ(4))) {
    error(2);
  }

  // create files if they dont exist and grab device name and ratio
  configfile = SD.open("DeviceNumber.csv", FILE_WRITE);
  configfile = SD.open("DeviceNumber.csv", FILE_READ);
  FED = configfile.parseInt();
  configfile.close();

  ratiofile = SD.open("FEDmode.csv", FILE_WRITE);
  ratiofile = SD.open("FEDmode.csv", FILE_READ);
  FEDmode = ratiofile.parseInt();
  ratiofile.close();

  startfile = SD.open("start.csv", FILE_WRITE);
  startfile = SD.open("start.csv", FILE_READ);
  timedStart = startfile.parseInt();
  startfile.close();

  stopfile = SD.open("stop.csv", FILE_WRITE);
  stopfile = SD.open("stop.csv", FILE_READ);
  timedEnd = stopfile.parseInt();
  stopfile.close();

  // Name filename in format F###_MMDDYYNN, where MM is month, DD is day, YY is year, and NN is an incrementing number for the number of files initialized each day
  strcpy(filename, "FED_____________.CSV");  // placeholder filename
  getFilename(filename);
}

void CreateDataFile () {
  getFilename(filename);
  logfile = SD.open(filename, FILE_WRITE);
  if ( ! logfile ) {
    error(3);
  }
}

// Write data header to file of uSD.
void writeHeader() {
  logfile.println("MM:DD:YYYY hh:mm:ss,FED_Version,Device_Number,Battery_Voltage,Motor_Turns,Session_Type,Event,Active_Poke,Left_Poke_Count,Right_Poke_Count,Pellet_Count,Retrieval_Time");
}

void writeConfigFile() {
  configfile = SD.open("DeviceNumber.csv", FILE_WRITE);
  configfile.rewind();
  configfile.println(FED);
  configfile.flush();
  configfile.close();
}

void writeFEDmode() {
  ratiofile = SD.open("FEDmode.csv", FILE_WRITE);
  ratiofile.rewind();
  ratiofile.println(FEDmode);
  ratiofile.flush();
  ratiofile.close();

  startfile = SD.open("start.csv", FILE_WRITE);
  startfile.rewind();
  startfile.println(timedStart);
  startfile.flush();
  startfile.close();

  stopfile = SD.open("stop.csv", FILE_WRITE);
  stopfile.rewind();
  stopfile.println(timedEnd);
  stopfile.flush();
  stopfile.close();
}

// Print data and time followed by pellet count and motorturns to SD card
void WriteToSD() {
  DateTime now = rtc.now();
  logfile.print(now.month());
  logfile.print("/");
  logfile.print(now.day());
  logfile.print("/");
  logfile.print(now.year());
  logfile.print(" ");
  logfile.print(now.hour());
  logfile.print(":");
  if (now.minute() < 10)
    logfile.print('0');      // Trick to add leading zero for formatting
  logfile.print(now.minute());
  logfile.print(":");
  if (now.second() < 10)
    logfile.print('0');      // Trick to add leading zero for formatting
  logfile.print(now.second());
  logfile.print(",");

  logfile.print(ver); // Print device name
  logfile.print(",");
  
  logfile.print(FED); // Print device name
  logfile.print(",");

  logfile.print(measuredvbat); // Print battery voltage
  logfile.print(",");

  logfile.print((numJamClears * 10) + numMotorTurns); // Print the number of attempts to dispense a pellet, including through jam clears
  numMotorTurns = 0; //reset numMotorTurns
  numJamClears = 0; // reset numJamClears
  logfile.print(",");

  if (FEDmode == 4) {
    logfile.print("PR");
    logfile.print(round ((5 * exp (0.2 * PelletCount)) - 5)); // Print current PR ratio
    logfile.print(",");
  }

  else if (FEDmode == 8) {
    logfile.print("PR_reversed");
    logfile.print(round ((5 * exp (0.2 * PelletCount)) - 5)); // Print current PR ratio
    logfile.print(",");
  }

  else if (FEDmode == 0) {
    logfile.print("FED"); // Print trial type
    logfile.print(",");
  }

  else if (FEDmode == 5) {
    logfile.print("Extinction"); // Print trial type
    logfile.print(",");
  }

  else if (FEDmode == 6) {
    logfile.print("FR1_Light_tracking"); // Print trial type
    logfile.print(",");
  }

  else if (FEDmode == 7) {
    logfile.print("FR1_reversed"); // Print trial type
    logfile.print(",");
  }

  else if (FEDmode == 9) {
    logfile.print("Self_stim"); // Print trial type
    logfile.print(",");
  }

  else if (FEDmode == 10) {
    logfile.print("Self_stim_reversed"); // Print trial type
    logfile.print(",");
  }

  else if (FEDmode == 11) {
    logfile.print("Timed_"); // Print trial type
    logfile.print(timedStart); // Print trial type
    logfile.print("to"); // Print trial type
    logfile.print(timedEnd); // Print trial type
    logfile.print(",");
  }
  
  else if (FEDmode == 12) {
    logfile.print("FR3_reversed"); // Print trial type
    logfile.print(",");
  }
  
  else if (FEDmode == 13) {
    logfile.print("FR5_reversed"); // Print trial type
    logfile.print(",");
  }
  
  else if (FEDmode == 14) {
    logfile.print("FR1_both"); // Print trial type
    logfile.print(",");
  }
  
  else {
    logfile.print("FR"); // Print ratio
    logfile.print(FR); // Print ratio
    logfile.print(",");
  }

  // Print event type
  if (pellet == true ) logfile.print("Pellet"); // If this event is a pellet retrieval print "Pellet"
  if (pellet == false ) logfile.print("Poke"); // If this event is not a pellet retrieval print "Poke"
  logfile.print(",");

  if (FEDmode == 6 || FEDmode == 7 || FEDmode == 8 || FEDmode == 10 || FEDmode == 12 || FEDmode == 13) {
    if (activePoke == 0)  logfile.print("Right"); //
    if (activePoke == 1)  logfile.print("Left"); //
  }
  else if (FEDmode ==0 || FEDmode ==11 ){
    logfile.print("N/A");  
  }
  else if (FEDmode ==14){
    if (activePoke == 0)  logfile.print("Right"); //
    if (activePoke == 0)  logfile.print("Left"); //
  }
  else {
    logfile.print("Left");
  }
  logfile.print(",");

  logfile.print(LeftCount); // Print Left poke count
  logfile.print(",");

  logfile.print(RightCount); // Print Right poke count
  logfile.print(",");

  logfile.print(PelletCount); // print Pellet counts
  logfile.print(",");

  if (pellet  == false ) {
    logfile.println(sqrt (-1)); // print NaN if it's not a pellet line!
  }

  else if (retInterval < 120 ) {  // only log retrieval intervals below 2 minutes
    logfile.println(retInterval); // print interval between pellet dispensing and being taken
  }

  else if (retInterval >= 120) { 
    logfile.println("Timed_out"); // print "Timed_out" if retreival interval is >120
  }

  else {
    logfile.println("Error"); // print error if value is < 0 (this shouldn't ever happen)
  }

  Blink(GREEN_LED, 100, 2);
  logfile.flush();
  // logfile.close();
}

/********************************************************
  If any errors are detected with the SD card upon boot this function
  will blink both LEDs on the Feather M0, turn the NeoPixel into red wipe pattern,
  and display "Check SD Card" on the screen
********************************************************/
void error(uint8_t errno) {
  DisplaySDError();
  while (1) {
    uint8_t i;
    for (i = 0; i < errno; i++) {
      Blink(GREEN_LED, 25, 2);
      colorWipe(strip.Color(5, 0, 0), 25); // RED
    }
    for (i = errno; i < 10; i++) {
      colorWipe(strip.Color(0, 0, 0), 25); // clear
    }
    CheckReset();
  }
}

/********************************************************
  This function creates a unique filename for each file that
  starts with "FED", then the date in MMDDYY,
  then an incrementing number for each new file created on the same date
********************************************************/
void getFilename(char *filename) {
  DateTime now = rtc.now();

  filename[3] = FED / 100 + '0';
  filename[4] = FED / 10 + '0';
  filename[5] = FED % 10 + '0';
  filename[7] = now.month() / 10 + '0';
  filename[8] = now.month() % 10 + '0';
  filename[9] = now.day() / 10 + '0';
  filename[10] = now.day() % 10 + '0';
  filename[11] = (now.year() - 2000) / 10 + '0';
  filename[12] = (now.year() - 2000) % 10 + '0';
  filename[16] = '.';
  filename[17] = 'C';
  filename[18] = 'S';
  filename[19] = 'V';
  for (uint8_t i = 0; i < 100; i++) {
    filename[14] = '0' + i / 10;
    filename[15] = '0' + i % 10;

    if (! SD.exists(filename)) {
      break;
    }
  }
  return;
}
