// 반응속도 게임: 카운트다운 중 초록일 때 버튼을 누르면 +1, 그 외에는 -1 (0 미만 없음)
// 9점이 되면 LED 전부 3초 동안 깜빡인 뒤 초기화
// 7세그먼트: 5161AS 공통 캐소드 (COM = GND), A~G 각각 220~330Ω 저항
const byte SEGMENT_PINS[] = {2, 3, 4, 5, 6, 7, 8};  // A, B, C, D, E, F, G
const byte BUTTON_PIN = 10;
const byte RED_PIN = 11;
const byte ORANGE_PIN = 12;
const byte GREEN_PIN = 13;

const byte DIGITS[] = {
  0x3F, 0x06, 0x5B, 0x4F, 0x66,
  0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

const unsigned long COUNTDOWN_MS = 10000;    // 카운트다운 시간 (임의로 변경 가능)
const unsigned long ORANGE_FROM_MS = 5000;   // 남은 시간이 이 이하면 주황
const unsigned long GREEN_FROM_MS = 2000;    // 남은 시간이 이 이하면 초록
const byte WIN_SCORE = 9;
const unsigned long WIN_BLINK_TOTAL_MS = 3000;
const unsigned long WIN_BLINK_STEP_MS = 250;
const unsigned long DEBOUNCE_MS = 35;

enum Light { RED, ORANGE, GREEN };

byte score = 0;
unsigned long roundStartAt = 0;
bool winning = false;
unsigned long winStartAt = 0;
unsigned long lastReportedSecond = 0;

int lastReading = HIGH;   // 과거값
int stableButton = HIGH;  // 확정된 버튼 상태
unsigned long changedAt = 0;

void showScore() {
  for (byte segment = 0; segment < 7; segment++) {
    digitalWrite(SEGMENT_PINS[segment], bitRead(DIGITS[score], segment));
  }
}

void setLights(bool red, bool orange, bool green) {
  digitalWrite(RED_PIN, red);
  digitalWrite(ORANGE_PIN, orange);
  digitalWrite(GREEN_PIN, green);
}

Light currentLight(unsigned long now) {
  const unsigned long elapsed = now - roundStartAt;
  const unsigned long remaining = elapsed >= COUNTDOWN_MS ? 0 : COUNTDOWN_MS - elapsed;
  if (remaining <= GREEN_FROM_MS) return GREEN;
  if (remaining <= ORANGE_FROM_MS) return ORANGE;
  return RED;
}

void startRound(unsigned long now) {
  roundStartAt = now;
  lastReportedSecond = COUNTDOWN_MS / 1000 + 1;
}

void resetGame(unsigned long now) {
  score = 0;
  winning = false;
  showScore();
  startRound(now);
  Serial.println(F("Game reset"));
}

void onButtonPressed(unsigned long now) {
  if (currentLight(now) == GREEN) {
    score++;
    Serial.print(F("GREEN! +1 -> Score: ")); Serial.println(score);
    showScore();
    if (score >= WIN_SCORE) {
      winning = true;
      winStartAt = now;
      Serial.println(F("9 points! Blinking for 3 seconds"));
      return;
    }
    startRound(now);  // 맞혔으면 다음 카운트다운 시작
  } else {
    if (score > 0) score--;  // 0 미만으로 떨어지지 않음
    Serial.print(F("Too early! -1 -> Score: ")); Serial.println(score);
    showScore();
  }
}

void setup() {
  Serial.begin(9600);
  for (byte pin : SEGMENT_PINS) pinMode(pin, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // 안 누르면 HIGH, 누르면 LOW
  pinMode(RED_PIN, OUTPUT);
  pinMode(ORANGE_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  resetGame(millis());
}

void loop() {
  const unsigned long now = millis();

  // 9점 달성: 3초 동안 LED 전부 깜빡인 뒤 초기화
  if (winning) {
    const bool on = ((now - winStartAt) / WIN_BLINK_STEP_MS) % 2 == 0;
    setLights(on, on, on);
    if (now - winStartAt >= WIN_BLINK_TOTAL_MS) resetGame(now);
    return;
  }

  // 카운트다운이 끝나면 다시 처음부터
  if (now - roundStartAt >= COUNTDOWN_MS) startRound(now);

  const Light light = currentLight(now);
  setLights(light == RED, light == ORANGE, light == GREEN);

  // 남은 초를 시리얼에 표시
  const unsigned long remainingSecond = (COUNTDOWN_MS - (now - roundStartAt) + 999) / 1000;
  if (remainingSecond != lastReportedSecond) {
    lastReportedSecond = remainingSecond;
    Serial.print(F("Countdown: ")); Serial.print(remainingSecond);
    Serial.print(F("  Light: ")); Serial.println(light == GREEN ? F("GREEN") : light == ORANGE ? F("ORANGE") : F("RED"));
  }

  // 버튼: 현재값과 과거값을 비교해 떨림을 거르고, 누르는 순간만 처리
  const int reading = digitalRead(BUTTON_PIN);
  if (reading != lastReading) changedAt = now;
  lastReading = reading;
  if (now - changedAt >= DEBOUNCE_MS && reading != stableButton) {
    stableButton = reading;
    if (stableButton == LOW) onButtonPressed(now);
  }
}
