const int POT1_PIN = A4;
const int POT2_PIN = A5;

const unsigned long READ_INTERVAL = 200;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int value1 = analogRead(POT1_PIN);   // 0 ~ 1023
  int value2 = analogRead(POT2_PIN);   // 0 ~ 1023

  Serial.print("가변저항1: ");
  Serial.print(value1);
  Serial.print("  가변저항2: ");
  Serial.println(value2);

  delay(READ_INTERVAL);
}
