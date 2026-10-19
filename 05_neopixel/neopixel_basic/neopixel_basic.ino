#include <Adafruit_NeoPixel.h>

const int PIXEL_PIN = 6;
const int PIXEL_COUNT = 8;

const unsigned long STEP_TIME = 100;

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

void colorWipe(uint32_t color) {
  for (int i = 0; i < PIXEL_COUNT; i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(STEP_TIME);
  }
}

void rainbow() {
  for (long hue = 0; hue < 65536; hue += 512) {
    for (int i = 0; i < PIXEL_COUNT; i++) {
      long pixelHue = hue + (i * 65536L / PIXEL_COUNT);
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show();
    delay(10);
  }
}

void setup() {
  strip.begin();
  strip.setBrightness(50);   // 0 ~ 255
  strip.show();              // 전부 끈 상태로 시작
}

void loop() {
  colorWipe(strip.Color(255, 0, 0));   // 빨강
  colorWipe(strip.Color(0, 255, 0));   // 초록
  colorWipe(strip.Color(0, 0, 255));   // 파랑
  rainbow();
}
