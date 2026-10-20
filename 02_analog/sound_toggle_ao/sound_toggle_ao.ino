const byte SOUND_AO = A0;
const byte LED_PIN = 9;
const int THRESHOLD = 550;             // 이 값 이상으로 흔들리면 소리로 판단 (시리얼 Peak 값을 보고 조절)
const unsigned long WINDOW_MS = 20;     // 20ms 구간의 최대-최소 차이를 소리 크기로 사용
const unsigned long COOLDOWN_MS = 250;  // 한 번 감지 후 이 시간 동안은 무시
bool ledOn = false;
bool wasActive = false;
unsigned long lastEvent = 0;
unsigned long eventCount = 0;
unsigned long windowAt = 0;
unsigned long lastReport = 0;
int minimumAdc = 1023;
int maximumAdc = 0;
int peakToPeak = 0;
int lastRaw = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  lastEvent = millis() - COOLDOWN_MS;
  windowAt = millis();
  Serial.println(F("Sound ready: clap to toggle LED (AO peak-to-peak >= THRESHOLD)"));
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

    const bool active = peakToPeak >= THRESHOLD;
    if (active && !wasActive && now - lastEvent >= COOLDOWN_MS) {
      lastEvent = now;
      eventCount++;
      ledOn = !ledOn;
      digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
    }
    wasActive = active;
  }

  if (now - lastReport >= 100) {
    lastReport = now;
    Serial.print(F("AO:")); Serial.print(lastRaw);
    Serial.print(F(" Peak:")); Serial.print(peakToPeak);
    Serial.print(F(" Active:")); Serial.print(wasActive ? 1 : 0);
    Serial.print(F(" LED:")); Serial.print(ledOn ? 1 : 0);
    Serial.print(F(" Count:")); Serial.println(eventCount);
  }
}
