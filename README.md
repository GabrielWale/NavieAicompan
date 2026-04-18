# NavieAicompan
is a project make esp32 dev which is usefull LLM stage one wiht the brain in the computer

NOTE
tessting Audio
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "BluetoothA2DPSink.h"

// Konfigurasi Pin OLED sesuai rakitan
#define SDA_PIN 13
#define SCL_PIN 14
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
BluetoothA2DPSink a2dp_sink;

void setup() {
  Serial.begin(115200);

  // --- 1. Inisialisasi Layar OLED ---
  Wire.begin(SDA_PIN, SCL_PIN);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED gagal memuat");
    for(;;);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Gerry Speaker: ON");
  display.setCursor(0, 30);
  display.println("Menunggu HP...");
  display.display();

  // --- 2. Inisialisasi Bluetooth Audio ---
  // Mengarahkan output audio ke Internal DAC ESP32 (Pin 25 untuk L, Pin 26 untuk R)
  static const i2s_config_t i2s_config = {
      .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
      .sample_rate = 44100, 
      .bits_per_sample = (i2s_bits_per_sample_t) 16,
      .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
      .communication_format = (i2s_comm_format_t) (I2S_COMM_FORMAT_STAND_I2S),
      .intr_alloc_flags = 0, 
      .dma_buf_count = 8,
      .dma_buf_len = 64,
      .use_apll = false
  };
  
  a2dp_sink.set_i2s_config(i2s_config);
  
  // Nama Bluetooth yang akan muncul di HP-mu
  a2dp_sink.start("Gerry_BT_Speaker"); 
}

void loop() {
  // Pemrosesan audio ditangani di background oleh ESP32
  // Bagian loop ini dibiarkan kosong untuk saat ini
}
