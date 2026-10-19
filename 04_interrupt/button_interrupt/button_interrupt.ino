const int BUTTON_PIN = 2;   // Uno interrupt pins: 2, 3
const int LED_PIN = A5;

const unsigned long DEBOUNCE_TIME = 50;

volatile bool edgeDetected = false;
volatile unsigned long lastEdgeTime = 0;

int buttonState = HIGH;     // confirmed button state
int pressCount = 0;
bool ledOn = false;

void onButtonChange() {
  edgeDetected = true;
  lastEdgeTime = millis();
}

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);   // HIGH when released, LOW when pressed
  pinMode(LED_PIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), onButtonChange, CHANGE);
}

void loop() {
  noInterrupts();
  bool detected = edgeDetected;
  unsigned long edgeTime = lastEdgeTime;
  interrupts();

  // Wait until the bouncing has settled, then check the real state
  if (detected && millis() - edgeTime > DEBOUNCE_TIME) {
    edgeDetected = false;

    int reading = digitalRead(BUTTON_PIN);
    if (reading != buttonState) {
      buttonState = reading;

      // Toggle only on press, ignore release
      if (buttonState == LOW) {
        pressCount++;
        ledOn = !ledOn;
        digitalWrite(LED_PIN, ledOn);

        Serial.print("Press count: ");
        Serial.print(pressCount);
        Serial.print(" -> LED ");
        Serial.println(ledOn ? "ON" : "OFF");
      }
    }
  }
}
