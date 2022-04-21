/********************************************************
  Conditioned stimuli
********************************************************/
void ConditionedStimulus() {
  digitalWrite (MOTOR_ENABLE, HIGH);
  tone (BUZZER, 4, 3);
  colorWipe(strip.Color(0, 0, 0), 0); // Color wipe
  colorWipe(strip.Color(0, 0, 0), 0); // OFF
  digitalWrite (MOTOR_ENABLE, LOW);
}

void RConditionedStim() {
  digitalWrite (MOTOR_ENABLE, HIGH);
  tone (BUZZER, 4, 3);
  RcolorWipe(strip.Color(0, 0, 0), 0); // Color wipe
  RcolorWipe(strip.Color(0, 0, 0), 0); // OFF
  digitalWrite (MOTOR_ENABLE, LOW);
}

void ErrorStim() {
  //random noise to signal errors
  for (int i = 0; i < 30; i++) {
    tone (BUZZER, random(100, 200), 20);
    delay(5);
  }
}

/********************************************************
  Visual tracking stimulus - left
********************************************************/
void leftStimulus() {
  digitalWrite (MOTOR_ENABLE, LOW);
  strip.setPixelColor(0, strip.Color(0, 0, 0, 0) );
  strip.show();
  digitalWrite (MOTOR_ENABLE, HIGH);
}

/********************************************************
  Visual tracking stimulus - right
********************************************************/
void rightStimulus() {
  digitalWrite (MOTOR_ENABLE, LOW);
  strip.setPixelColor(7, strip.Color(0, 0, 0, 0) );
  strip.show();
  digitalWrite (MOTOR_ENABLE, HIGH);

}
