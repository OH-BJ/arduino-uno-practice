const int BUTTON_PIN = A0;
const int LED_PIN = A5;

const unsigned long DEBOUNCE_TIME = 50;
const unsigned long DOUBLE_CLICK_GAP = 300;   // max time allowed before the second click
const unsigned long LONG_PRESS_TIME = 1000;   // hold time to count as a long press

int lastReading = HIGH;     // previous value (read in the last loop)
int buttonState = HIGH;     // confirmed button state
unsigned long lastChangeTime = 0;

unsigned long pressStartTime = 0;
unsigned long lastReleaseTime = 0;
int clickCount = 0;
bool longPressHandled = false;

void setup() {
  Serial.begin(9600);
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

    if (buttonState == LOW) {
      // Pressed: if it came too long after the first click, start counting over
      if (clickCount == 1 && millis() - lastReleaseTime > DOUBLE_CLICK_GAP) {
        clickCount = 0;
      }
      pressStartTime = millis();
      longPressHandled = false;
    } else {
      // Released: count it as a short click unless it was a long press
      if (!longPressHandled) {
        clickCount++;
        lastReleaseTime = millis();

        if (clickCount == 2) {
          digitalWrite(LED_PIN, HIGH);
          Serial.println("Double click -> LED ON");
          clickCount = 0;
        }
      }
    }
  }

  // Check for a long press while the button is held
  if (buttonState == LOW && !longPressHandled && millis() - pressStartTime >= LONG_PRESS_TIME) {
    digitalWrite(LED_PIN, LOW);
    Serial.println("Long press -> LED OFF");
    longPressHandled = true;
    clickCount = 0;
  }

  // Single click with no follow-up: do nothing and reset
  if (buttonState == HIGH && clickCount == 1 && millis() - lastReleaseTime > DOUBLE_CLICK_GAP) {
    clickCount = 0;
  }

  lastReading = reading;
}
