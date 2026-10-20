// 버튼을 누를 때마다 7세그먼트 숫자 0 -> 9 -> 0 ... 증가, 누를 때 LED 반짝
const byte SEGMENT_PINS[7] = {2, 3, 4, 5, 6, 7, 8};  // a, b, c, d, e, f, g
const byte LED_PIN = 9;
const byte BUTTON_PIN = 10;
const bool COMMON_ANODE = false;  // 공통 애노드 디스플레이면 true로 변경

const unsigned long DEBOUNCE_MS = 50;
const unsigned long LED_FLASH_MS = 100;

// 숫자별로 켤 막대 (1 = 켜짐), 순서: a b c d e f g
const byte DIGITS[10][7] = {
  {1, 1, 1, 1, 1, 1, 0},  // 0
  {0, 1, 1, 0, 0, 0, 0},  // 1
  {1, 1, 0, 1, 1, 0, 1},  // 2
  {1, 1, 1, 1, 0, 0, 1},  // 3
  {0, 1, 1, 0, 0, 1, 1},  // 4
  {1, 0, 1, 1, 0, 1, 1},  // 5
  {1, 0, 1, 1, 1, 1, 1},  // 6
  {1, 1, 1, 0, 0, 0, 0},  // 7
  {1, 1, 1, 1, 1, 1, 1},  // 8
  {1, 1, 1, 1, 0, 1, 1},  // 9
};

int number = 0;
int lastReading = HIGH;   // 과거값
int buttonState = HIGH;   // 확정된 버튼 상태
unsigned long lastChangeTime = 0;
unsigned long ledOnAt = 0;

void showDigit(int digit) {
  for (int i = 0; i < 7; i++) {
    bool on = DIGITS[digit][i] == 1;
    if (COMMON_ANODE) on = !on;  // 공통 애노드는 LOW일 때 켜짐
    digitalWrite(SEGMENT_PINS[i], on ? HIGH : LOW);
  }
}

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 7; i++) {
    pinMode(SEGMENT_PINS[i], OUTPUT);
  }
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // 안 누르면 HIGH, 누르면 LOW
  showDigit(number);
  Serial.println(F("7-segment ready: press button to count 0..9"));
}

void loop() {
  const unsigned long now = millis();
  const int reading = digitalRead(BUTTON_PIN);  // 현재값

  if (reading != lastReading) {
    lastChangeTime = now;
  }

  if (now - lastChangeTime > DEBOUNCE_MS && reading != buttonState) {
    buttonState = reading;
    if (buttonState == LOW) {
      number = (number + 1) % 10;
      showDigit(number);
      digitalWrite(LED_PIN, HIGH);
      ledOnAt = now;
      Serial.print(F("Number: ")); Serial.println(number);
    }
  }
  lastReading = reading;

  if (ledOnAt != 0 && now - ledOnAt >= LED_FLASH_MS) {
    digitalWrite(LED_PIN, LOW);
    ledOnAt = 0;
  }
}
