const int POT_PIN = A5;

const unsigned long READ_INTERVAL = 200;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int value = analogRead(POT_PIN);   // 0 ~ 1023
  float voltage = value * 5.0 / 1023.0;

  Serial.print("값: ");
  Serial.print(value);
  Serial.print("  전압: ");
  Serial.print(voltage);
  Serial.println(" V");

  delay(READ_INTERVAL);
}
