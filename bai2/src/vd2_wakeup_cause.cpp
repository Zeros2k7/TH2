#include <Arduino.h>
#include <driver/rtc_io.h>
#define LED_PIN    2
#define BUTTON_PIN GPIO_NUM_4
#define SLEEP_S    10
RTC_DATA_ATTR int soLanThuc = 0;
const char* tenNguyenNhan(esp_sleep_wakeup_cause_t c) {
  switch (c) {
    case ESP_SLEEP_WAKEUP_TIMER:     return "TIMER (hen gio)";
    case ESP_SLEEP_WAKEUP_EXT0:      return "EXT0 (nut ngoai GPIO4)";
    case ESP_SLEEP_WAKEUP_UNDEFINED: return "UNDEFINED (cap nguon / reset)";
    default:                         return "KHAC";
  }
}
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  delay(500);
  soLanThuc++;
  esp_sleep_wakeup_cause_t nn = esp_sleep_get_wakeup_cause();
  Serial.printf("\n=== Lan thuc #%d | nguyen nhan: %s ===\n", soLanThuc, tenNguyenNhan(nn));
  Serial.printf(">soLanThuc:%d\n>nguyenNhan:%d\n", soLanThuc, (int)nn);
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH); delay(250);
    digitalWrite(LED_PIN, LOW);  delay(250);
  }
  esp_sleep_enable_timer_wakeup((uint64_t)SLEEP_S * 1000000ULL);
  rtc_gpio_pullup_en(BUTTON_PIN);                  // giu muc cao khi ngu
  rtc_gpio_pulldown_dis(BUTTON_PIN);
  esp_sleep_enable_ext0_wakeup(BUTTON_PIN, 0);     // thuc khi GPIO4 xuong LOW
  Serial.printf("Ngu toi da %d giay (hoac nhan nut GPIO4)...\n", SLEEP_S);
  Serial.flush();
  esp_deep_sleep_start();
}

void loop() {}
