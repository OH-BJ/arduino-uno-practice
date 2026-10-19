const int BUTTON_PIN = 2;
const int LED_PIN = 3;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);   // 외부 풀업 저항 사용: 안 누르면 HIGH, 누르면 LOW
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int state = digitalRead(BUTTON_PIN);

  digitalWrite(LED_PIN, state == LOW);   // 누르는 동안 LED 켜짐

  Serial.println(state == LOW ? "눌림" : "안 눌림");
  delay(100);
}
