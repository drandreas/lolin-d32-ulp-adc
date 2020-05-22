#include <esp_sleep.h>
#include <esp32/ulp.h>
#include <driver/adc.h>
#include <driver/rtc_io.h>

#include "ulp_main.h"
#include "ulptool.h"

// Unlike the esp-idf always use these binary blob names
extern const uint8_t ulp_main_bin_start[] asm("_binary_ulp_main_bin_start");
extern const uint8_t ulp_main_bin_end[]   asm("_binary_ulp_main_bin_end");

void setup() {
  Serial.print("Starting ULP Coprocessor...");

  // Configure ADC for Battery Voltage measurement...
  adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_ulp_enable();

  // Load code and schedule ULP wake up
  ulp_set_wakeup_period(0, 5 * 1000 * 1000);
  esp_err_t err = ulptool_load_binary(0,
                                      ulp_main_bin_start,
                                      (ulp_main_bin_end - ulp_main_bin_start) / sizeof(uint32_t));
  err = ulp_run((&ulp_entry - RTC_SLOW_MEM) / sizeof(uint32_t));
  if (err) {
    Serial.println("failed");
  } else {
    Serial.println("success");
  }
}

void loop() {
  printf("Voltage: %i\n", (ulp_voltage & 0xFFFF));
  delay(5000);
}
