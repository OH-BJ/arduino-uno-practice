const int BUTTON_PIN = 2;
const int LED_PIN = 3;

const unsigned long DEBOUNCE_TIME = 50;

int lastReading = HIGH;     // previous value (read in the last loop)
int buttonState = HIGH;     // confirmed button state
unsigned long lastChangeTime = 0;

bool ledOn = false;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);   // HIGH when released, LOW when pressed
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);   // current value

  // If the current value differs from the previous one, it's bouncing, so restart the timer
  if (reading != lastReading) {
    lastChangeTime = millis();
  }

  // If the value stays the same long enough, accept it as a real state change
  if (millis() - lastChangeTime > DEBOUNCE_TIME && reading != buttonState) {
    buttonState = reading;

    // Toggle the LED on each press
    if (buttonState == LOW) {
      ledOn = !ledOn;
      digitalWrite(LED_PIN, ledOn);
    }
  }

  lastReading = reading;
}
