// 소리 센서로 LED 밝기 조절: 소리가 클수록 LED가 밝아짐
const byte SOUND_AO_PIN = A0;  // 센서 AO: 소리 크기 (아날로그)
const byte SOUND_DO_PIN = 2;   // 센서 DO: 기준보다 큰 소리 감지 (디지털)
const byte LED_PIN = 9;        // Uno에서 ~ 표시가 있는 PWM 핀

const unsigned long SAMPLE_WINDOW = 50;  // 50ms 동안 파형의 최대/최소를 잰다
const int MAX_AMPLITUDE = 512;           // 이 진폭 이상이면 LED 최대 밝기

void setup() {
  Serial.begin(9600);
  pinMode(SOUND_DO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.println(F("Sound sensor ready: amplitude 0..512 -> PWM 0..255"));
}

void loop() {
  // 소리는 위아래로 흔들리는 파형이라 한 번 읽은 값보다 흔들림의 폭(진폭)이 소리 크기다
  int minValue = 1023;
  int maxValue = 0;
  const unsigned long start = millis();

  while (millis() - start < SAMPLE_WINDOW) {
    const int raw = analogRead(SOUND_AO_PIN);
    if (raw < minValue) minValue = raw;
    if (raw > maxValue) maxValue = raw;
  }

  const int amplitude = maxValue - minValue;
  const int pwm = map(constrain(amplitude, 0, MAX_AMPLITUDE), 0, MAX_AMPLITUDE, 0, 255);
  analogWrite(LED_PIN, pwm);

  const int detected = digitalRead(SOUND_DO_PIN);

  Serial.print(F("Amplitude:")); Serial.print(amplitude);
  Serial.print(F(" PWM:")); Serial.print(pwm);
  Serial.print(F(" DO:")); Serial.println(detected);
}
