#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "AudioTools.h"
#include "BluetoothA2DPSink.h"

#define SDA_PIN 13
#define SCL_PIN 14
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

AnalogAudioStream out;
BluetoothA2DPSink a2dp_sink(out);

unsigned long lastBlinkTime = 0;
int blinkInterval = 3000;
bool isBlinking = false;
int eyeSize = 15;

void drawEyes(int h_offset, bool blink, int audioLevel) {
  display.clearDisplay();
  
  int eyeY = 32;
  int eyeXLeft = 40;
  int eyeXRight = 88;
  int currentEyeHeight = eyeSize;

  if (blink) {
    currentEyeHeight = 2;
  } else {
    currentEyeHeight = eyeSize + map(audioLevel, 0, 100, 0, 10);
  }

  display.fillRoundRect(eyeXLeft - 10, eyeY - (currentEyeHeight / 2), 20, currentEyeHeight, 10, WHITE);
  display.fillRoundRect(eyeXRight - 10, eyeY - (currentEyeHeight / 2), 20, currentEyeHeight, 10, WHITE);
  
  display.display();
}

int globalAudioLevel = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED gagal!");
    while(true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20, 25);
  display.println("Navie_v2 Ready...");
  display.display();
  delay(2000);

  a2dp_sink.start("Navie_v2");
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastBlinkTime > blinkInterval) {
    isBlinking = true;
    if (currentMillis - lastBlinkTime > (blinkInterval + 150)) {
      isBlinking = false;
      lastBlinkTime = currentMillis;
      blinkInterval = random(2000, 6000);
    }
  }

  if (a2dp_sink.get_audio_state() == ESP_A2D_AUDIO_STATE_STARTED) {
      globalAudioLevel = random(0, 30); 
  } else {
      globalAudioLevel = 0;
  }

  drawEyes(0, isBlinking, globalAudioLevel);
}