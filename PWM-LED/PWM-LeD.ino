#include "driver/ledc.h"

const int ledPin = 2;      // Dahili LED
const int freq = 5000;     // 5 kHz
const int resolution = 8;  // 8-bit (0–255)

void setup() {
  // LEDC yapılandırma
  ledc_channel_config_t ledChannel = {
    .gpio_num   = ledPin,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel    = LEDC_CHANNEL_0,
    .intr_type  = LEDC_INTR_DISABLE,
    .timer_sel  = LEDC_TIMER_0,
    .duty       = 0,
    .hpoint     = 0
  };

  ledc_timer_config_t ledTimer = {
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .duty_resolution = LEDC_TIMER_8_BIT,
    .timer_num = LEDC_TIMER_0,
    .freq_hz = freq,
    .clk_cfg = LEDC_AUTO_CLK
  };

  // Zamanlayıcıyı ve kanalı başlat
  ledc_timer_config(&ledTimer);
  ledc_channel_config(&ledChannel);
}

void loop() {
  // Parlaklığı artır
  for (int duty = 0; duty <= 255; duty++) {
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    delay(10);
  }
  // Parlaklığı azalt
  for (int duty = 255; duty >= 0; duty--) {
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    delay(10);
  }
}
