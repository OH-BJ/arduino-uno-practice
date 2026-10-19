const int RED_PIN = 3;
const int YELLOW_PIN = 4;
const int GREEN_PIN = 5;

const unsigned long GREEN_TIME = 5000;
const unsigned long YELLOW_TIME = 2000;
const unsigned long RED_TIME = 5000;

void setLights(bool red, bool yellow, bool green) {
  digitalWrite(RED_PIN, red);
  digitalWrite(YELLOW_PIN, yellow);
  digitalWrite(GREEN_PIN, green);
}

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
}

void loop() {
  setLights(LOW, LOW, HIGH);   // 초록불
  delay(GREEN_TIME);

  setLights(LOW, HIGH, LOW);   // 노란불
  delay(YELLOW_TIME);

  setLights(HIGH, LOW, LOW);   // 빨간불
  delay(RED_TIME);
}
