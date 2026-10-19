const int BUTTON_PIN = A0;
const int LED_PIN = A5;

const unsigned long DEBOUNCE_TIME = 50;
const int TOGGLE_EVERY = 5;

int lastReading = HIGH;     // 과거값 (직전 loop에서 읽은 값)
int buttonState = HIGH;     // 확정된 버튼 상태
unsigned long lastChangeTime = 0;

int pressCount = 0;
bool ledOn = false;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);   // 안 누르면 HIGH, 누르면 LOW
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);   // 현재값

  // 현재값이 과거값과 다르면 흔들리는 중이므로 시간을 새로 잰다
  if (reading != lastReading) {
    lastChangeTime = millis();
  }

  // 일정 시간 동안 값이 그대로면 진짜 상태 변화로 인정
  if (millis() - lastChangeTime > DEBOUNCE_TIME && reading != buttonState) {
    buttonState = reading;

    if (buttonState == LOW) {
      pressCount++;

      Serial.print("버튼 누른 횟수: ");
      Serial.println(pressCount);

      // 5번 누를 때마다 LED 토글
      if (pressCount % TOGGLE_EVERY == 0) {
        ledOn = !ledOn;
        digitalWrite(LED_PIN, ledOn);
      }
    }
  }

  lastReading = reading;
}
