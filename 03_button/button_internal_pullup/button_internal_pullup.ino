const int BUTTON_PIN = A0;

const unsigned long DEBOUNCE_TIME = 50;

int lastState = HIGH;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);   // 안 누르면 HIGH, 누르면 LOW
}

void loop() {
  int state = digitalRead(BUTTON_PIN);

  if (state != lastState) {
    if (state == LOW) {
      Serial.println("버튼 눌림");
    } else {
      Serial.println("버튼 뗌");
    }
    lastState = state;
    delay(DEBOUNCE_TIME);   // 채터링 방지
  }
}
