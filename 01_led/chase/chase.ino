const int LED_PINS[] = {3, 4, 5};
const int LED_COUNT = 3;

const unsigned long STEP_TIME = 300;

void setup() {
  for (int i = 0; i < LED_COUNT; i++) {
    pinMode(LED_PINS[i], OUTPUT);
  }
}

void loop() {
  for (int i = 0; i < LED_COUNT; i++) {
    digitalWrite(LED_PINS[i], HIGH);
    delay(STEP_TIME);
    digitalWrite(LED_PINS[i], LOW);
  }
}
