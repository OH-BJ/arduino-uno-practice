// 소리가 안 들리면 빨간 LED, 소리가 들리는 중이면 초록 LED
const byte SOUND_AO = A0;
const byte RED_LED_PIN = 9;
const byte GREEN_LED_PIN = 10;
const int THRESHOLD = 550;           // 이 값 이상으로 흔들리면 소리로 판단 (시리얼 Peak 값을 보고 조절)
const unsigned long WINDOW_MS = 20;  // 20ms 구간의 최대-최소 차이를 소리 크기로 사용
const unsigned long HOLD_MS = 300;   // 소리가 멈춘 뒤에도 초록을 이 시간만큼 유지
bool hearing = false;
unsigned long lastSoundAt = 0;
unsigned long windowAt = 0;
unsigned long lastReport = 0;
int minimumAdc = 1023;
int maximumAdc = 0;
int peakToPeak = 0;
int lastRaw = 0;

void setup() {
  Serial.begin(9600);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  windowAt = millis();
  Serial.println(F("Sound ready: quiet -> RED, sound -> GREEN"));
}

void loop() {
  const unsigned long now = millis();
  lastRaw = analogRead(SOUND_AO);
  minimumAdc = min(minimumAdc, lastRaw);
  maximumAdc = max(maximumAdc, lastRaw);

  // 마이크 AO는 파형이므로 20ms 구간의 최대-최소 차이(진폭)로 소리 크기를 판단합니다.
  if (now - windowAt >= WINDOW_MS) {
    peakToPeak = maximumAdc - minimumAdc;
    minimumAdc = 1023;
    maximumAdc = 0;
    windowAt = now;

    if (peakToPeak >= THRESHOLD) {
      lastSoundAt = now;
    }
  }

  // 마지막으로 소리가 난 뒤 HOLD_MS 안이면 "소리가 들리는 중"
  hearing = (lastSoundAt != 0) && (now - lastSoundAt < HOLD_MS);
  digitalWrite(GREEN_LED_PIN, hearing ? HIGH : LOW);
  digitalWrite(RED_LED_PIN, hearing ? LOW : HIGH);

  if (now - lastReport >= 100) {
    lastReport = now;
    Serial.print(F("AO:")); Serial.print(lastRaw);
    Serial.print(F(" Peak:")); Serial.print(peakToPeak);
    Serial.print(F(" State:")); Serial.println(hearing ? F("GREEN") : F("RED"));
  }
}
