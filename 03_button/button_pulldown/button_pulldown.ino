const int BUTTON_PIN = 2;
const int LED_PIN = 3;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);   // 외부 풀다운 저항 사용: 안 누르면 LOW, 누르면 HIGH
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int state = digitalRead(BUTTON_PIN);

  digitalWrite(LED_PIN, state == HIGH);   // 누르는 동안 LED 켜짐

  Serial.println(state == HIGH ? "눌림" : "안 눌림");
  delay(100);
}
